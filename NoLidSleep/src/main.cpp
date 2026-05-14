// 合盖不睡眠
// 启动时：将"合盖时执行"动作一律设为"不操作"（交流+电池），并阻止系统空闲睡眠。
// 退出时：将"合盖时执行"动作一律设为"睡眠"（交流+电池）。

#include <SDKDDKVer.h>
#define WIN32_LEAN_AND_MEAN             // 从 Windows 头文件中排除极少使用的内容
#include <windows.h>

#include <string>
#include <format>
#include "resource.h"
#include "strings.h"

#include <powrprof.h>
#include <imm.h>
#include <shellapi.h>
#pragma comment(lib, "PowrProf.lib")
#pragma comment(lib, "User32.lib")
#pragma comment(lib, "Gdi32.lib")
#pragma comment(lib, "Imm32.lib")
#pragma comment(lib, "Shell32.lib")

static const Strings& g_s = SelectStrings();

// 电源子组: Power buttons and lid
static const GUID GUID_SUB_BUTTONS =
{ 0x4f971e89, 0xeebd, 0x4455,
  { 0xa8, 0xde, 0x9e, 0x59, 0x04, 0x0e, 0x73, 0x47 } };

// 设置项: Lid close action  (0=不操作, 1=睡眠, 2=休眠, 3=关机)
static const GUID GUID_LIDCLOSE_ACTION =
{ 0x5ca83367, 0x6e45, 0x459f,
  { 0xa2, 0x7b, 0x47, 0x6b, 0x1d, 0x01, 0xc9, 0x36 } };

static const DWORD LID_ON_START = 0; // 启动: 不操作
static const DWORD LID_ON_EXIT = 1; // 退出: 睡眠

static GUID* g_scheme = nullptr;
static bool  g_restored = false;
static HFONT g_font = nullptr;
static HFONT g_fontLink = nullptr;
static HWND  g_hLink = nullptr;
static UINT  g_dpi = 96;

static int Dp(int v) { return MulDiv(v, g_dpi, 96); }

static sv ActionName(DWORD v) {
    switch (v) {
    case 0: return g_s.lid_nothing;
    case 1: return g_s.lid_sleep;
    case 2: return g_s.lid_hibernate;
    case 3: return g_s.lid_shutdown;
    default: return g_s.lid_unknown;
    }
}

// 预计算窗口客户区高度（基于当前语言文本）
static int CalcClientHeight() {
    HFONT font = CreateFontW(
        -Dp(14), 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
        L"Microsoft YaHei UI");
    HDC dc = GetDC(nullptr);
    HFONT old = (HFONT)SelectObject(dc, font);
    const int cw = Dp(440);
    auto measure = [&](const std::wstring& text) {
        RECT rc = { 0, 0, cw, 0 };
        DrawTextW(dc, text.c_str(), -1, &rc, DT_CALCRECT | DT_WORDBREAK | DT_CENTER);
        return rc.bottom - rc.top;
    };

    int y = Dp(18);
    y += measure(std::wstring{g_s.status_ok}) + Dp(8);
    auto act1 = ActionName(LID_ON_START);
    y += measure(std::vformat(g_s.action_fmt, std::make_wformat_args(act1))) + Dp(4);
    y += measure(std::wstring{g_s.info_line}) + Dp(4);
    auto act2 = ActionName(LID_ON_EXIT);
    y += measure(std::vformat(g_s.close_fmt, std::make_wformat_args(act2))) + Dp(10);
    // button width
    SIZE sz = {};
    GetTextExtentPoint32W(dc, g_s.btn_exit.data(), (int)g_s.btn_exit.size(), &sz);
    int bw = sz.cx + Dp(48);
    (void)bw; // x-position not needed for height calc
    y += Dp(32) + Dp(10);   // button height + gap
    y += Dp(20) + Dp(12);   // link + bottom padding

    SelectObject(dc, old);
    ReleaseDC(nullptr, dc);
    DeleteObject(font);
    return y;
}

// 运行时兜底:即使 manifest 未生效也尽量声明 DPI 感知
static void EnableDpiAwareness() {
    HMODULE u32 = GetModuleHandleW(L"user32.dll");
    if (u32) {
        typedef BOOL(WINAPI* PFN_SetCtx)(HANDLE);
        auto p = (PFN_SetCtx)GetProcAddress(u32, "SetProcessDpiAwarenessContext");
        // DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2 == (HANDLE)-4
        if (p && p((HANDLE)-4)) return;
        if (p && p((HANDLE)-3)) return; // PER_MONITOR_AWARE
    }
    HMODULE shc = LoadLibraryW(L"Shcore.dll");
    if (shc) {
        typedef HRESULT(WINAPI* PFN_Set)(int);
        auto p = (PFN_Set)GetProcAddress(shc, "SetProcessDpiAwareness");
        if (p && SUCCEEDED(p(2))) return; // PROCESS_PER_MONITOR_DPI_AWARE
    }
    SetProcessDPIAware();
}

static UINT QueryDpiForWindow(HWND hwnd) {
    HMODULE u32 = GetModuleHandleW(L"user32.dll");
    if (u32) {
        if (hwnd) {
            typedef UINT(WINAPI* PFN_GetDpi)(HWND);
            auto p = (PFN_GetDpi)GetProcAddress(u32, "GetDpiForWindow");
            if (p) {
                UINT d = p(hwnd);
                if (d) return d;
            }
        }
        typedef UINT(WINAPI* PFN_GetSysDpi)(void);
        auto ps = (PFN_GetSysDpi)GetProcAddress(u32, "GetDpiForSystem");
        if (ps) {
            UINT d = ps();
            if (d) return d;
        }
    }
    HDC dc = GetDC(nullptr);
    UINT dpi = dc ? (UINT)GetDeviceCaps(dc, LOGPIXELSY) : 96;
    if (dc) ReleaseDC(nullptr, dc);
    return dpi ? dpi : 96;
}

static bool ApplyBlock() {
    if (PowerGetActiveScheme(nullptr, &g_scheme) != ERROR_SUCCESS || !g_scheme) {
        return false;
    }
    PowerWriteACValueIndex(nullptr, g_scheme,
        &GUID_SUB_BUTTONS, &GUID_LIDCLOSE_ACTION, LID_ON_START);
    PowerWriteDCValueIndex(nullptr, g_scheme,
        &GUID_SUB_BUTTONS, &GUID_LIDCLOSE_ACTION, LID_ON_START);
    PowerSetActiveScheme(nullptr, g_scheme);
    SetThreadExecutionState(ES_CONTINUOUS | ES_SYSTEM_REQUIRED);
    return true;
}

static void Restore() {
    if (g_restored || g_scheme == nullptr) return;
    PowerWriteACValueIndex(nullptr, g_scheme,
        &GUID_SUB_BUTTONS, &GUID_LIDCLOSE_ACTION, LID_ON_EXIT);
    PowerWriteDCValueIndex(nullptr, g_scheme,
        &GUID_SUB_BUTTONS, &GUID_LIDCLOSE_ACTION, LID_ON_EXIT);
    PowerSetActiveScheme(nullptr, g_scheme);
    SetThreadExecutionState(ES_CONTINUOUS);
    g_restored = true;
}

// 测量按钮文本所需宽度（含内边距）
static int CalcButtonWidth(HWND hwnd) {
    HDC dc = GetDC(hwnd);
    HFONT old = (HFONT)SelectObject(dc, g_font);
    RECT rc = {};
    DrawTextW(dc, g_s.btn_exit.data(), -1, &rc, DT_CALCRECT | DT_SINGLELINE);
    SelectObject(dc, old);
    ReleaseDC(hwnd, dc);
    return (rc.right - rc.left) + Dp(48);  // 左右各 24dp 内边距
}

// 测量文本在指定宽度下自动换行后的像素高度
static int MeasureTextH(HWND hwnd, const std::wstring& text, int width) {
    HDC dc = GetDC(hwnd);
    HFONT old = (HFONT)SelectObject(dc, g_font);
    RECT rc = { 0, 0, width, 0 };
    DrawTextW(dc, text.c_str(), -1, &rc, DT_CALCRECT | DT_WORDBREAK | DT_CENTER);
    SelectObject(dc, old);
    ReleaseDC(hwnd, dc);
    return rc.bottom - rc.top;
}

// 创建自动换行的居中静态文本控件，返回其高度
static int CreateLabel(HWND hwnd, const std::wstring& text, int y, int cw, int id) {
    int h = MeasureTextH(hwnd, text, cw);
    CreateWindowW(L"STATIC", text.c_str(),
        WS_CHILD | WS_VISIBLE | SS_CENTER,
        Dp(20), y, cw, h, hwnd, (HMENU)id, nullptr, nullptr);
    return h;
}

static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
    case WM_CREATE: {
        g_font = CreateFontW(
            -Dp(14), 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
            L"Microsoft YaHei UI");

        const int cw = Dp(440);  // 控件可用宽度
        int y = Dp(18);

        y += CreateLabel(hwnd, std::wstring{g_s.status_ok}, y, cw, 101) + Dp(8);

        auto act1 = ActionName(LID_ON_START);
        auto line1 = std::vformat(g_s.action_fmt, std::make_wformat_args(act1));
        y += CreateLabel(hwnd, line1, y, cw, 102) + Dp(4);

        y += CreateLabel(hwnd, std::wstring{g_s.info_line}, y, cw, 103) + Dp(4);

        auto act2 = ActionName(LID_ON_EXIT);
        auto line2 = std::vformat(g_s.close_fmt, std::make_wformat_args(act2));
        y += CreateLabel(hwnd, line2, y, cw, 104) + Dp(10);

        int bw = CalcButtonWidth(hwnd);
        CreateWindowW(L"BUTTON", g_s.btn_exit.data(),
            WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
            (Dp(480) - bw) / 2, y, bw, Dp(32), hwnd, (HMENU)1, nullptr, nullptr);
        y += Dp(42);

        g_fontLink = CreateFontW(
            -Dp(12), 0, 0, 0, FW_NORMAL, FALSE, TRUE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
            L"Microsoft YaHei UI");
        g_hLink = CreateWindowW(L"STATIC", L"https://github.com/jark006/NoLidSleep",
            WS_CHILD | WS_VISIBLE | SS_CENTER | SS_NOTIFY,
            Dp(20), y, cw, Dp(20), hwnd, (HMENU)105, nullptr, nullptr);
        SendMessageW(g_hLink, WM_SETFONT, (WPARAM)g_fontLink, TRUE);

        EnumChildWindows(hwnd, [](HWND child, LPARAM lParam) -> BOOL {
            SendMessageW(child, WM_SETFONT, (WPARAM)lParam, TRUE);
            return TRUE;
            }, (LPARAM)g_font);
        return 0;
    }
    case WM_COMMAND:
        if (LOWORD(wp) == 1) {
            DestroyWindow(hwnd);
        } else if (LOWORD(wp) == 105) {
            ShellExecuteW(nullptr, L"open",
                L"https://github.com/jark006/NoLidSleep",
                nullptr, nullptr, SW_SHOWNORMAL);
        }
        return 0;
    case WM_CLOSE:
        DestroyWindow(hwnd);
        return 0;
    case WM_SETCURSOR:
        if ((HWND)wp == g_hLink) {
            SetCursor(LoadCursorW(nullptr, IDC_HAND));
            return TRUE;
        }
        break;
    case WM_CTLCOLORSTATIC:
        if ((HWND)lp == g_hLink) {
            SetTextColor((HDC)wp, RGB(0, 0, 200));
            SetBkMode((HDC)wp, TRANSPARENT);
            return (LRESULT)GetSysColorBrush(COLOR_BTNFACE);
        }
        break;
    case WM_DESTROY:
        Restore();
        if (g_fontLink) { DeleteObject(g_fontLink); g_fontLink = nullptr; }
        if (g_font) { DeleteObject(g_font); g_font = nullptr; }
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProcW(hwnd, msg, wp, lp);
}

static BOOL WINAPI CtrlHandler(DWORD) {
    Restore();
    return FALSE;
}

int WINAPI wWinMain(_In_ HINSTANCE hInst, _In_opt_ HINSTANCE, _In_ LPWSTR, _In_ int nCmdShow) {
    ::ImmDisableIME(GetCurrentThreadId());
    EnableDpiAwareness();

    const wchar_t* kClass = L"TempNoLidSleepWnd";

    if (!ApplyBlock()) {
        MessageBoxW(nullptr, g_s.err_msg.data(),
            g_s.app_title.data(), MB_OK | MB_ICONERROR);
        return 1;
    }
    SetConsoleCtrlHandler(CtrlHandler, TRUE);

    g_dpi = QueryDpiForWindow(nullptr);

    WNDCLASSEXW wc = { sizeof(wc) };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInst;
    wc.hCursor = LoadCursorW(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
    wc.lpszClassName = kClass;
    wc.hIcon = LoadIconW(hInst, MAKEINTRESOURCE(IDI_NOLIDSLEEP));
    wc.hIconSm = LoadIconW(hInst, MAKEINTRESOURCE(IDI_SMALL));
    RegisterClassExW(&wc);

    DWORD style = (WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME);
    DWORD exStyle = WS_EX_APPWINDOW;

    RECT rc = { 0, 0, Dp(480), CalcClientHeight() };
    BOOL adjusted = FALSE;
    HMODULE u32 = GetModuleHandleW(L"user32.dll");
    if (u32) {
        typedef BOOL(WINAPI* PFN_Adj)(LPRECT, DWORD, BOOL, DWORD, UINT);
        auto p = (PFN_Adj)GetProcAddress(u32, "AdjustWindowRectExForDpi");
        if (p) adjusted = p(&rc, style, FALSE, exStyle, g_dpi);
    }
    if (!adjusted) AdjustWindowRectEx(&rc, style, FALSE, exStyle);

    const int W = rc.right - rc.left;
    const int H = rc.bottom - rc.top;
    int sw = GetSystemMetrics(SM_CXSCREEN);
    int sh = GetSystemMetrics(SM_CYSCREEN);

    HWND hwnd = CreateWindowExW(
        exStyle,
        kClass, g_s.app_title.data(),
        style,
        (sw - W) / 2, (sh - H) / 2, W, H,
        nullptr, nullptr, hInst, nullptr);

    if (!hwnd) {
        Restore();
        return 1;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessageW(&msg, nullptr, 0, 0) > 0) {
        if (msg.message == WM_KEYDOWN &&
            (msg.wParam == VK_ESCAPE || msg.wParam == VK_SPACE)) {
            PostMessageW(hwnd, WM_CLOSE, 0, 0);
            continue;
        }
        if (!IsDialogMessageW(hwnd, &msg)) {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
    }

    Restore();
    if (g_scheme) { LocalFree(g_scheme); g_scheme = nullptr; }
    return 0;
}

# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## 项目简介

NoLidSleep 是一个 Windows 桌面工具，用于临时阻止笔记本合盖睡眠。启动时将"合盖时执行"动作设为"不操作"（交流+电池均生效），同时调用 `SetThreadExecutionState` 阻止系统空闲睡眠；退出时恢复为"睡眠"。

## 构建

使用 Visual Studio 2026 (v145 工具集) 打开 `NoLidSleep.slnx` 构建。

在`PowerShell`环境使用`MSVC生成工具`进行**Release x64**构建

`"C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\MSBuild.exe" NoLidSleep.slnx /p:Configuration=Release
            /p:Platform=x64 /v:minimal`

输出路径: `x64\Release\NoLidSleep.exe`

## 架构

单文件 Win32 GUI 应用，无框架依赖：

- `NoLidSleep/src/main.cpp` — 全部逻辑：电源方案读写、窗口创建、消息循环、DPI 适配
- `NoLidSleep/include/Resource.h` — 资源 ID 定义
- `NoLidSleep/NoLidSleep.manifest` — DPI 感知声明 + Common Controls 6 依赖
- `NoLidSleep/NoLidSleep.rc` — 资源脚本（图标、菜单、对话框）

### 核心机制

1. `ApplyBlock()` — 获取当前电源方案 GUID，通过 `PowerWriteACValueIndex` / `PowerWriteDCValueIndex` 将合盖动作改为 0（不操作），再 `PowerSetActiveScheme` 生效，同时 `SetThreadExecutionState(ES_CONTINUOUS | ES_SYSTEM_REQUIRED)` 阻止空闲睡眠
2. `Restore()` — 将合盖动作改回 1（睡眠），`SetThreadExecutionState(ES_CONTINUOUS)` 取消睡眠阻止
3. `g_restored` 标志确保只恢复一次（`WM_DESTROY`、`CtrlHandler`、`wWinMain` 末尾均调用 `Restore()`）

### 关键 GUID

- 电源子组 `GUID_SUB_BUTTONS` (4f971e89-eebd-4455-a8de-9e59040e7347) — Power buttons and lid
- 设置项 `GUID_LIDCLOSE_ACTION` (5ca83367-6e45-459f-a27b-476b1d01c936) — Lid close action

### DPI 适配

双重保障：manifest 声明 PerMonitorV2 + 运行时 `EnableDpiAwareness()` 兜底。所有布局尺寸通过 `Dp()` 函数按 `g_dpi/96` 缩放。窗口创建时使用 `AdjustWindowRectExForDpi`（优先）或 `AdjustWindowRectEx` 计算客户区。

## 编译选项

- 语言标准: C++23，C17
- 字符集: Unicode (`wWinMain`)
- 额外编译选项: `/utf-8 /Zc:__cplusplus`
- 链接选项: 嵌入 manifest (`/MANIFEST:EMBED /MANIFESTINPUT:NoLidSleep.manifest`)
- 链接库: PowrProf.lib, User32.lib, Gdi32.lib, Imm32.lib

## 退出方式

点击"恢复并退出"按钮、窗口关闭按钮、ESC 键、空格键均可退出，退出时自动恢复电源设置。

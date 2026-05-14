#include "strings.h"
#include <windows.h>
#include <winnls.h>

// ── 中文简体 ──────────────────────────────────────────────────────────────────
constexpr Strings g_zh_cn = {
    L"合盖不睡眠",
    L"✅ 已阻止合盖睡眠",
    L"当前合盖动作: {} (充电状态 + 用电池状态)",
    L"现在可放心合盖，软件会继续运行。",
    L"关闭本窗口将恢复为: {}  (空格 / ESC 可退出)",
    L"恢复并退出",
    L"无法获取/修改当前电源方案。",
    L"不操作", L"睡眠", L"休眠", L"关机", L"未知"
};

// ── 中文繁體 ──────────────────────────────────────────────────────────────────
constexpr Strings g_zh_tw = {
    L"合蓋不睡眠",
    L"✅ 已阻止合蓋睡眠",
    L"目前合蓋動作: {} (充電狀態 + 使用電池狀態)",
    L"現在可放心合蓋，軟體會繼續執行。",
    L"關閉本視窗將恢復為: {}  (空格 / ESC 可退出)",
    L"恢復並退出",
    L"無法取得/修改目前電源計畫。",
    L"不動作", L"睡眠", L"休眠", L"關機", L"未知"
};

// ── English ───────────────────────────────────────────────────────────────────
constexpr Strings g_en = {
    L"NoLidSleep",
    L"✅ Lid sleep blocked",
    L"Lid close action: {} (Plugged in + On battery)",
    L"Safe to close the lid — the app keeps running.",
    L"Closing this window restores: {}  (Space / ESC to quit)",
    L"Restore and Exit",
    L"Failed to read or modify the active power scheme.",
    L"Do nothing", L"Sleep", L"Hibernate", L"Shut down", L"Unknown"
};

// ── 日本語 ───────────────────────────────────────────────────────────────────
constexpr Strings g_ja = {
    L"スリープ防止",
    L"✅ 閉じたスリープをブロック中",
    L"蓋閉じ動作: {} (電源接続 + バッテリ駆動)",
    L"安心して蓋を閉じてください。アプリは引き続き実行されます。",
    L"このウィンドウを閉じると復元: {}  (Space / ESC で終了)",
    L"復元して終了",
    L"電源プランの読み取りまたは変更に失敗しました。",
    L"何もしない", L"スリープ", L"休止状態", L"シャットダウン", L"不明"
};

// ── 한국어 ────────────────────────────────────────────────────────────────────
constexpr Strings g_ko = {
    L"덮개 수면 방지",
    L"✅ 덮개 수면 차단됨",
    L"덮개 닫기 동작: {} (전원 연결 + 배터리)",
    L"안심하고 덮개를 닫으세요. 앱이 계속 실행됩니다.",
    L"이 창을 닫으면 복원: {}  (Space / ESC로 종료)",
    L"복원 후 종료",
    L"현재 전원 구성표를 읽거나 수정하지 못했습니다.",
    L"아무 작업 안 함", L"절전", L"최대 절전", L"시스템 종료", L"알 수 없음"
};

// ── Deutsch ───────────────────────────────────────────────────────────────────
constexpr Strings g_de = {
    L"NoLidSleep",
    L"✅ Deckel-Schlaf blockiert",
    L"Aktion bei Deckelschluss: {} (Netzbetrieb + Akku)",
    L"Der Deckel kann sicher geschlossen werden — die App läuft weiter.",
    L"Schließen dieses Fensters stellt wieder her: {}  (Leertaste / ESC zum Beenden)",
    L"Wiederherstellen && Beenden",
    L"Fehler beim Lesen oder Ändern des aktiven Energieschemas.",
    L"Nichts tun", L"Standby", L"Ruhezustand", L"Herunterfahren", L"Unbekannt"
};

// ── Français ──────────────────────────────────────────────────────────────────
constexpr Strings g_fr = {
    L"NoLidSleep",
    L"✅ Mise en veille au couvercle bloquée",
    L"Action fermeture couvercle : {} (Secteur + Sur batterie)",
    L"Vous pouvez fermer le couvercle en toute sécurité — l'application continue.",
    L"Fermer cette fenêtre restaurera : {}  (Espace / ESC pour quitter)",
    L"Restaurer et quitter",
    L"Impossible de lire ou modifier le profil d'alimentation actif.",
    L"Aucune action", L"Veille", L"Hibernation", L"Arrêter", L"Inconnu"
};

// ── Español ───────────────────────────────────────────────────────────────────
constexpr Strings g_es = {
    L"NoLidSleep",
    L"✅ Suspensión por tapa bloqueada",
    L"Acción al cerrar tapa: {} (Enchufado + Con batería)",
    L"Puede cerrar la tapa con seguridad — la aplicación sigue en ejecución.",
    L"Cerrar esta ventana restaura: {}  (Espacio / ESC para salir)",
    L"Restaurar y salir",
    L"No se pudo leer o modificar el plan de energía activo.",
    L"No hacer nada", L"Suspender", L"Hibernar", L"Apagar", L"Desconocido"
};

// ── Português ─────────────────────────────────────────────────────────────────
constexpr Strings g_pt = {
    L"NoLidSleep",
    L"✅ Suspensão ao fechar tampa bloqueada",
    L"Ação ao fechar tampa: {} (Na tomada + Com bateria)",
    L"Pode fechar a tampa com segurança — o aplicativo continua em execução.",
    L"Fechar esta janela restaura: {}  (Espaço / ESC para sair)",
    L"Restaurar e sair",
    L"Falha ao ler ou modificar o plano de energia ativo.",
    L"Nenhuma ação", L"Suspender", L"Hibernar", L"Desligar", L"Desconhecido"
};

// ── Русский ───────────────────────────────────────────────────────────────────
constexpr Strings g_ru = {
    L"NoLidSleep",
    L"✅ Спящий режим при закрытии заблокирован",
    L"Действие при закрытии крышки: {} (От сети + От батареи)",
    L"Можно безопасно закрыть крышку — приложение продолжит работу.",
    L"Закрытие этого окна восстановит: {}  (Пробел / ESC для выхода)",
    L"Восстановить и выйти",
    L"Не удалось прочитать или изменить текущую схему электропитания.",
    L"Ничего не делать", L"Спящий режим", L"Гибернация", L"Завершение работы", L"Неизвестно"
};

// ── 语言选择 ──────────────────────────────────────────────────────────────────
const Strings& SelectStrings() {
    LANGID lang = GetUserDefaultUILanguage();
    switch (PRIMARYLANGID(lang)) {
    case LANG_CHINESE:  return SUBLANGID(lang) == SUBLANG_CHINESE_TRADITIONAL
                             ? g_zh_tw : g_zh_cn;
    case LANG_JAPANESE: return g_ja;
    case LANG_KOREAN:   return g_ko;
    case LANG_GERMAN:   return g_de;
    case LANG_FRENCH:   return g_fr;
    case LANG_SPANISH:  return g_es;
    case LANG_PORTUGUESE: return g_pt;
    case LANG_RUSSIAN:  return g_ru;
    default:            return g_en;
    }
}

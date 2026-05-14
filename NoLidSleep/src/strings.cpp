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
    L"无操作", L"睡眠", L"休眠", L"关机", L"未知"
};

// ── 中文繁體 ──────────────────────────────────────────────────────────────────
constexpr Strings g_zh = {
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

// ── Italiano ────────────────────────────────────────────────────────────────────
constexpr Strings g_it = {
    L"NoLidSleep",
    L"✅ Sospensione coperchio bloccata",
    L"Azione chiusura coperchio: {} (Allimentazione + A batteria)",
    L"È possibile chiudere il coperchio in sicurezza — l'app continua.",
    L"Chiudere questa finestra ripristina: {}  (Spazio / ESC per uscire)",
    L"Ripristina ed esci",
    L"Impossibile leggere o modificare lo schema alimentazione attivo.",
    L"Nessuna azione", L"Sospensione", L"Ibernazione", L"Arresta", L"Sconosciuto"
};

// ── Nederlands ─────────────────────────────────────────────────────────────────
constexpr Strings g_nl = {
    L"NoLidSleep",
    L"✅ Slaap bij deksel geblokkeerd",
    L"Actie bij deksel sluiten: {} (Op netstroom + Op batterij)",
    L"U kunt het deksel veilig sluiten — de app blijft draaien.",
    L"Dit venster sluiten herstelt: {}  (Spatie / ESC om af te sluiten)",
    L"Herstellen en afsluiten",
    L"Kan het actieve energiebeleid niet lezen of wijzigen.",
    L"Niets doen", L"Slaapstand", L"Slaap naar schijf", L"Afsluiten", L"Onbekend"
};

// ── Polski ──────────────────────────────────────────────────────────────────────
constexpr Strings g_pl = {
    L"NoLidSleep",
    L"✅ Uśpienie po zamknięciu zablokowane",
    L"Działanie przy zamknięciu pokrywy: {} (Zasilacz + Na baterii)",
    L"Można bezpiecznie zamknąć pokrywę — aplikacja nadal działa.",
    L"Zamknięcie tego okna przywraca: {}  (Spacja / ESC, by wyjść)",
    L"Przywróć i wyjdź",
    L"Nie udało się odczytać lub zmodyfikować aktywnego planu zasilania.",
    L"Brak działania", L"Uśpij", L"Hibernuj", L"Wyłącz", L"Nieznane"
};

// ── Türkçe ─────────────────────────────────────────────────────────────────────
constexpr Strings g_tr = {
    L"NoLidSleep",
    L"✅ Kapak uyku modu engellendi",
    L"Kapak kapatma eylemi: {} (Prize takılı + Pilde)",
    L"Kapağı güvenle kapatabilirsiniz — uygulama çalışmaya devam eder.",
    L"Bu pencereyi kapatmak geri yükler: {}  (Boşluk / ESC ile çıkış)",
    L"Geri yükle ve çık",
    L"Etkin güç planı okunamadı veya değiştirilemedi.",
    L"Hiçbir şey yapma", L"Uyku", L"Hazırda beklet", L"Kapat", L"Bilinmiyor"
};

// ── ภาษาไทย ───────────────────────────────────────────────────────────────────
constexpr Strings g_th = {
    L"NoLidSleep",
    L"✅ บล็อกการเข้าสู่โหมดพักเมื่อพับจอ",
    L"การทำงานเมื่อพับจอ: {} (เสียบปลั๊ก + ใช้แบตเตอรี่)",
    L"พับจอได้อย่างปลอดภัย — แอปจะทำงานต่อไป",
    L"ปิดหน้าต่างนี้จะกู้คืน: {}  (Space / ESC เพื่อออก)",
    L"กู้คืนและออก",
    L"ไม่สามารถอ่านหรือแก้ไขแผนพลังงานที่ใช้อยู่",
    L"ไม่ทำอะไร", L"โหมดพัก", L"จำศีล", L"ปิดเครื่อง", L"ไม่ทราบ"
};

// ── Tiếng Việt ─────────────────────────────────────────────────────────────────
constexpr Strings g_vi = {
    L"NoLidSleep",
    L"✅ Đã chặn ngủ khi gập màn hình",
    L"Hành động khi gập màn hình: {} (Cắm điện + Dùng pin)",
    L"Có thể gập màn hình an toàn — ứng dụng vẫn chạy.",
    L"Đóng cửa sổ này sẽ khôi phục: {}  (Space / ESC để thoát)",
    L"Khôi phục và thoát",
    L"Không thể đọc hoặc sửa kế hoạch nguồn điện đang hoạt động.",
    L"Không làm gì", L"Ngủ", L"Ngủ đông", L"Tắt máy", L"Không rõ"
};

// ── Svenska ────────────────────────────────────────────────────────────────────
constexpr Strings g_sv = {
    L"NoLidSleep",
    L"✅ Viloläge vid lock stängt blockerat",
    L"Åtgärd när lock stängs: {} (Nätansluten + På batteri)",
    L"Det är säkert att stänga locket — appen fortsätter köra.",
    L"Stänga detta fönster återställer: {}  (Mellanslag / ESC för att avsluta)",
    L"Återställ och avsluta",
    L"Kunde inte läsa eller ändra det aktiva strömschemat.",
    L"Gör ingenting", L"Viloläge", L"Strömsparläge", L"Stäng av", L"Okänd"
};

// ── Čeština ────────────────────────────────────────────────────────────────────
constexpr Strings g_cs = {
    L"NoLidSleep",
    L"✅ Režim spánku při zavření blokován",
    L"Akce při zavření víka: {} (Na síti + Na baterii)",
    L"Víko lze bezpečně zavřít — aplikace bude pokračovat.",
    L"Zavřením tohoto okna se obnoví: {}  (Mezerník / ESC pro ukončení)",
    L"Obnovit a ukončit",
    L"Nepodařilo se přečíst nebo upravit aktivní schéma napájení.",
    L"Žádná akce", L"Režim spánku", L"Hibernace", L"Vypnout", L"Neznámé"
};

// ── Magyar ─────────────────────────────────────────────────────────────────────
constexpr Strings g_hu = {
    L"NoLidSleep",
    L"✅ Fedél-alvás blokkolva",
    L"Művelet fedél csukásakor: {} (Hálózaton + Akkun)",
    L"Biztonságosan csukható a fedél — az alkalmazás tovább fut.",
    L"Ablak bezárása visszaállítja: {}  (Szóköz / ESC a kilépéshez)",
    L"Visszaállítás és kilépés",
    L"Nem sikerült olvasni vagy módosítani az aktív energiafényt.",
    L"Nincs művelet", L"Alvás", L"Hibernálás", L"Leállítás", L"Ismeretlen"
};

// ── Dansk ──────────────────────────────────────────────────────────────────────
constexpr Strings g_da = {
    L"NoLidSleep",
    L"✅ Dvaletilstand ved lukning blokeret",
    L"Handling ved lukning af låg: {} (Strømstik + På batteri)",
    L"Låget kan lukkes sikkert — appen kører videre.",
    L"Lukning af dette vindue gendanner: {}  (Mellemrum / ESC for at afslutte)",
    L"Gendan og afslut",
    L"Kunne ikke læse eller ændre det aktive strømskema.",
    L"Intet", L"Dvale", L"Dvaletilstand", L"Luk ned", L"Ukendt"
};

// ── 语言选择 ──────────────────────────────────────────────────────────────────
const Strings& SelectStrings() {
    LANGID lang = GetUserDefaultUILanguage();
    switch (PRIMARYLANGID(lang)) {
    case LANG_CHINESE:    return SUBLANGID(lang) == SUBLANG_CHINESE_SIMPLIFIED || 
                                 SUBLANGID(lang) == SUBLANG_CHINESE_SINGAPORE ? 
                                 g_zh_cn : g_zh;
    case LANG_JAPANESE:   return g_ja;
    case LANG_KOREAN:     return g_ko;
    case LANG_GERMAN:     return g_de;
    case LANG_FRENCH:     return g_fr;
    case LANG_SPANISH:    return g_es;
    case LANG_PORTUGUESE: return g_pt;
    case LANG_RUSSIAN:    return g_ru;
    case LANG_ITALIAN:    return g_it;
    case LANG_DUTCH:      return g_nl;
    case LANG_POLISH:     return g_pl;
    case LANG_TURKISH:    return g_tr;
    case LANG_THAI:       return g_th;
    case LANG_VIETNAMESE: return g_vi;
    case LANG_SWEDISH:    return g_sv;
    case LANG_CZECH:      return g_cs;
    case LANG_HUNGARIAN:  return g_hu;
    case LANG_DANISH:     return g_da;
    default:              return g_en;
    }
}

#pragma once
#include <string_view>

using sv = std::wstring_view;

struct Strings {
    sv app_title;
    sv status_ok;
    sv action_fmt;      // {} → action name
    sv info_line;
    sv close_fmt;       // {} → action name
    sv btn_exit;
    sv err_msg;
    sv lid_nothing;
    sv lid_sleep;
    sv lid_hibernate;
    sv lid_shutdown;
    sv lid_unknown;
};

extern const Strings g_zh_cn;
extern const Strings g_zh_tw;
extern const Strings g_en;
extern const Strings g_ja;
extern const Strings g_ko;
extern const Strings g_de;
extern const Strings g_fr;
extern const Strings g_es;
extern const Strings g_pt;
extern const Strings g_ru;
extern const Strings g_it;
extern const Strings g_nl;
extern const Strings g_pl;
extern const Strings g_tr;
extern const Strings g_th;
extern const Strings g_vi;
extern const Strings g_sv;
extern const Strings g_cs;
extern const Strings g_hu;
extern const Strings g_da;

const Strings& SelectStrings();

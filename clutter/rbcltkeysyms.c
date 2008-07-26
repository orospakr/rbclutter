/* Ruby bindings for the Clutter 'interactive canvas' library.
 * Copyright (C) 2007-2008  Neil Roberts
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA  02110-1301  USA
 */

#include <ruby.h>
#include <rbgobject.h>
#include <clutter/clutter-keysyms.h>

#include "rbclutter.h"

static const char rbclt_keysyms_names[] =
  "Key_VoidSymbol\0Key_BackSpace\0Key_Tab\0Key_Linefeed\0Key_Clear\0"
  "Key_Return\0Key_Pause\0Key_Scroll_Lock\0Key_Sys_Req\0Key_Escape\0"
  "Key_Delete\0Key_Multi_key\0Key_Codeinput\0Key_SingleCandidate\0"
  "Key_MultipleCandidate\0Key_PreviousCandidate\0Key_Kanji\0Key_Muhenkan\0"
  "Key_Henkan_Mode\0Key_Henkan\0Key_Romaji\0Key_Hiragana\0Key_Katakana\0"
  "Key_Hiragana_Katakana\0Key_Zenkaku\0Key_Hankaku\0Key_Zenkaku_Hankaku\0"
  "Key_Touroku\0Key_Massyo\0Key_Kana_Lock\0Key_Kana_Shift\0Key_Eisu_Shift\0"
  "Key_Eisu_toggle\0Key_Kanji_Bangou\0Key_Zen_Koho\0Key_Mae_Koho\0Key_Home\0"
  "Key_Left\0Key_Up\0Key_Right\0Key_Down\0Key_Prior\0Key_Page_Up\0Key_Next\0"
  "Key_Page_Down\0Key_End\0Key_Begin\0Key_Select\0Key_Print\0Key_Execute\0"
  "Key_Insert\0Key_Undo\0Key_Redo\0Key_Menu\0Key_Find\0Key_Cancel\0Key_Help\0"
  "Key_Break\0Key_Mode_switch\0Key_script_switch\0Key_Num_Lock\0Key_KP_Space\0"
  "Key_KP_Tab\0Key_KP_Enter\0Key_KP_F1\0Key_KP_F2\0Key_KP_F3\0Key_KP_F4\0"
  "Key_KP_Home\0Key_KP_Left\0Key_KP_Up\0Key_KP_Right\0Key_KP_Down\0"
  "Key_KP_Prior\0Key_KP_Page_Up\0Key_KP_Next\0Key_KP_Page_Down\0Key_KP_End\0"
  "Key_KP_Begin\0Key_KP_Insert\0Key_KP_Delete\0Key_KP_Equal\0Key_KP_Multiply\0"
  "Key_KP_Add\0Key_KP_Separator\0Key_KP_Subtract\0Key_KP_Decimal\0"
  "Key_KP_Divide\0Key_KP_0\0Key_KP_1\0Key_KP_2\0Key_KP_3\0Key_KP_4\0Key_KP_5\0"
  "Key_KP_6\0Key_KP_7\0Key_KP_8\0Key_KP_9\0Key_F1\0Key_F2\0Key_F3\0Key_F4\0"
  "Key_F5\0Key_F6\0Key_F7\0Key_F8\0Key_F9\0Key_F10\0Key_F11\0Key_L1\0Key_F12\0"
  "Key_L2\0Key_F13\0Key_L3\0Key_F14\0Key_L4\0Key_F15\0Key_L5\0Key_F16\0"
  "Key_L6\0Key_F17\0Key_L7\0Key_F18\0Key_L8\0Key_F19\0Key_L9\0Key_F20\0"
  "Key_L10\0Key_F21\0Key_R1\0Key_F22\0Key_R2\0Key_F23\0Key_R3\0Key_F24\0"
  "Key_R4\0Key_F25\0Key_R5\0Key_F26\0Key_R6\0Key_F27\0Key_R7\0Key_F28\0"
  "Key_R8\0Key_F29\0Key_R9\0Key_F30\0Key_R10\0Key_F31\0Key_R11\0Key_F32\0"
  "Key_R12\0Key_F33\0Key_R13\0Key_F34\0Key_R14\0Key_F35\0Key_R15\0"
  "Key_Shift_L\0Key_Shift_R\0Key_Control_L\0Key_Control_R\0Key_Caps_Lock\0"
  "Key_Shift_Lock\0Key_Meta_L\0Key_Meta_R\0Key_Alt_L\0Key_Alt_R\0Key_Super_L\0"
  "Key_Super_R\0Key_Hyper_L\0Key_Hyper_R\0Key_ISO_Lock\0Key_ISO_Level2_Latch\0"
  "Key_ISO_Level3_Shift\0Key_ISO_Level3_Latch\0Key_ISO_Level3_Lock\0"
  "Key_ISO_Group_Shift\0Key_ISO_Group_Latch\0Key_ISO_Group_Lock\0"
  "Key_ISO_Next_Group\0Key_ISO_Next_Group_Lock\0Key_ISO_Prev_Group\0"
  "Key_ISO_Prev_Group_Lock\0Key_ISO_First_Group\0Key_ISO_First_Group_Lock\0"
  "Key_ISO_Last_Group\0Key_ISO_Last_Group_Lock\0Key_ISO_Left_Tab\0"
  "Key_ISO_Move_Line_Up\0Key_ISO_Move_Line_Down\0Key_ISO_Partial_Line_Up\0"
  "Key_ISO_Partial_Line_Down\0Key_ISO_Partial_Space_Left\0"
  "Key_ISO_Partial_Space_Right\0Key_ISO_Set_Margin_Left\0"
  "Key_ISO_Set_Margin_Right\0Key_ISO_Release_Margin_Left\0"
  "Key_ISO_Release_Margin_Right\0Key_ISO_Release_Both_Margins\0"
  "Key_ISO_Fast_Cursor_Left\0Key_ISO_Fast_Cursor_Right\0"
  "Key_ISO_Fast_Cursor_Up\0Key_ISO_Fast_Cursor_Down\0"
  "Key_ISO_Continuous_Underline\0Key_ISO_Discontinuous_Underline\0"
  "Key_ISO_Emphasize\0Key_ISO_Center_Object\0Key_ISO_Enter\0Key_dead_grave\0"
  "Key_dead_acute\0Key_dead_circumflex\0Key_dead_tilde\0Key_dead_macron\0"
  "Key_dead_breve\0Key_dead_abovedot\0Key_dead_diaeresis\0Key_dead_abovering\0"
  "Key_dead_doubleacute\0Key_dead_caron\0Key_dead_cedilla\0Key_dead_ogonek\0"
  "Key_dead_iota\0Key_dead_voiced_sound\0Key_dead_semivoiced_sound\0"
  "Key_dead_belowdot\0Key_dead_hook\0Key_dead_horn\0Key_First_Virtual_Screen\0"
  "Key_Prev_Virtual_Screen\0Key_Next_Virtual_Screen\0Key_Last_Virtual_Screen\0"
  "Key_Terminate_Server\0Key_AccessX_Enable\0Key_AccessX_Feedback_Enable\0"
  "Key_RepeatKeys_Enable\0Key_SlowKeys_Enable\0Key_BounceKeys_Enable\0"
  "Key_StickyKeys_Enable\0Key_MouseKeys_Enable\0Key_MouseKeys_Accel_Enable\0"
  "Key_Overlay1_Enable\0Key_Overlay2_Enable\0Key_AudibleBell_Enable\0"
  "Key_Pointer_Left\0Key_Pointer_Right\0Key_Pointer_Up\0Key_Pointer_Down\0"
  "Key_Pointer_UpLeft\0Key_Pointer_UpRight\0Key_Pointer_DownLeft\0"
  "Key_Pointer_DownRight\0Key_Pointer_Button_Dflt\0Key_Pointer_Button1\0"
  "Key_Pointer_Button2\0Key_Pointer_Button3\0Key_Pointer_Button4\0"
  "Key_Pointer_Button5\0Key_Pointer_DblClick_Dflt\0Key_Pointer_DblClick1\0"
  "Key_Pointer_DblClick2\0Key_Pointer_DblClick3\0Key_Pointer_DblClick4\0"
  "Key_Pointer_DblClick5\0Key_Pointer_Drag_Dflt\0Key_Pointer_Drag1\0"
  "Key_Pointer_Drag2\0Key_Pointer_Drag3\0Key_Pointer_Drag4\0"
  "Key_Pointer_Drag5\0Key_Pointer_EnableKeys\0Key_Pointer_Accelerate\0"
  "Key_Pointer_DfltBtnNext\0Key_Pointer_DfltBtnPrev\0Key_3270_Duplicate\0"
  "Key_3270_FieldMark\0Key_3270_Right2\0Key_3270_Left2\0Key_3270_BackTab\0"
  "Key_3270_EraseEOF\0Key_3270_EraseInput\0Key_3270_Reset\0Key_3270_Quit\0"
  "Key_3270_PA1\0Key_3270_PA2\0Key_3270_PA3\0Key_3270_Test\0Key_3270_Attn\0"
  "Key_3270_CursorBlink\0Key_3270_AltCursor\0Key_3270_KeyClick\0"
  "Key_3270_Jump\0Key_3270_Ident\0Key_3270_Rule\0Key_3270_Copy\0"
  "Key_3270_Play\0Key_3270_Setup\0Key_3270_Record\0Key_3270_ChangeScreen\0"
  "Key_3270_DeleteWord\0Key_3270_ExSelect\0Key_3270_CursorSelect\0"
  "Key_3270_PrintScreen\0Key_3270_Enter\0Key_space\0Key_exclam\0Key_quotedbl\0"
  "Key_numbersign\0Key_dollar\0Key_percent\0Key_ampersand\0Key_apostrophe\0"
  "Key_quoteright\0Key_parenleft\0Key_parenright\0Key_asterisk\0Key_plus\0"
  "Key_comma\0Key_minus\0Key_period\0Key_slash\0Key_0\0Key_1\0Key_2\0Key_3\0"
  "Key_4\0Key_5\0Key_6\0Key_7\0Key_8\0Key_9\0Key_colon\0Key_semicolon\0"
  "Key_less\0Key_equal\0Key_greater\0Key_question\0Key_at\0Key_A\0Key_B\0"
  "Key_C\0Key_D\0Key_E\0Key_F\0Key_G\0Key_H\0Key_I\0Key_J\0Key_K\0Key_L\0"
  "Key_M\0Key_N\0Key_O\0Key_P\0Key_Q\0Key_R\0Key_S\0Key_T\0Key_U\0Key_V\0"
  "Key_W\0Key_X\0Key_Y\0Key_Z\0Key_bracketleft\0Key_backslash\0"
  "Key_bracketright\0Key_asciicircum\0Key_underscore\0Key_grave\0"
  "Key_quoteleft\0Key_a\0Key_b\0Key_c\0Key_d\0Key_e\0Key_f\0Key_g\0Key_h\0"
  "Key_i\0Key_j\0Key_k\0Key_l\0Key_m\0Key_n\0Key_o\0Key_p\0Key_q\0Key_r\0"
  "Key_s\0Key_t\0Key_u\0Key_v\0Key_w\0Key_x\0Key_y\0Key_z\0Key_braceleft\0"
  "Key_bar\0Key_braceright\0Key_asciitilde\0Key_nobreakspace\0Key_exclamdown\0"
  "Key_cent\0Key_sterling\0Key_currency\0Key_yen\0Key_brokenbar\0Key_section\0"
  "Key_diaeresis\0Key_copyright\0Key_ordfeminine\0Key_guillemotleft\0"
  "Key_notsign\0Key_hyphen\0Key_registered\0Key_macron\0Key_degree\0"
  "Key_plusminus\0Key_twosuperior\0Key_threesuperior\0Key_acute\0Key_mu\0"
  "Key_paragraph\0Key_periodcentered\0Key_cedilla\0Key_onesuperior\0"
  "Key_masculine\0Key_guillemotright\0Key_onequarter\0Key_onehalf\0"
  "Key_threequarters\0Key_questiondown\0Key_Agrave\0Key_Aacute\0"
  "Key_Acircumflex\0Key_Atilde\0Key_Adiaeresis\0Key_Aring\0Key_AE\0"
  "Key_Ccedilla\0Key_Egrave\0Key_Eacute\0Key_Ecircumflex\0Key_Ediaeresis\0"
  "Key_Igrave\0Key_Iacute\0Key_Icircumflex\0Key_Idiaeresis\0Key_ETH\0Key_Eth\0"
  "Key_Ntilde\0Key_Ograve\0Key_Oacute\0Key_Ocircumflex\0Key_Otilde\0"
  "Key_Odiaeresis\0Key_multiply\0Key_Ooblique\0Key_Ugrave\0Key_Uacute\0"
  "Key_Ucircumflex\0Key_Udiaeresis\0Key_Yacute\0Key_THORN\0Key_Thorn\0"
  "Key_ssharp\0Key_agrave\0Key_aacute\0Key_acircumflex\0Key_atilde\0"
  "Key_adiaeresis\0Key_aring\0Key_ae\0Key_ccedilla\0Key_egrave\0Key_eacute\0"
  "Key_ecircumflex\0Key_ediaeresis\0Key_igrave\0Key_iacute\0Key_icircumflex\0"
  "Key_idiaeresis\0Key_eth\0Key_ntilde\0Key_ograve\0Key_oacute\0"
  "Key_ocircumflex\0Key_otilde\0Key_odiaeresis\0Key_division\0Key_oslash\0"
  "Key_ugrave\0Key_uacute\0Key_ucircumflex\0Key_udiaeresis\0Key_yacute\0"
  "Key_thorn\0Key_ydiaeresis\0Key_Aogonek\0Key_breve\0Key_Lstroke\0"
  "Key_Lcaron\0Key_Sacute\0Key_Scaron\0Key_Scedilla\0Key_Tcaron\0Key_Zacute\0"
  "Key_Zcaron\0Key_Zabovedot\0Key_aogonek\0Key_ogonek\0Key_lstroke\0"
  "Key_lcaron\0Key_sacute\0Key_caron\0Key_scaron\0Key_scedilla\0Key_tcaron\0"
  "Key_zacute\0Key_doubleacute\0Key_zcaron\0Key_zabovedot\0Key_Racute\0"
  "Key_Abreve\0Key_Lacute\0Key_Cacute\0Key_Ccaron\0Key_Eogonek\0Key_Ecaron\0"
  "Key_Dcaron\0Key_Dstroke\0Key_Nacute\0Key_Ncaron\0Key_Odoubleacute\0"
  "Key_Rcaron\0Key_Uring\0Key_Udoubleacute\0Key_Tcedilla\0Key_racute\0"
  "Key_abreve\0Key_lacute\0Key_cacute\0Key_ccaron\0Key_eogonek\0Key_ecaron\0"
  "Key_dcaron\0Key_dstroke\0Key_nacute\0Key_ncaron\0Key_odoubleacute\0"
  "Key_udoubleacute\0Key_rcaron\0Key_uring\0Key_tcedilla\0Key_abovedot\0"
  "Key_Hstroke\0Key_Hcircumflex\0Key_Iabovedot\0Key_Gbreve\0Key_Jcircumflex\0"
  "Key_hstroke\0Key_hcircumflex\0Key_idotless\0Key_gbreve\0Key_jcircumflex\0"
  "Key_Cabovedot\0Key_Ccircumflex\0Key_Gabovedot\0Key_Gcircumflex\0"
  "Key_Ubreve\0Key_Scircumflex\0Key_cabovedot\0Key_ccircumflex\0"
  "Key_gabovedot\0Key_gcircumflex\0Key_ubreve\0Key_scircumflex\0Key_kra\0"
  "Key_kappa\0Key_Rcedilla\0Key_Itilde\0Key_Lcedilla\0Key_Emacron\0"
  "Key_Gcedilla\0Key_Tslash\0Key_rcedilla\0Key_itilde\0Key_lcedilla\0"
  "Key_emacron\0Key_gcedilla\0Key_tslash\0Key_ENG\0Key_eng\0Key_Amacron\0"
  "Key_Iogonek\0Key_Eabovedot\0Key_Imacron\0Key_Ncedilla\0Key_Omacron\0"
  "Key_Kcedilla\0Key_Uogonek\0Key_Utilde\0Key_Umacron\0Key_amacron\0"
  "Key_iogonek\0Key_eabovedot\0Key_imacron\0Key_ncedilla\0Key_omacron\0"
  "Key_kcedilla\0Key_uogonek\0Key_utilde\0Key_umacron\0Key_OE\0Key_oe\0"
  "Key_Ydiaeresis\0Key_overline\0Key_kana_fullstop\0Key_kana_openingbracket\0"
  "Key_kana_closingbracket\0Key_kana_comma\0Key_kana_conjunctive\0"
  "Key_kana_middledot\0Key_kana_WO\0Key_kana_a\0Key_kana_i\0Key_kana_u\0"
  "Key_kana_e\0Key_kana_o\0Key_kana_ya\0Key_kana_yu\0Key_kana_yo\0"
  "Key_kana_tsu\0Key_kana_tu\0Key_prolongedsound\0Key_kana_A\0Key_kana_I\0"
  "Key_kana_U\0Key_kana_E\0Key_kana_O\0Key_kana_KA\0Key_kana_KI\0Key_kana_KU\0"
  "Key_kana_KE\0Key_kana_KO\0Key_kana_SA\0Key_kana_SHI\0Key_kana_SU\0"
  "Key_kana_SE\0Key_kana_SO\0Key_kana_TA\0Key_kana_CHI\0Key_kana_TI\0"
  "Key_kana_TSU\0Key_kana_TU\0Key_kana_TE\0Key_kana_TO\0Key_kana_NA\0"
  "Key_kana_NI\0Key_kana_NU\0Key_kana_NE\0Key_kana_NO\0Key_kana_HA\0"
  "Key_kana_HI\0Key_kana_FU\0Key_kana_HU\0Key_kana_HE\0Key_kana_HO\0"
  "Key_kana_MA\0Key_kana_MI\0Key_kana_MU\0Key_kana_ME\0Key_kana_MO\0"
  "Key_kana_YA\0Key_kana_YU\0Key_kana_YO\0Key_kana_RA\0Key_kana_RI\0"
  "Key_kana_RU\0Key_kana_RE\0Key_kana_RO\0Key_kana_WA\0Key_kana_N\0"
  "Key_voicedsound\0Key_semivoicedsound\0Key_kana_switch\0Key_Arabic_comma\0"
  "Key_Arabic_semicolon\0Key_Arabic_question_mark\0Key_Arabic_hamza\0"
  "Key_Arabic_maddaonalef\0Key_Arabic_hamzaonalef\0Key_Arabic_hamzaonwaw\0"
  "Key_Arabic_hamzaunderalef\0Key_Arabic_hamzaonyeh\0Key_Arabic_alef\0"
  "Key_Arabic_beh\0Key_Arabic_tehmarbuta\0Key_Arabic_teh\0Key_Arabic_theh\0"
  "Key_Arabic_jeem\0Key_Arabic_hah\0Key_Arabic_khah\0Key_Arabic_dal\0"
  "Key_Arabic_thal\0Key_Arabic_ra\0Key_Arabic_zain\0Key_Arabic_seen\0"
  "Key_Arabic_sheen\0Key_Arabic_sad\0Key_Arabic_dad\0Key_Arabic_tah\0"
  "Key_Arabic_zah\0Key_Arabic_ain\0Key_Arabic_ghain\0Key_Arabic_tatweel\0"
  "Key_Arabic_feh\0Key_Arabic_qaf\0Key_Arabic_kaf\0Key_Arabic_lam\0"
  "Key_Arabic_meem\0Key_Arabic_noon\0Key_Arabic_ha\0Key_Arabic_heh\0"
  "Key_Arabic_waw\0Key_Arabic_alefmaksura\0Key_Arabic_yeh\0"
  "Key_Arabic_fathatan\0Key_Arabic_dammatan\0Key_Arabic_kasratan\0"
  "Key_Arabic_fatha\0Key_Arabic_damma\0Key_Arabic_kasra\0Key_Arabic_shadda\0"
  "Key_Arabic_sukun\0Key_Arabic_switch\0Key_Serbian_dje\0Key_Macedonia_gje\0"
  "Key_Cyrillic_io\0Key_Ukrainian_ie\0Key_Ukranian_je\0Key_Macedonia_dse\0"
  "Key_Ukrainian_i\0Key_Ukranian_i\0Key_Ukrainian_yi\0Key_Ukranian_yi\0"
  "Key_Cyrillic_je\0Key_Serbian_je\0Key_Cyrillic_lje\0Key_Serbian_lje\0"
  "Key_Cyrillic_nje\0Key_Serbian_nje\0Key_Serbian_tshe\0Key_Macedonia_kje\0"
  "Key_Ukrainian_ghe_with_upturn\0Key_Byelorussian_shortu\0Key_Cyrillic_dzhe\0"
  "Key_Serbian_dze\0Key_numerosign\0Key_Serbian_DJE\0Key_Macedonia_GJE\0"
  "Key_Cyrillic_IO\0Key_Ukrainian_IE\0Key_Ukranian_JE\0Key_Macedonia_DSE\0"
  "Key_Ukrainian_I\0Key_Ukranian_I\0Key_Ukrainian_YI\0Key_Ukranian_YI\0"
  "Key_Cyrillic_JE\0Key_Serbian_JE\0Key_Cyrillic_LJE\0Key_Serbian_LJE\0"
  "Key_Cyrillic_NJE\0Key_Serbian_NJE\0Key_Serbian_TSHE\0Key_Macedonia_KJE\0"
  "Key_Ukrainian_GHE_WITH_UPTURN\0Key_Byelorussian_SHORTU\0Key_Cyrillic_DZHE\0"
  "Key_Serbian_DZE\0Key_Cyrillic_yu\0Key_Cyrillic_a\0Key_Cyrillic_be\0"
  "Key_Cyrillic_tse\0Key_Cyrillic_de\0Key_Cyrillic_ie\0Key_Cyrillic_ef\0"
  "Key_Cyrillic_ghe\0Key_Cyrillic_ha\0Key_Cyrillic_i\0Key_Cyrillic_shorti\0"
  "Key_Cyrillic_ka\0Key_Cyrillic_el\0Key_Cyrillic_em\0Key_Cyrillic_en\0"
  "Key_Cyrillic_o\0Key_Cyrillic_pe\0Key_Cyrillic_ya\0Key_Cyrillic_er\0"
  "Key_Cyrillic_es\0Key_Cyrillic_te\0Key_Cyrillic_u\0Key_Cyrillic_zhe\0"
  "Key_Cyrillic_ve\0Key_Cyrillic_softsign\0Key_Cyrillic_yeru\0"
  "Key_Cyrillic_ze\0Key_Cyrillic_sha\0Key_Cyrillic_e\0Key_Cyrillic_shcha\0"
  "Key_Cyrillic_che\0Key_Cyrillic_hardsign\0Key_Cyrillic_YU\0Key_Cyrillic_A\0"
  "Key_Cyrillic_BE\0Key_Cyrillic_TSE\0Key_Cyrillic_DE\0Key_Cyrillic_IE\0"
  "Key_Cyrillic_EF\0Key_Cyrillic_GHE\0Key_Cyrillic_HA\0Key_Cyrillic_I\0"
  "Key_Cyrillic_SHORTI\0Key_Cyrillic_KA\0Key_Cyrillic_EL\0Key_Cyrillic_EM\0"
  "Key_Cyrillic_EN\0Key_Cyrillic_O\0Key_Cyrillic_PE\0Key_Cyrillic_YA\0"
  "Key_Cyrillic_ER\0Key_Cyrillic_ES\0Key_Cyrillic_TE\0Key_Cyrillic_U\0"
  "Key_Cyrillic_ZHE\0Key_Cyrillic_VE\0Key_Cyrillic_SOFTSIGN\0"
  "Key_Cyrillic_YERU\0Key_Cyrillic_ZE\0Key_Cyrillic_SHA\0Key_Cyrillic_E\0"
  "Key_Cyrillic_SHCHA\0Key_Cyrillic_CHE\0Key_Cyrillic_HARDSIGN\0"
  "Key_Greek_ALPHAaccent\0Key_Greek_EPSILONaccent\0Key_Greek_ETAaccent\0"
  "Key_Greek_IOTAaccent\0Key_Greek_IOTAdieresis\0Key_Greek_OMICRONaccent\0"
  "Key_Greek_UPSILONaccent\0Key_Greek_UPSILONdieresis\0Key_Greek_OMEGAaccent\0"
  "Key_Greek_accentdieresis\0Key_Greek_horizbar\0Key_Greek_alphaaccent\0"
  "Key_Greek_epsilonaccent\0Key_Greek_etaaccent\0Key_Greek_iotaaccent\0"
  "Key_Greek_iotadieresis\0Key_Greek_iotaaccentdieresis\0"
  "Key_Greek_omicronaccent\0Key_Greek_upsilonaccent\0"
  "Key_Greek_upsilondieresis\0Key_Greek_upsilonaccentdieresis\0"
  "Key_Greek_omegaaccent\0Key_Greek_ALPHA\0Key_Greek_BETA\0Key_Greek_GAMMA\0"
  "Key_Greek_DELTA\0Key_Greek_EPSILON\0Key_Greek_ZETA\0Key_Greek_ETA\0"
  "Key_Greek_THETA\0Key_Greek_IOTA\0Key_Greek_KAPPA\0Key_Greek_LAMDA\0"
  "Key_Greek_LAMBDA\0Key_Greek_MU\0Key_Greek_NU\0Key_Greek_XI\0"
  "Key_Greek_OMICRON\0Key_Greek_PI\0Key_Greek_RHO\0Key_Greek_SIGMA\0"
  "Key_Greek_TAU\0Key_Greek_UPSILON\0Key_Greek_PHI\0Key_Greek_CHI\0"
  "Key_Greek_PSI\0Key_Greek_OMEGA\0Key_Greek_alpha\0Key_Greek_beta\0"
  "Key_Greek_gamma\0Key_Greek_delta\0Key_Greek_epsilon\0Key_Greek_zeta\0"
  "Key_Greek_eta\0Key_Greek_theta\0Key_Greek_iota\0Key_Greek_kappa\0"
  "Key_Greek_lamda\0Key_Greek_lambda\0Key_Greek_mu\0Key_Greek_nu\0"
  "Key_Greek_xi\0Key_Greek_omicron\0Key_Greek_pi\0Key_Greek_rho\0"
  "Key_Greek_sigma\0Key_Greek_finalsmallsigma\0Key_Greek_tau\0"
  "Key_Greek_upsilon\0Key_Greek_phi\0Key_Greek_chi\0Key_Greek_psi\0"
  "Key_Greek_omega\0Key_Greek_switch\0Key_leftradical\0Key_topleftradical\0"
  "Key_horizconnector\0Key_topintegral\0Key_botintegral\0Key_vertconnector\0"
  "Key_topleftsqbracket\0Key_botleftsqbracket\0Key_toprightsqbracket\0"
  "Key_botrightsqbracket\0Key_topleftparens\0Key_botleftparens\0"
  "Key_toprightparens\0Key_botrightparens\0Key_leftmiddlecurlybrace\0"
  "Key_rightmiddlecurlybrace\0Key_topleftsummation\0Key_botleftsummation\0"
  "Key_topvertsummationconnector\0Key_botvertsummationconnector\0"
  "Key_toprightsummation\0Key_botrightsummation\0Key_rightmiddlesummation\0"
  "Key_lessthanequal\0Key_notequal\0Key_greaterthanequal\0Key_integral\0"
  "Key_therefore\0Key_variation\0Key_infinity\0Key_nabla\0Key_approximate\0"
  "Key_similarequal\0Key_ifonlyif\0Key_implies\0Key_identical\0Key_radical\0"
  "Key_includedin\0Key_includes\0Key_intersection\0Key_union\0Key_logicaland\0"
  "Key_logicalor\0Key_partialderivative\0Key_function\0Key_leftarrow\0"
  "Key_uparrow\0Key_rightarrow\0Key_downarrow\0Key_blank\0Key_soliddiamond\0"
  "Key_checkerboard\0Key_ht\0Key_ff\0Key_cr\0Key_lf\0Key_nl\0Key_vt\0"
  "Key_lowrightcorner\0Key_uprightcorner\0Key_upleftcorner\0"
  "Key_lowleftcorner\0Key_crossinglines\0Key_horizlinescan1\0"
  "Key_horizlinescan3\0Key_horizlinescan5\0Key_horizlinescan7\0"
  "Key_horizlinescan9\0Key_leftt\0Key_rightt\0Key_bott\0Key_topt\0"
  "Key_vertbar\0Key_emspace\0Key_enspace\0Key_em3space\0Key_em4space\0"
  "Key_digitspace\0Key_punctspace\0Key_thinspace\0Key_hairspace\0Key_emdash\0"
  "Key_endash\0Key_signifblank\0Key_ellipsis\0Key_doubbaselinedot\0"
  "Key_onethird\0Key_twothirds\0Key_onefifth\0Key_twofifths\0Key_threefifths\0"
  "Key_fourfifths\0Key_onesixth\0Key_fivesixths\0Key_careof\0Key_figdash\0"
  "Key_leftanglebracket\0Key_decimalpoint\0Key_rightanglebracket\0Key_marker\0"
  "Key_oneeighth\0Key_threeeighths\0Key_fiveeighths\0Key_seveneighths\0"
  "Key_trademark\0Key_signaturemark\0Key_trademarkincircle\0"
  "Key_leftopentriangle\0Key_rightopentriangle\0Key_emopencircle\0"
  "Key_emopenrectangle\0Key_leftsinglequotemark\0Key_rightsinglequotemark\0"
  "Key_leftdoublequotemark\0Key_rightdoublequotemark\0Key_prescription\0"
  "Key_minutes\0Key_seconds\0Key_latincross\0Key_hexagram\0"
  "Key_filledrectbullet\0Key_filledlefttribullet\0Key_filledrighttribullet\0"
  "Key_emfilledcircle\0Key_emfilledrect\0Key_enopencircbullet\0"
  "Key_enopensquarebullet\0Key_openrectbullet\0Key_opentribulletup\0"
  "Key_opentribulletdown\0Key_openstar\0Key_enfilledcircbullet\0"
  "Key_enfilledsqbullet\0Key_filledtribulletup\0Key_filledtribulletdown\0"
  "Key_leftpointer\0Key_rightpointer\0Key_club\0Key_diamond\0Key_heart\0"
  "Key_maltesecross\0Key_dagger\0Key_doubledagger\0Key_checkmark\0"
  "Key_ballotcross\0Key_musicalsharp\0Key_musicalflat\0Key_malesymbol\0"
  "Key_femalesymbol\0Key_telephone\0Key_telephonerecorder\0"
  "Key_phonographcopyright\0Key_caret\0Key_singlelowquotemark\0"
  "Key_doublelowquotemark\0Key_cursor\0Key_leftcaret\0Key_rightcaret\0"
  "Key_downcaret\0Key_upcaret\0Key_overbar\0Key_downtack\0Key_upshoe\0"
  "Key_downstile\0Key_underbar\0Key_jot\0Key_quad\0Key_uptack\0Key_circle\0"
  "Key_upstile\0Key_downshoe\0Key_rightshoe\0Key_leftshoe\0Key_lefttack\0"
  "Key_righttack\0Key_hebrew_doublelowline\0Key_hebrew_aleph\0Key_hebrew_bet\0"
  "Key_hebrew_beth\0Key_hebrew_gimel\0Key_hebrew_gimmel\0Key_hebrew_dalet\0"
  "Key_hebrew_daleth\0Key_hebrew_he\0Key_hebrew_waw\0Key_hebrew_zain\0"
  "Key_hebrew_zayin\0Key_hebrew_chet\0Key_hebrew_het\0Key_hebrew_tet\0"
  "Key_hebrew_teth\0Key_hebrew_yod\0Key_hebrew_finalkaph\0Key_hebrew_kaph\0"
  "Key_hebrew_lamed\0Key_hebrew_finalmem\0Key_hebrew_mem\0"
  "Key_hebrew_finalnun\0Key_hebrew_nun\0Key_hebrew_samech\0Key_hebrew_samekh\0"
  "Key_hebrew_ayin\0Key_hebrew_finalpe\0Key_hebrew_pe\0Key_hebrew_finalzade\0"
  "Key_hebrew_finalzadi\0Key_hebrew_zade\0Key_hebrew_zadi\0Key_hebrew_qoph\0"
  "Key_hebrew_kuf\0Key_hebrew_resh\0Key_hebrew_shin\0Key_hebrew_taw\0"
  "Key_hebrew_taf\0Key_Hebrew_switch\0Key_Thai_kokai\0Key_Thai_khokhai\0"
  "Key_Thai_khokhuat\0Key_Thai_khokhwai\0Key_Thai_khokhon\0"
  "Key_Thai_khorakhang\0Key_Thai_ngongu\0Key_Thai_chochan\0Key_Thai_choching\0"
  "Key_Thai_chochang\0Key_Thai_soso\0Key_Thai_chochoe\0Key_Thai_yoying\0"
  "Key_Thai_dochada\0Key_Thai_topatak\0Key_Thai_thothan\0"
  "Key_Thai_thonangmontho\0Key_Thai_thophuthao\0Key_Thai_nonen\0"
  "Key_Thai_dodek\0Key_Thai_totao\0Key_Thai_thothung\0Key_Thai_thothahan\0"
  "Key_Thai_thothong\0Key_Thai_nonu\0Key_Thai_bobaimai\0Key_Thai_popla\0"
  "Key_Thai_phophung\0Key_Thai_fofa\0Key_Thai_phophan\0Key_Thai_fofan\0"
  "Key_Thai_phosamphao\0Key_Thai_moma\0Key_Thai_yoyak\0Key_Thai_rorua\0"
  "Key_Thai_ru\0Key_Thai_loling\0Key_Thai_lu\0Key_Thai_wowaen\0"
  "Key_Thai_sosala\0Key_Thai_sorusi\0Key_Thai_sosua\0Key_Thai_hohip\0"
  "Key_Thai_lochula\0Key_Thai_oang\0Key_Thai_honokhuk\0Key_Thai_paiyannoi\0"
  "Key_Thai_saraa\0Key_Thai_maihanakat\0Key_Thai_saraaa\0Key_Thai_saraam\0"
  "Key_Thai_sarai\0Key_Thai_saraii\0Key_Thai_saraue\0Key_Thai_sarauee\0"
  "Key_Thai_sarau\0Key_Thai_sarauu\0Key_Thai_phinthu\0"
  "Key_Thai_maihanakat_maitho\0Key_Thai_baht\0Key_Thai_sarae\0"
  "Key_Thai_saraae\0Key_Thai_sarao\0Key_Thai_saraaimaimuan\0"
  "Key_Thai_saraaimaimalai\0Key_Thai_lakkhangyao\0Key_Thai_maiyamok\0"
  "Key_Thai_maitaikhu\0Key_Thai_maiek\0Key_Thai_maitho\0Key_Thai_maitri\0"
  "Key_Thai_maichattawa\0Key_Thai_thanthakhat\0Key_Thai_nikhahit\0"
  "Key_Thai_leksun\0Key_Thai_leknung\0Key_Thai_leksong\0Key_Thai_leksam\0"
  "Key_Thai_leksi\0Key_Thai_lekha\0Key_Thai_lekhok\0Key_Thai_lekchet\0"
  "Key_Thai_lekpaet\0Key_Thai_lekkao\0Key_Hangul\0Key_Hangul_Start\0"
  "Key_Hangul_End\0Key_Hangul_Hanja\0Key_Hangul_Jamo\0Key_Hangul_Romaja\0"
  "Key_Hangul_Codeinput\0Key_Hangul_Jeonja\0Key_Hangul_Banja\0"
  "Key_Hangul_PreHanja\0Key_Hangul_PostHanja\0Key_Hangul_SingleCandidate\0"
  "Key_Hangul_MultipleCandidate\0Key_Hangul_PreviousCandidate\0"
  "Key_Hangul_Special\0Key_Hangul_switch\0Key_Hangul_Kiyeog\0"
  "Key_Hangul_SsangKiyeog\0Key_Hangul_KiyeogSios\0Key_Hangul_Nieun\0"
  "Key_Hangul_NieunJieuj\0Key_Hangul_NieunHieuh\0Key_Hangul_Dikeud\0"
  "Key_Hangul_SsangDikeud\0Key_Hangul_Rieul\0Key_Hangul_RieulKiyeog\0"
  "Key_Hangul_RieulMieum\0Key_Hangul_RieulPieub\0Key_Hangul_RieulSios\0"
  "Key_Hangul_RieulTieut\0Key_Hangul_RieulPhieuf\0Key_Hangul_RieulHieuh\0"
  "Key_Hangul_Mieum\0Key_Hangul_Pieub\0Key_Hangul_SsangPieub\0"
  "Key_Hangul_PieubSios\0Key_Hangul_Sios\0Key_Hangul_SsangSios\0"
  "Key_Hangul_Ieung\0Key_Hangul_Jieuj\0Key_Hangul_SsangJieuj\0"
  "Key_Hangul_Cieuc\0Key_Hangul_Khieuq\0Key_Hangul_Tieut\0Key_Hangul_Phieuf\0"
  "Key_Hangul_Hieuh\0Key_Hangul_A\0Key_Hangul_AE\0Key_Hangul_YA\0"
  "Key_Hangul_YAE\0Key_Hangul_EO\0Key_Hangul_E\0Key_Hangul_YEO\0"
  "Key_Hangul_YE\0Key_Hangul_O\0Key_Hangul_WA\0Key_Hangul_WAE\0Key_Hangul_OE\0"
  "Key_Hangul_YO\0Key_Hangul_U\0Key_Hangul_WEO\0Key_Hangul_WE\0Key_Hangul_WI\0"
  "Key_Hangul_YU\0Key_Hangul_EU\0Key_Hangul_YI\0Key_Hangul_I\0"
  "Key_Hangul_J_Kiyeog\0Key_Hangul_J_SsangKiyeog\0Key_Hangul_J_KiyeogSios\0"
  "Key_Hangul_J_Nieun\0Key_Hangul_J_NieunJieuj\0Key_Hangul_J_NieunHieuh\0"
  "Key_Hangul_J_Dikeud\0Key_Hangul_J_Rieul\0Key_Hangul_J_RieulKiyeog\0"
  "Key_Hangul_J_RieulMieum\0Key_Hangul_J_RieulPieub\0Key_Hangul_J_RieulSios\0"
  "Key_Hangul_J_RieulTieut\0Key_Hangul_J_RieulPhieuf\0"
  "Key_Hangul_J_RieulHieuh\0Key_Hangul_J_Mieum\0Key_Hangul_J_Pieub\0"
  "Key_Hangul_J_PieubSios\0Key_Hangul_J_Sios\0Key_Hangul_J_SsangSios\0"
  "Key_Hangul_J_Ieung\0Key_Hangul_J_Jieuj\0Key_Hangul_J_Cieuc\0"
  "Key_Hangul_J_Khieuq\0Key_Hangul_J_Tieut\0Key_Hangul_J_Phieuf\0"
  "Key_Hangul_J_Hieuh\0Key_Hangul_RieulYeorinHieuh\0"
  "Key_Hangul_SunkyeongeumMieum\0Key_Hangul_SunkyeongeumPieub\0"
  "Key_Hangul_PanSios\0Key_Hangul_KkogjiDalrinIeung\0"
  "Key_Hangul_SunkyeongeumPhieuf\0Key_Hangul_YeorinHieuh\0Key_Hangul_AraeA\0"
  "Key_Hangul_AraeAE\0Key_Hangul_J_PanSios\0Key_Hangul_J_KkogjiDalrinIeung\0"
  "Key_Hangul_J_YeorinHieuh\0Key_Korean_Won\0Key_EcuSign\0Key_ColonSign\0"
  "Key_CruzeiroSign\0Key_FFrancSign\0Key_LiraSign\0Key_MillSign\0"
  "Key_NairaSign\0Key_PesetaSign\0Key_RupeeSign\0Key_WonSign\0"
  "Key_NewSheqelSign\0Key_DongSign\0Key_EuroSign\0";

static const VALUE rbclt_keysyms_values[] =
  {
    INT2FIX (CLUTTER_VoidSymbol), INT2FIX (CLUTTER_BackSpace), 
    INT2FIX (CLUTTER_Tab), INT2FIX (CLUTTER_Linefeed), INT2FIX (CLUTTER_Clear), 
    INT2FIX (CLUTTER_Return), INT2FIX (CLUTTER_Pause), 
    INT2FIX (CLUTTER_Scroll_Lock), INT2FIX (CLUTTER_Sys_Req), 
    INT2FIX (CLUTTER_Escape), INT2FIX (CLUTTER_Delete), 
    INT2FIX (CLUTTER_Multi_key), INT2FIX (CLUTTER_Codeinput), 
    INT2FIX (CLUTTER_SingleCandidate), INT2FIX (CLUTTER_MultipleCandidate), 
    INT2FIX (CLUTTER_PreviousCandidate), INT2FIX (CLUTTER_Kanji), 
    INT2FIX (CLUTTER_Muhenkan), INT2FIX (CLUTTER_Henkan_Mode), 
    INT2FIX (CLUTTER_Henkan), INT2FIX (CLUTTER_Romaji), 
    INT2FIX (CLUTTER_Hiragana), INT2FIX (CLUTTER_Katakana), 
    INT2FIX (CLUTTER_Hiragana_Katakana), INT2FIX (CLUTTER_Zenkaku), 
    INT2FIX (CLUTTER_Hankaku), INT2FIX (CLUTTER_Zenkaku_Hankaku), 
    INT2FIX (CLUTTER_Touroku), INT2FIX (CLUTTER_Massyo), 
    INT2FIX (CLUTTER_Kana_Lock), INT2FIX (CLUTTER_Kana_Shift), 
    INT2FIX (CLUTTER_Eisu_Shift), INT2FIX (CLUTTER_Eisu_toggle), 
    INT2FIX (CLUTTER_Kanji_Bangou), INT2FIX (CLUTTER_Zen_Koho), 
    INT2FIX (CLUTTER_Mae_Koho), INT2FIX (CLUTTER_Home), INT2FIX (CLUTTER_Left), 
    INT2FIX (CLUTTER_Up), INT2FIX (CLUTTER_Right), INT2FIX (CLUTTER_Down), 
    INT2FIX (CLUTTER_Prior), INT2FIX (CLUTTER_Page_Up), INT2FIX (CLUTTER_Next), 
    INT2FIX (CLUTTER_Page_Down), INT2FIX (CLUTTER_End), 
    INT2FIX (CLUTTER_Begin), INT2FIX (CLUTTER_Select), INT2FIX (CLUTTER_Print), 
    INT2FIX (CLUTTER_Execute), INT2FIX (CLUTTER_Insert), 
    INT2FIX (CLUTTER_Undo), INT2FIX (CLUTTER_Redo), INT2FIX (CLUTTER_Menu), 
    INT2FIX (CLUTTER_Find), INT2FIX (CLUTTER_Cancel), INT2FIX (CLUTTER_Help), 
    INT2FIX (CLUTTER_Break), INT2FIX (CLUTTER_Mode_switch), 
    INT2FIX (CLUTTER_script_switch), INT2FIX (CLUTTER_Num_Lock), 
    INT2FIX (CLUTTER_KP_Space), INT2FIX (CLUTTER_KP_Tab), 
    INT2FIX (CLUTTER_KP_Enter), INT2FIX (CLUTTER_KP_F1), 
    INT2FIX (CLUTTER_KP_F2), INT2FIX (CLUTTER_KP_F3), INT2FIX (CLUTTER_KP_F4), 
    INT2FIX (CLUTTER_KP_Home), INT2FIX (CLUTTER_KP_Left), 
    INT2FIX (CLUTTER_KP_Up), INT2FIX (CLUTTER_KP_Right), 
    INT2FIX (CLUTTER_KP_Down), INT2FIX (CLUTTER_KP_Prior), 
    INT2FIX (CLUTTER_KP_Page_Up), INT2FIX (CLUTTER_KP_Next), 
    INT2FIX (CLUTTER_KP_Page_Down), INT2FIX (CLUTTER_KP_End), 
    INT2FIX (CLUTTER_KP_Begin), INT2FIX (CLUTTER_KP_Insert), 
    INT2FIX (CLUTTER_KP_Delete), INT2FIX (CLUTTER_KP_Equal), 
    INT2FIX (CLUTTER_KP_Multiply), INT2FIX (CLUTTER_KP_Add), 
    INT2FIX (CLUTTER_KP_Separator), INT2FIX (CLUTTER_KP_Subtract), 
    INT2FIX (CLUTTER_KP_Decimal), INT2FIX (CLUTTER_KP_Divide), 
    INT2FIX (CLUTTER_KP_0), INT2FIX (CLUTTER_KP_1), INT2FIX (CLUTTER_KP_2), 
    INT2FIX (CLUTTER_KP_3), INT2FIX (CLUTTER_KP_4), INT2FIX (CLUTTER_KP_5), 
    INT2FIX (CLUTTER_KP_6), INT2FIX (CLUTTER_KP_7), INT2FIX (CLUTTER_KP_8), 
    INT2FIX (CLUTTER_KP_9), INT2FIX (CLUTTER_F1), INT2FIX (CLUTTER_F2), 
    INT2FIX (CLUTTER_F3), INT2FIX (CLUTTER_F4), INT2FIX (CLUTTER_F5), 
    INT2FIX (CLUTTER_F6), INT2FIX (CLUTTER_F7), INT2FIX (CLUTTER_F8), 
    INT2FIX (CLUTTER_F9), INT2FIX (CLUTTER_F10), INT2FIX (CLUTTER_F11), 
    INT2FIX (CLUTTER_L1), INT2FIX (CLUTTER_F12), INT2FIX (CLUTTER_L2), 
    INT2FIX (CLUTTER_F13), INT2FIX (CLUTTER_L3), INT2FIX (CLUTTER_F14), 
    INT2FIX (CLUTTER_L4), INT2FIX (CLUTTER_F15), INT2FIX (CLUTTER_L5), 
    INT2FIX (CLUTTER_F16), INT2FIX (CLUTTER_L6), INT2FIX (CLUTTER_F17), 
    INT2FIX (CLUTTER_L7), INT2FIX (CLUTTER_F18), INT2FIX (CLUTTER_L8), 
    INT2FIX (CLUTTER_F19), INT2FIX (CLUTTER_L9), INT2FIX (CLUTTER_F20), 
    INT2FIX (CLUTTER_L10), INT2FIX (CLUTTER_F21), INT2FIX (CLUTTER_R1), 
    INT2FIX (CLUTTER_F22), INT2FIX (CLUTTER_R2), INT2FIX (CLUTTER_F23), 
    INT2FIX (CLUTTER_R3), INT2FIX (CLUTTER_F24), INT2FIX (CLUTTER_R4), 
    INT2FIX (CLUTTER_F25), INT2FIX (CLUTTER_R5), INT2FIX (CLUTTER_F26), 
    INT2FIX (CLUTTER_R6), INT2FIX (CLUTTER_F27), INT2FIX (CLUTTER_R7), 
    INT2FIX (CLUTTER_F28), INT2FIX (CLUTTER_R8), INT2FIX (CLUTTER_F29), 
    INT2FIX (CLUTTER_R9), INT2FIX (CLUTTER_F30), INT2FIX (CLUTTER_R10), 
    INT2FIX (CLUTTER_F31), INT2FIX (CLUTTER_R11), INT2FIX (CLUTTER_F32), 
    INT2FIX (CLUTTER_R12), INT2FIX (CLUTTER_F33), INT2FIX (CLUTTER_R13), 
    INT2FIX (CLUTTER_F34), INT2FIX (CLUTTER_R14), INT2FIX (CLUTTER_F35), 
    INT2FIX (CLUTTER_R15), INT2FIX (CLUTTER_Shift_L), 
    INT2FIX (CLUTTER_Shift_R), INT2FIX (CLUTTER_Control_L), 
    INT2FIX (CLUTTER_Control_R), INT2FIX (CLUTTER_Caps_Lock), 
    INT2FIX (CLUTTER_Shift_Lock), INT2FIX (CLUTTER_Meta_L), 
    INT2FIX (CLUTTER_Meta_R), INT2FIX (CLUTTER_Alt_L), INT2FIX (CLUTTER_Alt_R), 
    INT2FIX (CLUTTER_Super_L), INT2FIX (CLUTTER_Super_R), 
    INT2FIX (CLUTTER_Hyper_L), INT2FIX (CLUTTER_Hyper_R), 
    INT2FIX (CLUTTER_ISO_Lock), INT2FIX (CLUTTER_ISO_Level2_Latch), 
    INT2FIX (CLUTTER_ISO_Level3_Shift), INT2FIX (CLUTTER_ISO_Level3_Latch), 
    INT2FIX (CLUTTER_ISO_Level3_Lock), INT2FIX (CLUTTER_ISO_Group_Shift), 
    INT2FIX (CLUTTER_ISO_Group_Latch), INT2FIX (CLUTTER_ISO_Group_Lock), 
    INT2FIX (CLUTTER_ISO_Next_Group), INT2FIX (CLUTTER_ISO_Next_Group_Lock), 
    INT2FIX (CLUTTER_ISO_Prev_Group), INT2FIX (CLUTTER_ISO_Prev_Group_Lock), 
    INT2FIX (CLUTTER_ISO_First_Group), INT2FIX (CLUTTER_ISO_First_Group_Lock), 
    INT2FIX (CLUTTER_ISO_Last_Group), INT2FIX (CLUTTER_ISO_Last_Group_Lock), 
    INT2FIX (CLUTTER_ISO_Left_Tab), INT2FIX (CLUTTER_ISO_Move_Line_Up), 
    INT2FIX (CLUTTER_ISO_Move_Line_Down), 
    INT2FIX (CLUTTER_ISO_Partial_Line_Up), 
    INT2FIX (CLUTTER_ISO_Partial_Line_Down), 
    INT2FIX (CLUTTER_ISO_Partial_Space_Left), 
    INT2FIX (CLUTTER_ISO_Partial_Space_Right), 
    INT2FIX (CLUTTER_ISO_Set_Margin_Left), 
    INT2FIX (CLUTTER_ISO_Set_Margin_Right), 
    INT2FIX (CLUTTER_ISO_Release_Margin_Left), 
    INT2FIX (CLUTTER_ISO_Release_Margin_Right), 
    INT2FIX (CLUTTER_ISO_Release_Both_Margins), 
    INT2FIX (CLUTTER_ISO_Fast_Cursor_Left), 
    INT2FIX (CLUTTER_ISO_Fast_Cursor_Right), 
    INT2FIX (CLUTTER_ISO_Fast_Cursor_Up), 
    INT2FIX (CLUTTER_ISO_Fast_Cursor_Down), 
    INT2FIX (CLUTTER_ISO_Continuous_Underline), 
    INT2FIX (CLUTTER_ISO_Discontinuous_Underline), 
    INT2FIX (CLUTTER_ISO_Emphasize), INT2FIX (CLUTTER_ISO_Center_Object), 
    INT2FIX (CLUTTER_ISO_Enter), INT2FIX (CLUTTER_dead_grave), 
    INT2FIX (CLUTTER_dead_acute), INT2FIX (CLUTTER_dead_circumflex), 
    INT2FIX (CLUTTER_dead_tilde), INT2FIX (CLUTTER_dead_macron), 
    INT2FIX (CLUTTER_dead_breve), INT2FIX (CLUTTER_dead_abovedot), 
    INT2FIX (CLUTTER_dead_diaeresis), INT2FIX (CLUTTER_dead_abovering), 
    INT2FIX (CLUTTER_dead_doubleacute), INT2FIX (CLUTTER_dead_caron), 
    INT2FIX (CLUTTER_dead_cedilla), INT2FIX (CLUTTER_dead_ogonek), 
    INT2FIX (CLUTTER_dead_iota), INT2FIX (CLUTTER_dead_voiced_sound), 
    INT2FIX (CLUTTER_dead_semivoiced_sound), INT2FIX (CLUTTER_dead_belowdot), 
    INT2FIX (CLUTTER_dead_hook), INT2FIX (CLUTTER_dead_horn), 
    INT2FIX (CLUTTER_First_Virtual_Screen), 
    INT2FIX (CLUTTER_Prev_Virtual_Screen), 
    INT2FIX (CLUTTER_Next_Virtual_Screen), 
    INT2FIX (CLUTTER_Last_Virtual_Screen), INT2FIX (CLUTTER_Terminate_Server), 
    INT2FIX (CLUTTER_AccessX_Enable), 
    INT2FIX (CLUTTER_AccessX_Feedback_Enable), 
    INT2FIX (CLUTTER_RepeatKeys_Enable), INT2FIX (CLUTTER_SlowKeys_Enable), 
    INT2FIX (CLUTTER_BounceKeys_Enable), INT2FIX (CLUTTER_StickyKeys_Enable), 
    INT2FIX (CLUTTER_MouseKeys_Enable), 
    INT2FIX (CLUTTER_MouseKeys_Accel_Enable), 
    INT2FIX (CLUTTER_Overlay1_Enable), INT2FIX (CLUTTER_Overlay2_Enable), 
    INT2FIX (CLUTTER_AudibleBell_Enable), INT2FIX (CLUTTER_Pointer_Left), 
    INT2FIX (CLUTTER_Pointer_Right), INT2FIX (CLUTTER_Pointer_Up), 
    INT2FIX (CLUTTER_Pointer_Down), INT2FIX (CLUTTER_Pointer_UpLeft), 
    INT2FIX (CLUTTER_Pointer_UpRight), INT2FIX (CLUTTER_Pointer_DownLeft), 
    INT2FIX (CLUTTER_Pointer_DownRight), INT2FIX (CLUTTER_Pointer_Button_Dflt), 
    INT2FIX (CLUTTER_Pointer_Button1), INT2FIX (CLUTTER_Pointer_Button2), 
    INT2FIX (CLUTTER_Pointer_Button3), INT2FIX (CLUTTER_Pointer_Button4), 
    INT2FIX (CLUTTER_Pointer_Button5), INT2FIX (CLUTTER_Pointer_DblClick_Dflt), 
    INT2FIX (CLUTTER_Pointer_DblClick1), INT2FIX (CLUTTER_Pointer_DblClick2), 
    INT2FIX (CLUTTER_Pointer_DblClick3), INT2FIX (CLUTTER_Pointer_DblClick4), 
    INT2FIX (CLUTTER_Pointer_DblClick5), INT2FIX (CLUTTER_Pointer_Drag_Dflt), 
    INT2FIX (CLUTTER_Pointer_Drag1), INT2FIX (CLUTTER_Pointer_Drag2), 
    INT2FIX (CLUTTER_Pointer_Drag3), INT2FIX (CLUTTER_Pointer_Drag4), 
    INT2FIX (CLUTTER_Pointer_Drag5), INT2FIX (CLUTTER_Pointer_EnableKeys), 
    INT2FIX (CLUTTER_Pointer_Accelerate), 
    INT2FIX (CLUTTER_Pointer_DfltBtnNext), 
    INT2FIX (CLUTTER_Pointer_DfltBtnPrev), INT2FIX (CLUTTER_3270_Duplicate), 
    INT2FIX (CLUTTER_3270_FieldMark), INT2FIX (CLUTTER_3270_Right2), 
    INT2FIX (CLUTTER_3270_Left2), INT2FIX (CLUTTER_3270_BackTab), 
    INT2FIX (CLUTTER_3270_EraseEOF), INT2FIX (CLUTTER_3270_EraseInput), 
    INT2FIX (CLUTTER_3270_Reset), INT2FIX (CLUTTER_3270_Quit), 
    INT2FIX (CLUTTER_3270_PA1), INT2FIX (CLUTTER_3270_PA2), 
    INT2FIX (CLUTTER_3270_PA3), INT2FIX (CLUTTER_3270_Test), 
    INT2FIX (CLUTTER_3270_Attn), INT2FIX (CLUTTER_3270_CursorBlink), 
    INT2FIX (CLUTTER_3270_AltCursor), INT2FIX (CLUTTER_3270_KeyClick), 
    INT2FIX (CLUTTER_3270_Jump), INT2FIX (CLUTTER_3270_Ident), 
    INT2FIX (CLUTTER_3270_Rule), INT2FIX (CLUTTER_3270_Copy), 
    INT2FIX (CLUTTER_3270_Play), INT2FIX (CLUTTER_3270_Setup), 
    INT2FIX (CLUTTER_3270_Record), INT2FIX (CLUTTER_3270_ChangeScreen), 
    INT2FIX (CLUTTER_3270_DeleteWord), INT2FIX (CLUTTER_3270_ExSelect), 
    INT2FIX (CLUTTER_3270_CursorSelect), INT2FIX (CLUTTER_3270_PrintScreen), 
    INT2FIX (CLUTTER_3270_Enter), INT2FIX (CLUTTER_space), 
    INT2FIX (CLUTTER_exclam), INT2FIX (CLUTTER_quotedbl), 
    INT2FIX (CLUTTER_numbersign), INT2FIX (CLUTTER_dollar), 
    INT2FIX (CLUTTER_percent), INT2FIX (CLUTTER_ampersand), 
    INT2FIX (CLUTTER_apostrophe), INT2FIX (CLUTTER_quoteright), 
    INT2FIX (CLUTTER_parenleft), INT2FIX (CLUTTER_parenright), 
    INT2FIX (CLUTTER_asterisk), INT2FIX (CLUTTER_plus), 
    INT2FIX (CLUTTER_comma), INT2FIX (CLUTTER_minus), INT2FIX (CLUTTER_period), 
    INT2FIX (CLUTTER_slash), INT2FIX (CLUTTER_0), INT2FIX (CLUTTER_1), 
    INT2FIX (CLUTTER_2), INT2FIX (CLUTTER_3), INT2FIX (CLUTTER_4), 
    INT2FIX (CLUTTER_5), INT2FIX (CLUTTER_6), INT2FIX (CLUTTER_7), 
    INT2FIX (CLUTTER_8), INT2FIX (CLUTTER_9), INT2FIX (CLUTTER_colon), 
    INT2FIX (CLUTTER_semicolon), INT2FIX (CLUTTER_less), 
    INT2FIX (CLUTTER_equal), INT2FIX (CLUTTER_greater), 
    INT2FIX (CLUTTER_question), INT2FIX (CLUTTER_at), INT2FIX (CLUTTER_A), 
    INT2FIX (CLUTTER_B), INT2FIX (CLUTTER_C), INT2FIX (CLUTTER_D), 
    INT2FIX (CLUTTER_E), INT2FIX (CLUTTER_F), INT2FIX (CLUTTER_G), 
    INT2FIX (CLUTTER_H), INT2FIX (CLUTTER_I), INT2FIX (CLUTTER_J), 
    INT2FIX (CLUTTER_K), INT2FIX (CLUTTER_L), INT2FIX (CLUTTER_M), 
    INT2FIX (CLUTTER_N), INT2FIX (CLUTTER_O), INT2FIX (CLUTTER_P), 
    INT2FIX (CLUTTER_Q), INT2FIX (CLUTTER_R), INT2FIX (CLUTTER_S), 
    INT2FIX (CLUTTER_T), INT2FIX (CLUTTER_U), INT2FIX (CLUTTER_V), 
    INT2FIX (CLUTTER_W), INT2FIX (CLUTTER_X), INT2FIX (CLUTTER_Y), 
    INT2FIX (CLUTTER_Z), INT2FIX (CLUTTER_bracketleft), 
    INT2FIX (CLUTTER_backslash), INT2FIX (CLUTTER_bracketright), 
    INT2FIX (CLUTTER_asciicircum), INT2FIX (CLUTTER_underscore), 
    INT2FIX (CLUTTER_grave), INT2FIX (CLUTTER_quoteleft), INT2FIX (CLUTTER_a), 
    INT2FIX (CLUTTER_b), INT2FIX (CLUTTER_c), INT2FIX (CLUTTER_d), 
    INT2FIX (CLUTTER_e), INT2FIX (CLUTTER_f), INT2FIX (CLUTTER_g), 
    INT2FIX (CLUTTER_h), INT2FIX (CLUTTER_i), INT2FIX (CLUTTER_j), 
    INT2FIX (CLUTTER_k), INT2FIX (CLUTTER_l), INT2FIX (CLUTTER_m), 
    INT2FIX (CLUTTER_n), INT2FIX (CLUTTER_o), INT2FIX (CLUTTER_p), 
    INT2FIX (CLUTTER_q), INT2FIX (CLUTTER_r), INT2FIX (CLUTTER_s), 
    INT2FIX (CLUTTER_t), INT2FIX (CLUTTER_u), INT2FIX (CLUTTER_v), 
    INT2FIX (CLUTTER_w), INT2FIX (CLUTTER_x), INT2FIX (CLUTTER_y), 
    INT2FIX (CLUTTER_z), INT2FIX (CLUTTER_braceleft), INT2FIX (CLUTTER_bar), 
    INT2FIX (CLUTTER_braceright), INT2FIX (CLUTTER_asciitilde), 
    INT2FIX (CLUTTER_nobreakspace), INT2FIX (CLUTTER_exclamdown), 
    INT2FIX (CLUTTER_cent), INT2FIX (CLUTTER_sterling), 
    INT2FIX (CLUTTER_currency), INT2FIX (CLUTTER_yen), 
    INT2FIX (CLUTTER_brokenbar), INT2FIX (CLUTTER_section), 
    INT2FIX (CLUTTER_diaeresis), INT2FIX (CLUTTER_copyright), 
    INT2FIX (CLUTTER_ordfeminine), INT2FIX (CLUTTER_guillemotleft), 
    INT2FIX (CLUTTER_notsign), INT2FIX (CLUTTER_hyphen), 
    INT2FIX (CLUTTER_registered), INT2FIX (CLUTTER_macron), 
    INT2FIX (CLUTTER_degree), INT2FIX (CLUTTER_plusminus), 
    INT2FIX (CLUTTER_twosuperior), INT2FIX (CLUTTER_threesuperior), 
    INT2FIX (CLUTTER_acute), INT2FIX (CLUTTER_mu), INT2FIX (CLUTTER_paragraph), 
    INT2FIX (CLUTTER_periodcentered), INT2FIX (CLUTTER_cedilla), 
    INT2FIX (CLUTTER_onesuperior), INT2FIX (CLUTTER_masculine), 
    INT2FIX (CLUTTER_guillemotright), INT2FIX (CLUTTER_onequarter), 
    INT2FIX (CLUTTER_onehalf), INT2FIX (CLUTTER_threequarters), 
    INT2FIX (CLUTTER_questiondown), INT2FIX (CLUTTER_Agrave), 
    INT2FIX (CLUTTER_Aacute), INT2FIX (CLUTTER_Acircumflex), 
    INT2FIX (CLUTTER_Atilde), INT2FIX (CLUTTER_Adiaeresis), 
    INT2FIX (CLUTTER_Aring), INT2FIX (CLUTTER_AE), INT2FIX (CLUTTER_Ccedilla), 
    INT2FIX (CLUTTER_Egrave), INT2FIX (CLUTTER_Eacute), 
    INT2FIX (CLUTTER_Ecircumflex), INT2FIX (CLUTTER_Ediaeresis), 
    INT2FIX (CLUTTER_Igrave), INT2FIX (CLUTTER_Iacute), 
    INT2FIX (CLUTTER_Icircumflex), INT2FIX (CLUTTER_Idiaeresis), 
    INT2FIX (CLUTTER_ETH), INT2FIX (CLUTTER_Eth), INT2FIX (CLUTTER_Ntilde), 
    INT2FIX (CLUTTER_Ograve), INT2FIX (CLUTTER_Oacute), 
    INT2FIX (CLUTTER_Ocircumflex), INT2FIX (CLUTTER_Otilde), 
    INT2FIX (CLUTTER_Odiaeresis), INT2FIX (CLUTTER_multiply), 
    INT2FIX (CLUTTER_Ooblique), INT2FIX (CLUTTER_Ugrave), 
    INT2FIX (CLUTTER_Uacute), INT2FIX (CLUTTER_Ucircumflex), 
    INT2FIX (CLUTTER_Udiaeresis), INT2FIX (CLUTTER_Yacute), 
    INT2FIX (CLUTTER_THORN), INT2FIX (CLUTTER_Thorn), INT2FIX (CLUTTER_ssharp), 
    INT2FIX (CLUTTER_agrave), INT2FIX (CLUTTER_aacute), 
    INT2FIX (CLUTTER_acircumflex), INT2FIX (CLUTTER_atilde), 
    INT2FIX (CLUTTER_adiaeresis), INT2FIX (CLUTTER_aring), 
    INT2FIX (CLUTTER_ae), INT2FIX (CLUTTER_ccedilla), INT2FIX (CLUTTER_egrave), 
    INT2FIX (CLUTTER_eacute), INT2FIX (CLUTTER_ecircumflex), 
    INT2FIX (CLUTTER_ediaeresis), INT2FIX (CLUTTER_igrave), 
    INT2FIX (CLUTTER_iacute), INT2FIX (CLUTTER_icircumflex), 
    INT2FIX (CLUTTER_idiaeresis), INT2FIX (CLUTTER_eth), 
    INT2FIX (CLUTTER_ntilde), INT2FIX (CLUTTER_ograve), 
    INT2FIX (CLUTTER_oacute), INT2FIX (CLUTTER_ocircumflex), 
    INT2FIX (CLUTTER_otilde), INT2FIX (CLUTTER_odiaeresis), 
    INT2FIX (CLUTTER_division), INT2FIX (CLUTTER_oslash), 
    INT2FIX (CLUTTER_ugrave), INT2FIX (CLUTTER_uacute), 
    INT2FIX (CLUTTER_ucircumflex), INT2FIX (CLUTTER_udiaeresis), 
    INT2FIX (CLUTTER_yacute), INT2FIX (CLUTTER_thorn), 
    INT2FIX (CLUTTER_ydiaeresis), INT2FIX (CLUTTER_Aogonek), 
    INT2FIX (CLUTTER_breve), INT2FIX (CLUTTER_Lstroke), 
    INT2FIX (CLUTTER_Lcaron), INT2FIX (CLUTTER_Sacute), 
    INT2FIX (CLUTTER_Scaron), INT2FIX (CLUTTER_Scedilla), 
    INT2FIX (CLUTTER_Tcaron), INT2FIX (CLUTTER_Zacute), 
    INT2FIX (CLUTTER_Zcaron), INT2FIX (CLUTTER_Zabovedot), 
    INT2FIX (CLUTTER_aogonek), INT2FIX (CLUTTER_ogonek), 
    INT2FIX (CLUTTER_lstroke), INT2FIX (CLUTTER_lcaron), 
    INT2FIX (CLUTTER_sacute), INT2FIX (CLUTTER_caron), 
    INT2FIX (CLUTTER_scaron), INT2FIX (CLUTTER_scedilla), 
    INT2FIX (CLUTTER_tcaron), INT2FIX (CLUTTER_zacute), 
    INT2FIX (CLUTTER_doubleacute), INT2FIX (CLUTTER_zcaron), 
    INT2FIX (CLUTTER_zabovedot), INT2FIX (CLUTTER_Racute), 
    INT2FIX (CLUTTER_Abreve), INT2FIX (CLUTTER_Lacute), 
    INT2FIX (CLUTTER_Cacute), INT2FIX (CLUTTER_Ccaron), 
    INT2FIX (CLUTTER_Eogonek), INT2FIX (CLUTTER_Ecaron), 
    INT2FIX (CLUTTER_Dcaron), INT2FIX (CLUTTER_Dstroke), 
    INT2FIX (CLUTTER_Nacute), INT2FIX (CLUTTER_Ncaron), 
    INT2FIX (CLUTTER_Odoubleacute), INT2FIX (CLUTTER_Rcaron), 
    INT2FIX (CLUTTER_Uring), INT2FIX (CLUTTER_Udoubleacute), 
    INT2FIX (CLUTTER_Tcedilla), INT2FIX (CLUTTER_racute), 
    INT2FIX (CLUTTER_abreve), INT2FIX (CLUTTER_lacute), 
    INT2FIX (CLUTTER_cacute), INT2FIX (CLUTTER_ccaron), 
    INT2FIX (CLUTTER_eogonek), INT2FIX (CLUTTER_ecaron), 
    INT2FIX (CLUTTER_dcaron), INT2FIX (CLUTTER_dstroke), 
    INT2FIX (CLUTTER_nacute), INT2FIX (CLUTTER_ncaron), 
    INT2FIX (CLUTTER_odoubleacute), INT2FIX (CLUTTER_udoubleacute), 
    INT2FIX (CLUTTER_rcaron), INT2FIX (CLUTTER_uring), 
    INT2FIX (CLUTTER_tcedilla), INT2FIX (CLUTTER_abovedot), 
    INT2FIX (CLUTTER_Hstroke), INT2FIX (CLUTTER_Hcircumflex), 
    INT2FIX (CLUTTER_Iabovedot), INT2FIX (CLUTTER_Gbreve), 
    INT2FIX (CLUTTER_Jcircumflex), INT2FIX (CLUTTER_hstroke), 
    INT2FIX (CLUTTER_hcircumflex), INT2FIX (CLUTTER_idotless), 
    INT2FIX (CLUTTER_gbreve), INT2FIX (CLUTTER_jcircumflex), 
    INT2FIX (CLUTTER_Cabovedot), INT2FIX (CLUTTER_Ccircumflex), 
    INT2FIX (CLUTTER_Gabovedot), INT2FIX (CLUTTER_Gcircumflex), 
    INT2FIX (CLUTTER_Ubreve), INT2FIX (CLUTTER_Scircumflex), 
    INT2FIX (CLUTTER_cabovedot), INT2FIX (CLUTTER_ccircumflex), 
    INT2FIX (CLUTTER_gabovedot), INT2FIX (CLUTTER_gcircumflex), 
    INT2FIX (CLUTTER_ubreve), INT2FIX (CLUTTER_scircumflex), 
    INT2FIX (CLUTTER_kra), INT2FIX (CLUTTER_kappa), INT2FIX (CLUTTER_Rcedilla), 
    INT2FIX (CLUTTER_Itilde), INT2FIX (CLUTTER_Lcedilla), 
    INT2FIX (CLUTTER_Emacron), INT2FIX (CLUTTER_Gcedilla), 
    INT2FIX (CLUTTER_Tslash), INT2FIX (CLUTTER_rcedilla), 
    INT2FIX (CLUTTER_itilde), INT2FIX (CLUTTER_lcedilla), 
    INT2FIX (CLUTTER_emacron), INT2FIX (CLUTTER_gcedilla), 
    INT2FIX (CLUTTER_tslash), INT2FIX (CLUTTER_ENG), INT2FIX (CLUTTER_eng), 
    INT2FIX (CLUTTER_Amacron), INT2FIX (CLUTTER_Iogonek), 
    INT2FIX (CLUTTER_Eabovedot), INT2FIX (CLUTTER_Imacron), 
    INT2FIX (CLUTTER_Ncedilla), INT2FIX (CLUTTER_Omacron), 
    INT2FIX (CLUTTER_Kcedilla), INT2FIX (CLUTTER_Uogonek), 
    INT2FIX (CLUTTER_Utilde), INT2FIX (CLUTTER_Umacron), 
    INT2FIX (CLUTTER_amacron), INT2FIX (CLUTTER_iogonek), 
    INT2FIX (CLUTTER_eabovedot), INT2FIX (CLUTTER_imacron), 
    INT2FIX (CLUTTER_ncedilla), INT2FIX (CLUTTER_omacron), 
    INT2FIX (CLUTTER_kcedilla), INT2FIX (CLUTTER_uogonek), 
    INT2FIX (CLUTTER_utilde), INT2FIX (CLUTTER_umacron), INT2FIX (CLUTTER_OE), 
    INT2FIX (CLUTTER_oe), INT2FIX (CLUTTER_Ydiaeresis), 
    INT2FIX (CLUTTER_overline), INT2FIX (CLUTTER_kana_fullstop), 
    INT2FIX (CLUTTER_kana_openingbracket), 
    INT2FIX (CLUTTER_kana_closingbracket), INT2FIX (CLUTTER_kana_comma), 
    INT2FIX (CLUTTER_kana_conjunctive), INT2FIX (CLUTTER_kana_middledot), 
    INT2FIX (CLUTTER_kana_WO), INT2FIX (CLUTTER_kana_a), 
    INT2FIX (CLUTTER_kana_i), INT2FIX (CLUTTER_kana_u), 
    INT2FIX (CLUTTER_kana_e), INT2FIX (CLUTTER_kana_o), 
    INT2FIX (CLUTTER_kana_ya), INT2FIX (CLUTTER_kana_yu), 
    INT2FIX (CLUTTER_kana_yo), INT2FIX (CLUTTER_kana_tsu), 
    INT2FIX (CLUTTER_kana_tu), INT2FIX (CLUTTER_prolongedsound), 
    INT2FIX (CLUTTER_kana_A), INT2FIX (CLUTTER_kana_I), 
    INT2FIX (CLUTTER_kana_U), INT2FIX (CLUTTER_kana_E), 
    INT2FIX (CLUTTER_kana_O), INT2FIX (CLUTTER_kana_KA), 
    INT2FIX (CLUTTER_kana_KI), INT2FIX (CLUTTER_kana_KU), 
    INT2FIX (CLUTTER_kana_KE), INT2FIX (CLUTTER_kana_KO), 
    INT2FIX (CLUTTER_kana_SA), INT2FIX (CLUTTER_kana_SHI), 
    INT2FIX (CLUTTER_kana_SU), INT2FIX (CLUTTER_kana_SE), 
    INT2FIX (CLUTTER_kana_SO), INT2FIX (CLUTTER_kana_TA), 
    INT2FIX (CLUTTER_kana_CHI), INT2FIX (CLUTTER_kana_TI), 
    INT2FIX (CLUTTER_kana_TSU), INT2FIX (CLUTTER_kana_TU), 
    INT2FIX (CLUTTER_kana_TE), INT2FIX (CLUTTER_kana_TO), 
    INT2FIX (CLUTTER_kana_NA), INT2FIX (CLUTTER_kana_NI), 
    INT2FIX (CLUTTER_kana_NU), INT2FIX (CLUTTER_kana_NE), 
    INT2FIX (CLUTTER_kana_NO), INT2FIX (CLUTTER_kana_HA), 
    INT2FIX (CLUTTER_kana_HI), INT2FIX (CLUTTER_kana_FU), 
    INT2FIX (CLUTTER_kana_HU), INT2FIX (CLUTTER_kana_HE), 
    INT2FIX (CLUTTER_kana_HO), INT2FIX (CLUTTER_kana_MA), 
    INT2FIX (CLUTTER_kana_MI), INT2FIX (CLUTTER_kana_MU), 
    INT2FIX (CLUTTER_kana_ME), INT2FIX (CLUTTER_kana_MO), 
    INT2FIX (CLUTTER_kana_YA), INT2FIX (CLUTTER_kana_YU), 
    INT2FIX (CLUTTER_kana_YO), INT2FIX (CLUTTER_kana_RA), 
    INT2FIX (CLUTTER_kana_RI), INT2FIX (CLUTTER_kana_RU), 
    INT2FIX (CLUTTER_kana_RE), INT2FIX (CLUTTER_kana_RO), 
    INT2FIX (CLUTTER_kana_WA), INT2FIX (CLUTTER_kana_N), 
    INT2FIX (CLUTTER_voicedsound), INT2FIX (CLUTTER_semivoicedsound), 
    INT2FIX (CLUTTER_kana_switch), INT2FIX (CLUTTER_Arabic_comma), 
    INT2FIX (CLUTTER_Arabic_semicolon), INT2FIX (CLUTTER_Arabic_question_mark), 
    INT2FIX (CLUTTER_Arabic_hamza), INT2FIX (CLUTTER_Arabic_maddaonalef), 
    INT2FIX (CLUTTER_Arabic_hamzaonalef), INT2FIX (CLUTTER_Arabic_hamzaonwaw), 
    INT2FIX (CLUTTER_Arabic_hamzaunderalef), 
    INT2FIX (CLUTTER_Arabic_hamzaonyeh), INT2FIX (CLUTTER_Arabic_alef), 
    INT2FIX (CLUTTER_Arabic_beh), INT2FIX (CLUTTER_Arabic_tehmarbuta), 
    INT2FIX (CLUTTER_Arabic_teh), INT2FIX (CLUTTER_Arabic_theh), 
    INT2FIX (CLUTTER_Arabic_jeem), INT2FIX (CLUTTER_Arabic_hah), 
    INT2FIX (CLUTTER_Arabic_khah), INT2FIX (CLUTTER_Arabic_dal), 
    INT2FIX (CLUTTER_Arabic_thal), INT2FIX (CLUTTER_Arabic_ra), 
    INT2FIX (CLUTTER_Arabic_zain), INT2FIX (CLUTTER_Arabic_seen), 
    INT2FIX (CLUTTER_Arabic_sheen), INT2FIX (CLUTTER_Arabic_sad), 
    INT2FIX (CLUTTER_Arabic_dad), INT2FIX (CLUTTER_Arabic_tah), 
    INT2FIX (CLUTTER_Arabic_zah), INT2FIX (CLUTTER_Arabic_ain), 
    INT2FIX (CLUTTER_Arabic_ghain), INT2FIX (CLUTTER_Arabic_tatweel), 
    INT2FIX (CLUTTER_Arabic_feh), INT2FIX (CLUTTER_Arabic_qaf), 
    INT2FIX (CLUTTER_Arabic_kaf), INT2FIX (CLUTTER_Arabic_lam), 
    INT2FIX (CLUTTER_Arabic_meem), INT2FIX (CLUTTER_Arabic_noon), 
    INT2FIX (CLUTTER_Arabic_ha), INT2FIX (CLUTTER_Arabic_heh), 
    INT2FIX (CLUTTER_Arabic_waw), INT2FIX (CLUTTER_Arabic_alefmaksura), 
    INT2FIX (CLUTTER_Arabic_yeh), INT2FIX (CLUTTER_Arabic_fathatan), 
    INT2FIX (CLUTTER_Arabic_dammatan), INT2FIX (CLUTTER_Arabic_kasratan), 
    INT2FIX (CLUTTER_Arabic_fatha), INT2FIX (CLUTTER_Arabic_damma), 
    INT2FIX (CLUTTER_Arabic_kasra), INT2FIX (CLUTTER_Arabic_shadda), 
    INT2FIX (CLUTTER_Arabic_sukun), INT2FIX (CLUTTER_Arabic_switch), 
    INT2FIX (CLUTTER_Serbian_dje), INT2FIX (CLUTTER_Macedonia_gje), 
    INT2FIX (CLUTTER_Cyrillic_io), INT2FIX (CLUTTER_Ukrainian_ie), 
    INT2FIX (CLUTTER_Ukranian_je), INT2FIX (CLUTTER_Macedonia_dse), 
    INT2FIX (CLUTTER_Ukrainian_i), INT2FIX (CLUTTER_Ukranian_i), 
    INT2FIX (CLUTTER_Ukrainian_yi), INT2FIX (CLUTTER_Ukranian_yi), 
    INT2FIX (CLUTTER_Cyrillic_je), INT2FIX (CLUTTER_Serbian_je), 
    INT2FIX (CLUTTER_Cyrillic_lje), INT2FIX (CLUTTER_Serbian_lje), 
    INT2FIX (CLUTTER_Cyrillic_nje), INT2FIX (CLUTTER_Serbian_nje), 
    INT2FIX (CLUTTER_Serbian_tshe), INT2FIX (CLUTTER_Macedonia_kje), 
    INT2FIX (CLUTTER_Ukrainian_ghe_with_upturn), 
    INT2FIX (CLUTTER_Byelorussian_shortu), INT2FIX (CLUTTER_Cyrillic_dzhe), 
    INT2FIX (CLUTTER_Serbian_dze), INT2FIX (CLUTTER_numerosign), 
    INT2FIX (CLUTTER_Serbian_DJE), INT2FIX (CLUTTER_Macedonia_GJE), 
    INT2FIX (CLUTTER_Cyrillic_IO), INT2FIX (CLUTTER_Ukrainian_IE), 
    INT2FIX (CLUTTER_Ukranian_JE), INT2FIX (CLUTTER_Macedonia_DSE), 
    INT2FIX (CLUTTER_Ukrainian_I), INT2FIX (CLUTTER_Ukranian_I), 
    INT2FIX (CLUTTER_Ukrainian_YI), INT2FIX (CLUTTER_Ukranian_YI), 
    INT2FIX (CLUTTER_Cyrillic_JE), INT2FIX (CLUTTER_Serbian_JE), 
    INT2FIX (CLUTTER_Cyrillic_LJE), INT2FIX (CLUTTER_Serbian_LJE), 
    INT2FIX (CLUTTER_Cyrillic_NJE), INT2FIX (CLUTTER_Serbian_NJE), 
    INT2FIX (CLUTTER_Serbian_TSHE), INT2FIX (CLUTTER_Macedonia_KJE), 
    INT2FIX (CLUTTER_Ukrainian_GHE_WITH_UPTURN), 
    INT2FIX (CLUTTER_Byelorussian_SHORTU), INT2FIX (CLUTTER_Cyrillic_DZHE), 
    INT2FIX (CLUTTER_Serbian_DZE), INT2FIX (CLUTTER_Cyrillic_yu), 
    INT2FIX (CLUTTER_Cyrillic_a), INT2FIX (CLUTTER_Cyrillic_be), 
    INT2FIX (CLUTTER_Cyrillic_tse), INT2FIX (CLUTTER_Cyrillic_de), 
    INT2FIX (CLUTTER_Cyrillic_ie), INT2FIX (CLUTTER_Cyrillic_ef), 
    INT2FIX (CLUTTER_Cyrillic_ghe), INT2FIX (CLUTTER_Cyrillic_ha), 
    INT2FIX (CLUTTER_Cyrillic_i), INT2FIX (CLUTTER_Cyrillic_shorti), 
    INT2FIX (CLUTTER_Cyrillic_ka), INT2FIX (CLUTTER_Cyrillic_el), 
    INT2FIX (CLUTTER_Cyrillic_em), INT2FIX (CLUTTER_Cyrillic_en), 
    INT2FIX (CLUTTER_Cyrillic_o), INT2FIX (CLUTTER_Cyrillic_pe), 
    INT2FIX (CLUTTER_Cyrillic_ya), INT2FIX (CLUTTER_Cyrillic_er), 
    INT2FIX (CLUTTER_Cyrillic_es), INT2FIX (CLUTTER_Cyrillic_te), 
    INT2FIX (CLUTTER_Cyrillic_u), INT2FIX (CLUTTER_Cyrillic_zhe), 
    INT2FIX (CLUTTER_Cyrillic_ve), INT2FIX (CLUTTER_Cyrillic_softsign), 
    INT2FIX (CLUTTER_Cyrillic_yeru), INT2FIX (CLUTTER_Cyrillic_ze), 
    INT2FIX (CLUTTER_Cyrillic_sha), INT2FIX (CLUTTER_Cyrillic_e), 
    INT2FIX (CLUTTER_Cyrillic_shcha), INT2FIX (CLUTTER_Cyrillic_che), 
    INT2FIX (CLUTTER_Cyrillic_hardsign), INT2FIX (CLUTTER_Cyrillic_YU), 
    INT2FIX (CLUTTER_Cyrillic_A), INT2FIX (CLUTTER_Cyrillic_BE), 
    INT2FIX (CLUTTER_Cyrillic_TSE), INT2FIX (CLUTTER_Cyrillic_DE), 
    INT2FIX (CLUTTER_Cyrillic_IE), INT2FIX (CLUTTER_Cyrillic_EF), 
    INT2FIX (CLUTTER_Cyrillic_GHE), INT2FIX (CLUTTER_Cyrillic_HA), 
    INT2FIX (CLUTTER_Cyrillic_I), INT2FIX (CLUTTER_Cyrillic_SHORTI), 
    INT2FIX (CLUTTER_Cyrillic_KA), INT2FIX (CLUTTER_Cyrillic_EL), 
    INT2FIX (CLUTTER_Cyrillic_EM), INT2FIX (CLUTTER_Cyrillic_EN), 
    INT2FIX (CLUTTER_Cyrillic_O), INT2FIX (CLUTTER_Cyrillic_PE), 
    INT2FIX (CLUTTER_Cyrillic_YA), INT2FIX (CLUTTER_Cyrillic_ER), 
    INT2FIX (CLUTTER_Cyrillic_ES), INT2FIX (CLUTTER_Cyrillic_TE), 
    INT2FIX (CLUTTER_Cyrillic_U), INT2FIX (CLUTTER_Cyrillic_ZHE), 
    INT2FIX (CLUTTER_Cyrillic_VE), INT2FIX (CLUTTER_Cyrillic_SOFTSIGN), 
    INT2FIX (CLUTTER_Cyrillic_YERU), INT2FIX (CLUTTER_Cyrillic_ZE), 
    INT2FIX (CLUTTER_Cyrillic_SHA), INT2FIX (CLUTTER_Cyrillic_E), 
    INT2FIX (CLUTTER_Cyrillic_SHCHA), INT2FIX (CLUTTER_Cyrillic_CHE), 
    INT2FIX (CLUTTER_Cyrillic_HARDSIGN), INT2FIX (CLUTTER_Greek_ALPHAaccent), 
    INT2FIX (CLUTTER_Greek_EPSILONaccent), INT2FIX (CLUTTER_Greek_ETAaccent), 
    INT2FIX (CLUTTER_Greek_IOTAaccent), INT2FIX (CLUTTER_Greek_IOTAdieresis), 
    INT2FIX (CLUTTER_Greek_OMICRONaccent), 
    INT2FIX (CLUTTER_Greek_UPSILONaccent), 
    INT2FIX (CLUTTER_Greek_UPSILONdieresis), 
    INT2FIX (CLUTTER_Greek_OMEGAaccent), 
    INT2FIX (CLUTTER_Greek_accentdieresis), INT2FIX (CLUTTER_Greek_horizbar), 
    INT2FIX (CLUTTER_Greek_alphaaccent), INT2FIX (CLUTTER_Greek_epsilonaccent), 
    INT2FIX (CLUTTER_Greek_etaaccent), INT2FIX (CLUTTER_Greek_iotaaccent), 
    INT2FIX (CLUTTER_Greek_iotadieresis), 
    INT2FIX (CLUTTER_Greek_iotaaccentdieresis), 
    INT2FIX (CLUTTER_Greek_omicronaccent), 
    INT2FIX (CLUTTER_Greek_upsilonaccent), 
    INT2FIX (CLUTTER_Greek_upsilondieresis), 
    INT2FIX (CLUTTER_Greek_upsilonaccentdieresis), 
    INT2FIX (CLUTTER_Greek_omegaaccent), INT2FIX (CLUTTER_Greek_ALPHA), 
    INT2FIX (CLUTTER_Greek_BETA), INT2FIX (CLUTTER_Greek_GAMMA), 
    INT2FIX (CLUTTER_Greek_DELTA), INT2FIX (CLUTTER_Greek_EPSILON), 
    INT2FIX (CLUTTER_Greek_ZETA), INT2FIX (CLUTTER_Greek_ETA), 
    INT2FIX (CLUTTER_Greek_THETA), INT2FIX (CLUTTER_Greek_IOTA), 
    INT2FIX (CLUTTER_Greek_KAPPA), INT2FIX (CLUTTER_Greek_LAMDA), 
    INT2FIX (CLUTTER_Greek_LAMBDA), INT2FIX (CLUTTER_Greek_MU), 
    INT2FIX (CLUTTER_Greek_NU), INT2FIX (CLUTTER_Greek_XI), 
    INT2FIX (CLUTTER_Greek_OMICRON), INT2FIX (CLUTTER_Greek_PI), 
    INT2FIX (CLUTTER_Greek_RHO), INT2FIX (CLUTTER_Greek_SIGMA), 
    INT2FIX (CLUTTER_Greek_TAU), INT2FIX (CLUTTER_Greek_UPSILON), 
    INT2FIX (CLUTTER_Greek_PHI), INT2FIX (CLUTTER_Greek_CHI), 
    INT2FIX (CLUTTER_Greek_PSI), INT2FIX (CLUTTER_Greek_OMEGA), 
    INT2FIX (CLUTTER_Greek_alpha), INT2FIX (CLUTTER_Greek_beta), 
    INT2FIX (CLUTTER_Greek_gamma), INT2FIX (CLUTTER_Greek_delta), 
    INT2FIX (CLUTTER_Greek_epsilon), INT2FIX (CLUTTER_Greek_zeta), 
    INT2FIX (CLUTTER_Greek_eta), INT2FIX (CLUTTER_Greek_theta), 
    INT2FIX (CLUTTER_Greek_iota), INT2FIX (CLUTTER_Greek_kappa), 
    INT2FIX (CLUTTER_Greek_lamda), INT2FIX (CLUTTER_Greek_lambda), 
    INT2FIX (CLUTTER_Greek_mu), INT2FIX (CLUTTER_Greek_nu), 
    INT2FIX (CLUTTER_Greek_xi), INT2FIX (CLUTTER_Greek_omicron), 
    INT2FIX (CLUTTER_Greek_pi), INT2FIX (CLUTTER_Greek_rho), 
    INT2FIX (CLUTTER_Greek_sigma), INT2FIX (CLUTTER_Greek_finalsmallsigma), 
    INT2FIX (CLUTTER_Greek_tau), INT2FIX (CLUTTER_Greek_upsilon), 
    INT2FIX (CLUTTER_Greek_phi), INT2FIX (CLUTTER_Greek_chi), 
    INT2FIX (CLUTTER_Greek_psi), INT2FIX (CLUTTER_Greek_omega), 
    INT2FIX (CLUTTER_Greek_switch), INT2FIX (CLUTTER_leftradical), 
    INT2FIX (CLUTTER_topleftradical), INT2FIX (CLUTTER_horizconnector), 
    INT2FIX (CLUTTER_topintegral), INT2FIX (CLUTTER_botintegral), 
    INT2FIX (CLUTTER_vertconnector), INT2FIX (CLUTTER_topleftsqbracket), 
    INT2FIX (CLUTTER_botleftsqbracket), INT2FIX (CLUTTER_toprightsqbracket), 
    INT2FIX (CLUTTER_botrightsqbracket), INT2FIX (CLUTTER_topleftparens), 
    INT2FIX (CLUTTER_botleftparens), INT2FIX (CLUTTER_toprightparens), 
    INT2FIX (CLUTTER_botrightparens), INT2FIX (CLUTTER_leftmiddlecurlybrace), 
    INT2FIX (CLUTTER_rightmiddlecurlybrace), 
    INT2FIX (CLUTTER_topleftsummation), INT2FIX (CLUTTER_botleftsummation), 
    INT2FIX (CLUTTER_topvertsummationconnector), 
    INT2FIX (CLUTTER_botvertsummationconnector), 
    INT2FIX (CLUTTER_toprightsummation), INT2FIX (CLUTTER_botrightsummation), 
    INT2FIX (CLUTTER_rightmiddlesummation), INT2FIX (CLUTTER_lessthanequal), 
    INT2FIX (CLUTTER_notequal), INT2FIX (CLUTTER_greaterthanequal), 
    INT2FIX (CLUTTER_integral), INT2FIX (CLUTTER_therefore), 
    INT2FIX (CLUTTER_variation), INT2FIX (CLUTTER_infinity), 
    INT2FIX (CLUTTER_nabla), INT2FIX (CLUTTER_approximate), 
    INT2FIX (CLUTTER_similarequal), INT2FIX (CLUTTER_ifonlyif), 
    INT2FIX (CLUTTER_implies), INT2FIX (CLUTTER_identical), 
    INT2FIX (CLUTTER_radical), INT2FIX (CLUTTER_includedin), 
    INT2FIX (CLUTTER_includes), INT2FIX (CLUTTER_intersection), 
    INT2FIX (CLUTTER_union), INT2FIX (CLUTTER_logicaland), 
    INT2FIX (CLUTTER_logicalor), INT2FIX (CLUTTER_partialderivative), 
    INT2FIX (CLUTTER_function), INT2FIX (CLUTTER_leftarrow), 
    INT2FIX (CLUTTER_uparrow), INT2FIX (CLUTTER_rightarrow), 
    INT2FIX (CLUTTER_downarrow), INT2FIX (CLUTTER_blank), 
    INT2FIX (CLUTTER_soliddiamond), INT2FIX (CLUTTER_checkerboard), 
    INT2FIX (CLUTTER_ht), INT2FIX (CLUTTER_ff), INT2FIX (CLUTTER_cr), 
    INT2FIX (CLUTTER_lf), INT2FIX (CLUTTER_nl), INT2FIX (CLUTTER_vt), 
    INT2FIX (CLUTTER_lowrightcorner), INT2FIX (CLUTTER_uprightcorner), 
    INT2FIX (CLUTTER_upleftcorner), INT2FIX (CLUTTER_lowleftcorner), 
    INT2FIX (CLUTTER_crossinglines), INT2FIX (CLUTTER_horizlinescan1), 
    INT2FIX (CLUTTER_horizlinescan3), INT2FIX (CLUTTER_horizlinescan5), 
    INT2FIX (CLUTTER_horizlinescan7), INT2FIX (CLUTTER_horizlinescan9), 
    INT2FIX (CLUTTER_leftt), INT2FIX (CLUTTER_rightt), INT2FIX (CLUTTER_bott), 
    INT2FIX (CLUTTER_topt), INT2FIX (CLUTTER_vertbar), 
    INT2FIX (CLUTTER_emspace), INT2FIX (CLUTTER_enspace), 
    INT2FIX (CLUTTER_em3space), INT2FIX (CLUTTER_em4space), 
    INT2FIX (CLUTTER_digitspace), INT2FIX (CLUTTER_punctspace), 
    INT2FIX (CLUTTER_thinspace), INT2FIX (CLUTTER_hairspace), 
    INT2FIX (CLUTTER_emdash), INT2FIX (CLUTTER_endash), 
    INT2FIX (CLUTTER_signifblank), INT2FIX (CLUTTER_ellipsis), 
    INT2FIX (CLUTTER_doubbaselinedot), INT2FIX (CLUTTER_onethird), 
    INT2FIX (CLUTTER_twothirds), INT2FIX (CLUTTER_onefifth), 
    INT2FIX (CLUTTER_twofifths), INT2FIX (CLUTTER_threefifths), 
    INT2FIX (CLUTTER_fourfifths), INT2FIX (CLUTTER_onesixth), 
    INT2FIX (CLUTTER_fivesixths), INT2FIX (CLUTTER_careof), 
    INT2FIX (CLUTTER_figdash), INT2FIX (CLUTTER_leftanglebracket), 
    INT2FIX (CLUTTER_decimalpoint), INT2FIX (CLUTTER_rightanglebracket), 
    INT2FIX (CLUTTER_marker), INT2FIX (CLUTTER_oneeighth), 
    INT2FIX (CLUTTER_threeeighths), INT2FIX (CLUTTER_fiveeighths), 
    INT2FIX (CLUTTER_seveneighths), INT2FIX (CLUTTER_trademark), 
    INT2FIX (CLUTTER_signaturemark), INT2FIX (CLUTTER_trademarkincircle), 
    INT2FIX (CLUTTER_leftopentriangle), INT2FIX (CLUTTER_rightopentriangle), 
    INT2FIX (CLUTTER_emopencircle), INT2FIX (CLUTTER_emopenrectangle), 
    INT2FIX (CLUTTER_leftsinglequotemark), 
    INT2FIX (CLUTTER_rightsinglequotemark), 
    INT2FIX (CLUTTER_leftdoublequotemark), 
    INT2FIX (CLUTTER_rightdoublequotemark), INT2FIX (CLUTTER_prescription), 
    INT2FIX (CLUTTER_minutes), INT2FIX (CLUTTER_seconds), 
    INT2FIX (CLUTTER_latincross), INT2FIX (CLUTTER_hexagram), 
    INT2FIX (CLUTTER_filledrectbullet), INT2FIX (CLUTTER_filledlefttribullet), 
    INT2FIX (CLUTTER_filledrighttribullet), INT2FIX (CLUTTER_emfilledcircle), 
    INT2FIX (CLUTTER_emfilledrect), INT2FIX (CLUTTER_enopencircbullet), 
    INT2FIX (CLUTTER_enopensquarebullet), INT2FIX (CLUTTER_openrectbullet), 
    INT2FIX (CLUTTER_opentribulletup), INT2FIX (CLUTTER_opentribulletdown), 
    INT2FIX (CLUTTER_openstar), INT2FIX (CLUTTER_enfilledcircbullet), 
    INT2FIX (CLUTTER_enfilledsqbullet), INT2FIX (CLUTTER_filledtribulletup), 
    INT2FIX (CLUTTER_filledtribulletdown), INT2FIX (CLUTTER_leftpointer), 
    INT2FIX (CLUTTER_rightpointer), INT2FIX (CLUTTER_club), 
    INT2FIX (CLUTTER_diamond), INT2FIX (CLUTTER_heart), 
    INT2FIX (CLUTTER_maltesecross), INT2FIX (CLUTTER_dagger), 
    INT2FIX (CLUTTER_doubledagger), INT2FIX (CLUTTER_checkmark), 
    INT2FIX (CLUTTER_ballotcross), INT2FIX (CLUTTER_musicalsharp), 
    INT2FIX (CLUTTER_musicalflat), INT2FIX (CLUTTER_malesymbol), 
    INT2FIX (CLUTTER_femalesymbol), INT2FIX (CLUTTER_telephone), 
    INT2FIX (CLUTTER_telephonerecorder), INT2FIX (CLUTTER_phonographcopyright), 
    INT2FIX (CLUTTER_caret), INT2FIX (CLUTTER_singlelowquotemark), 
    INT2FIX (CLUTTER_doublelowquotemark), INT2FIX (CLUTTER_cursor), 
    INT2FIX (CLUTTER_leftcaret), INT2FIX (CLUTTER_rightcaret), 
    INT2FIX (CLUTTER_downcaret), INT2FIX (CLUTTER_upcaret), 
    INT2FIX (CLUTTER_overbar), INT2FIX (CLUTTER_downtack), 
    INT2FIX (CLUTTER_upshoe), INT2FIX (CLUTTER_downstile), 
    INT2FIX (CLUTTER_underbar), INT2FIX (CLUTTER_jot), INT2FIX (CLUTTER_quad), 
    INT2FIX (CLUTTER_uptack), INT2FIX (CLUTTER_circle), 
    INT2FIX (CLUTTER_upstile), INT2FIX (CLUTTER_downshoe), 
    INT2FIX (CLUTTER_rightshoe), INT2FIX (CLUTTER_leftshoe), 
    INT2FIX (CLUTTER_lefttack), INT2FIX (CLUTTER_righttack), 
    INT2FIX (CLUTTER_hebrew_doublelowline), INT2FIX (CLUTTER_hebrew_aleph), 
    INT2FIX (CLUTTER_hebrew_bet), INT2FIX (CLUTTER_hebrew_beth), 
    INT2FIX (CLUTTER_hebrew_gimel), INT2FIX (CLUTTER_hebrew_gimmel), 
    INT2FIX (CLUTTER_hebrew_dalet), INT2FIX (CLUTTER_hebrew_daleth), 
    INT2FIX (CLUTTER_hebrew_he), INT2FIX (CLUTTER_hebrew_waw), 
    INT2FIX (CLUTTER_hebrew_zain), INT2FIX (CLUTTER_hebrew_zayin), 
    INT2FIX (CLUTTER_hebrew_chet), INT2FIX (CLUTTER_hebrew_het), 
    INT2FIX (CLUTTER_hebrew_tet), INT2FIX (CLUTTER_hebrew_teth), 
    INT2FIX (CLUTTER_hebrew_yod), INT2FIX (CLUTTER_hebrew_finalkaph), 
    INT2FIX (CLUTTER_hebrew_kaph), INT2FIX (CLUTTER_hebrew_lamed), 
    INT2FIX (CLUTTER_hebrew_finalmem), INT2FIX (CLUTTER_hebrew_mem), 
    INT2FIX (CLUTTER_hebrew_finalnun), INT2FIX (CLUTTER_hebrew_nun), 
    INT2FIX (CLUTTER_hebrew_samech), INT2FIX (CLUTTER_hebrew_samekh), 
    INT2FIX (CLUTTER_hebrew_ayin), INT2FIX (CLUTTER_hebrew_finalpe), 
    INT2FIX (CLUTTER_hebrew_pe), INT2FIX (CLUTTER_hebrew_finalzade), 
    INT2FIX (CLUTTER_hebrew_finalzadi), INT2FIX (CLUTTER_hebrew_zade), 
    INT2FIX (CLUTTER_hebrew_zadi), INT2FIX (CLUTTER_hebrew_qoph), 
    INT2FIX (CLUTTER_hebrew_kuf), INT2FIX (CLUTTER_hebrew_resh), 
    INT2FIX (CLUTTER_hebrew_shin), INT2FIX (CLUTTER_hebrew_taw), 
    INT2FIX (CLUTTER_hebrew_taf), INT2FIX (CLUTTER_Hebrew_switch), 
    INT2FIX (CLUTTER_Thai_kokai), INT2FIX (CLUTTER_Thai_khokhai), 
    INT2FIX (CLUTTER_Thai_khokhuat), INT2FIX (CLUTTER_Thai_khokhwai), 
    INT2FIX (CLUTTER_Thai_khokhon), INT2FIX (CLUTTER_Thai_khorakhang), 
    INT2FIX (CLUTTER_Thai_ngongu), INT2FIX (CLUTTER_Thai_chochan), 
    INT2FIX (CLUTTER_Thai_choching), INT2FIX (CLUTTER_Thai_chochang), 
    INT2FIX (CLUTTER_Thai_soso), INT2FIX (CLUTTER_Thai_chochoe), 
    INT2FIX (CLUTTER_Thai_yoying), INT2FIX (CLUTTER_Thai_dochada), 
    INT2FIX (CLUTTER_Thai_topatak), INT2FIX (CLUTTER_Thai_thothan), 
    INT2FIX (CLUTTER_Thai_thonangmontho), INT2FIX (CLUTTER_Thai_thophuthao), 
    INT2FIX (CLUTTER_Thai_nonen), INT2FIX (CLUTTER_Thai_dodek), 
    INT2FIX (CLUTTER_Thai_totao), INT2FIX (CLUTTER_Thai_thothung), 
    INT2FIX (CLUTTER_Thai_thothahan), INT2FIX (CLUTTER_Thai_thothong), 
    INT2FIX (CLUTTER_Thai_nonu), INT2FIX (CLUTTER_Thai_bobaimai), 
    INT2FIX (CLUTTER_Thai_popla), INT2FIX (CLUTTER_Thai_phophung), 
    INT2FIX (CLUTTER_Thai_fofa), INT2FIX (CLUTTER_Thai_phophan), 
    INT2FIX (CLUTTER_Thai_fofan), INT2FIX (CLUTTER_Thai_phosamphao), 
    INT2FIX (CLUTTER_Thai_moma), INT2FIX (CLUTTER_Thai_yoyak), 
    INT2FIX (CLUTTER_Thai_rorua), INT2FIX (CLUTTER_Thai_ru), 
    INT2FIX (CLUTTER_Thai_loling), INT2FIX (CLUTTER_Thai_lu), 
    INT2FIX (CLUTTER_Thai_wowaen), INT2FIX (CLUTTER_Thai_sosala), 
    INT2FIX (CLUTTER_Thai_sorusi), INT2FIX (CLUTTER_Thai_sosua), 
    INT2FIX (CLUTTER_Thai_hohip), INT2FIX (CLUTTER_Thai_lochula), 
    INT2FIX (CLUTTER_Thai_oang), INT2FIX (CLUTTER_Thai_honokhuk), 
    INT2FIX (CLUTTER_Thai_paiyannoi), INT2FIX (CLUTTER_Thai_saraa), 
    INT2FIX (CLUTTER_Thai_maihanakat), INT2FIX (CLUTTER_Thai_saraaa), 
    INT2FIX (CLUTTER_Thai_saraam), INT2FIX (CLUTTER_Thai_sarai), 
    INT2FIX (CLUTTER_Thai_saraii), INT2FIX (CLUTTER_Thai_saraue), 
    INT2FIX (CLUTTER_Thai_sarauee), INT2FIX (CLUTTER_Thai_sarau), 
    INT2FIX (CLUTTER_Thai_sarauu), INT2FIX (CLUTTER_Thai_phinthu), 
    INT2FIX (CLUTTER_Thai_maihanakat_maitho), INT2FIX (CLUTTER_Thai_baht), 
    INT2FIX (CLUTTER_Thai_sarae), INT2FIX (CLUTTER_Thai_saraae), 
    INT2FIX (CLUTTER_Thai_sarao), INT2FIX (CLUTTER_Thai_saraaimaimuan), 
    INT2FIX (CLUTTER_Thai_saraaimaimalai), INT2FIX (CLUTTER_Thai_lakkhangyao), 
    INT2FIX (CLUTTER_Thai_maiyamok), INT2FIX (CLUTTER_Thai_maitaikhu), 
    INT2FIX (CLUTTER_Thai_maiek), INT2FIX (CLUTTER_Thai_maitho), 
    INT2FIX (CLUTTER_Thai_maitri), INT2FIX (CLUTTER_Thai_maichattawa), 
    INT2FIX (CLUTTER_Thai_thanthakhat), INT2FIX (CLUTTER_Thai_nikhahit), 
    INT2FIX (CLUTTER_Thai_leksun), INT2FIX (CLUTTER_Thai_leknung), 
    INT2FIX (CLUTTER_Thai_leksong), INT2FIX (CLUTTER_Thai_leksam), 
    INT2FIX (CLUTTER_Thai_leksi), INT2FIX (CLUTTER_Thai_lekha), 
    INT2FIX (CLUTTER_Thai_lekhok), INT2FIX (CLUTTER_Thai_lekchet), 
    INT2FIX (CLUTTER_Thai_lekpaet), INT2FIX (CLUTTER_Thai_lekkao), 
    INT2FIX (CLUTTER_Hangul), INT2FIX (CLUTTER_Hangul_Start), 
    INT2FIX (CLUTTER_Hangul_End), INT2FIX (CLUTTER_Hangul_Hanja), 
    INT2FIX (CLUTTER_Hangul_Jamo), INT2FIX (CLUTTER_Hangul_Romaja), 
    INT2FIX (CLUTTER_Hangul_Codeinput), INT2FIX (CLUTTER_Hangul_Jeonja), 
    INT2FIX (CLUTTER_Hangul_Banja), INT2FIX (CLUTTER_Hangul_PreHanja), 
    INT2FIX (CLUTTER_Hangul_PostHanja), 
    INT2FIX (CLUTTER_Hangul_SingleCandidate), 
    INT2FIX (CLUTTER_Hangul_MultipleCandidate), 
    INT2FIX (CLUTTER_Hangul_PreviousCandidate), 
    INT2FIX (CLUTTER_Hangul_Special), INT2FIX (CLUTTER_Hangul_switch), 
    INT2FIX (CLUTTER_Hangul_Kiyeog), INT2FIX (CLUTTER_Hangul_SsangKiyeog), 
    INT2FIX (CLUTTER_Hangul_KiyeogSios), INT2FIX (CLUTTER_Hangul_Nieun), 
    INT2FIX (CLUTTER_Hangul_NieunJieuj), INT2FIX (CLUTTER_Hangul_NieunHieuh), 
    INT2FIX (CLUTTER_Hangul_Dikeud), INT2FIX (CLUTTER_Hangul_SsangDikeud), 
    INT2FIX (CLUTTER_Hangul_Rieul), INT2FIX (CLUTTER_Hangul_RieulKiyeog), 
    INT2FIX (CLUTTER_Hangul_RieulMieum), INT2FIX (CLUTTER_Hangul_RieulPieub), 
    INT2FIX (CLUTTER_Hangul_RieulSios), INT2FIX (CLUTTER_Hangul_RieulTieut), 
    INT2FIX (CLUTTER_Hangul_RieulPhieuf), INT2FIX (CLUTTER_Hangul_RieulHieuh), 
    INT2FIX (CLUTTER_Hangul_Mieum), INT2FIX (CLUTTER_Hangul_Pieub), 
    INT2FIX (CLUTTER_Hangul_SsangPieub), INT2FIX (CLUTTER_Hangul_PieubSios), 
    INT2FIX (CLUTTER_Hangul_Sios), INT2FIX (CLUTTER_Hangul_SsangSios), 
    INT2FIX (CLUTTER_Hangul_Ieung), INT2FIX (CLUTTER_Hangul_Jieuj), 
    INT2FIX (CLUTTER_Hangul_SsangJieuj), INT2FIX (CLUTTER_Hangul_Cieuc), 
    INT2FIX (CLUTTER_Hangul_Khieuq), INT2FIX (CLUTTER_Hangul_Tieut), 
    INT2FIX (CLUTTER_Hangul_Phieuf), INT2FIX (CLUTTER_Hangul_Hieuh), 
    INT2FIX (CLUTTER_Hangul_A), INT2FIX (CLUTTER_Hangul_AE), 
    INT2FIX (CLUTTER_Hangul_YA), INT2FIX (CLUTTER_Hangul_YAE), 
    INT2FIX (CLUTTER_Hangul_EO), INT2FIX (CLUTTER_Hangul_E), 
    INT2FIX (CLUTTER_Hangul_YEO), INT2FIX (CLUTTER_Hangul_YE), 
    INT2FIX (CLUTTER_Hangul_O), INT2FIX (CLUTTER_Hangul_WA), 
    INT2FIX (CLUTTER_Hangul_WAE), INT2FIX (CLUTTER_Hangul_OE), 
    INT2FIX (CLUTTER_Hangul_YO), INT2FIX (CLUTTER_Hangul_U), 
    INT2FIX (CLUTTER_Hangul_WEO), INT2FIX (CLUTTER_Hangul_WE), 
    INT2FIX (CLUTTER_Hangul_WI), INT2FIX (CLUTTER_Hangul_YU), 
    INT2FIX (CLUTTER_Hangul_EU), INT2FIX (CLUTTER_Hangul_YI), 
    INT2FIX (CLUTTER_Hangul_I), INT2FIX (CLUTTER_Hangul_J_Kiyeog), 
    INT2FIX (CLUTTER_Hangul_J_SsangKiyeog), 
    INT2FIX (CLUTTER_Hangul_J_KiyeogSios), INT2FIX (CLUTTER_Hangul_J_Nieun), 
    INT2FIX (CLUTTER_Hangul_J_NieunJieuj), 
    INT2FIX (CLUTTER_Hangul_J_NieunHieuh), INT2FIX (CLUTTER_Hangul_J_Dikeud), 
    INT2FIX (CLUTTER_Hangul_J_Rieul), INT2FIX (CLUTTER_Hangul_J_RieulKiyeog), 
    INT2FIX (CLUTTER_Hangul_J_RieulMieum), 
    INT2FIX (CLUTTER_Hangul_J_RieulPieub), 
    INT2FIX (CLUTTER_Hangul_J_RieulSios), 
    INT2FIX (CLUTTER_Hangul_J_RieulTieut), 
    INT2FIX (CLUTTER_Hangul_J_RieulPhieuf), 
    INT2FIX (CLUTTER_Hangul_J_RieulHieuh), INT2FIX (CLUTTER_Hangul_J_Mieum), 
    INT2FIX (CLUTTER_Hangul_J_Pieub), INT2FIX (CLUTTER_Hangul_J_PieubSios), 
    INT2FIX (CLUTTER_Hangul_J_Sios), INT2FIX (CLUTTER_Hangul_J_SsangSios), 
    INT2FIX (CLUTTER_Hangul_J_Ieung), INT2FIX (CLUTTER_Hangul_J_Jieuj), 
    INT2FIX (CLUTTER_Hangul_J_Cieuc), INT2FIX (CLUTTER_Hangul_J_Khieuq), 
    INT2FIX (CLUTTER_Hangul_J_Tieut), INT2FIX (CLUTTER_Hangul_J_Phieuf), 
    INT2FIX (CLUTTER_Hangul_J_Hieuh), 
    INT2FIX (CLUTTER_Hangul_RieulYeorinHieuh), 
    INT2FIX (CLUTTER_Hangul_SunkyeongeumMieum), 
    INT2FIX (CLUTTER_Hangul_SunkyeongeumPieub), 
    INT2FIX (CLUTTER_Hangul_PanSios), 
    INT2FIX (CLUTTER_Hangul_KkogjiDalrinIeung), 
    INT2FIX (CLUTTER_Hangul_SunkyeongeumPhieuf), 
    INT2FIX (CLUTTER_Hangul_YeorinHieuh), INT2FIX (CLUTTER_Hangul_AraeA), 
    INT2FIX (CLUTTER_Hangul_AraeAE), INT2FIX (CLUTTER_Hangul_J_PanSios), 
    INT2FIX (CLUTTER_Hangul_J_KkogjiDalrinIeung), 
    INT2FIX (CLUTTER_Hangul_J_YeorinHieuh), INT2FIX (CLUTTER_Korean_Won), 
    INT2FIX (CLUTTER_EcuSign), INT2FIX (CLUTTER_ColonSign), 
    INT2FIX (CLUTTER_CruzeiroSign), INT2FIX (CLUTTER_FFrancSign), 
    INT2FIX (CLUTTER_LiraSign), INT2FIX (CLUTTER_MillSign), 
    INT2FIX (CLUTTER_NairaSign), INT2FIX (CLUTTER_PesetaSign), 
    INT2FIX (CLUTTER_RupeeSign), INT2FIX (CLUTTER_WonSign), 
    INT2FIX (CLUTTER_NewSheqelSign), INT2FIX (CLUTTER_DongSign), 
    INT2FIX (CLUTTER_EuroSign)
  };

void
rbclt_keysyms_init ()
{
  const char *namep = rbclt_keysyms_names;
  const VALUE *valuep = rbclt_keysyms_values;

  for (namep = rbclt_keysyms_names, valuep = rbclt_keysyms_values;
       *namep;
       namep += strlen (namep) + 1, valuep++)
    rb_define_const (rbclt_c_clutter, namep, *valuep);
}

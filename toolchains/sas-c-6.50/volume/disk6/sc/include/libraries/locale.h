€ˆLIBRARIES_LOCALE_H€LIBRARIES_LOCALE_HˆEXEC_TYPES_HŒ<exec/types.h>‡ˆµŒ<exec/nodes.h>‡ˆ·Œ<exec/lists.h>‡ˆEXEC_LIBRARIES_HŒ<exec/libraries.h>‡ˆUTILITY_TAGITEM_HŒ<utility/tagitem.h>‡€DAY_1 1€DAY_2 2€DAY_3 3€DAY_4 4€DAY_5 5€DAY_6 6€DAY_7 7€ABDAY_1 8€ABDAY_2 9€ABDAY_3 10€ABDAY_4 11€ABDAY_5 12€ABDAY_6 13€ABDAY_7 14€MON_1 15€MON_2 16€MON_3 17€MON_4 18€MON_5 19€MON_6 20€MON_7 21€MON_8 22€MON_9 23€MON_10 24€MON_11 25€MON_12 26€ABMON_1 27€ABMON_2 28€ABMON_3 29€ABMON_4 30€ABMON_5 31€ABMON_6 32€ABMON_7 33€ABMON_8 34€ABMON_9 35€ABMON_10 36€ABMON_11 37€ABMON_12 38€YESSTR 39€NOSTR 40€AM_STR 41€PM_STR 42€SOFTHYPHEN 43€HARDHYPHEN 44€OPENQUOTE 45€CLOSEQUOTE 46€YESTERDAYSTR 47€TODAYSTR 48€TOMORROWSTR 49€FUTURESTR 50€MAXSTRMSG 51
ƒLocaleBase
{
ƒLibrary lb_LibNode;
BOOL lb_SysPatches;
};
ƒLocale
{
STRPTR loc_LocaleName;
STRPTR loc_LanguageName;
STRPTR loc_PrefLanguages[10];
—loc_Flags;
—loc_CodeSet;
—loc_CountryCode;
—loc_TelephoneCode;
’loc_GMTOffset;
Šloc_MeasuringSystem;
Šloc_CalendarType;
Šloc_Reserved0[2];
STRPTR loc_DateTimeFormat;
STRPTR loc_DateFormat;
STRPTR loc_TimeFormat;
STRPTR loc_ShortDateTimeFormat;
STRPTR loc_ShortDateFormat;
STRPTR loc_ShortTimeFormat;
STRPTR loc_DecimalPoint;
STRPTR loc_GroupSeparator;
STRPTR loc_FracGroupSeparator;
Š*loc_Grouping;
Š*loc_FracGrouping;
STRPTR loc_MonDecimalPoint;
STRPTR loc_MonGroupSeparator;
STRPTR loc_MonFracGroupSeparator;
Š*loc_MonGrouping;
Š*loc_MonFracGrouping;
Šloc_MonFracDigits;
Šloc_MonIntFracDigits;
Šloc_Reserved1[2];
STRPTR loc_MonCS;
STRPTR loc_MonSmallCS;
STRPTR loc_MonIntCS;
STRPTR loc_MonPositiveSign;
Šloc_MonPositiveSpaceSep;
Šloc_MonPositiveSignPos;
Šloc_MonPositiveCSPos;
Šloc_Reserved2;
STRPTR loc_MonNegativeSign;
Šloc_MonNegativeSpaceSep;
Šloc_MonNegativeSignPos;
Šloc_MonNegativeCSPos;
Šloc_Reserved3;
};€MS_ISO 0€MS_AMERICAN 1€MS_IMPERIAL 2€MS_BRITISH 3€CT_7SUN 0€CT_7MON 1€CT_7TUE 2€CT_7WED 3€CT_7THU 4€CT_7FRI 5€CT_7SAT 6€SS_NOSPACE 0€SS_SPACE 1€SP_PARENS 0€SP_PREC_ALL 1€SP_SUCC_ALL 2€SP_PREC_CURR 3€SP_SUCC_CURR 4€CSP_PRECEDES 0€CSP_SUCCEEDS 1€OC_TagBase (TAG_USER+0x90000)€OC_BuiltInLanguage OC_TagBase+1€OC_BuiltInCodeSet OC_TagBase+2€OC_Version OC_TagBase+3€OC_Language OC_TagBase+4€SC_ASCII 0€SC_COLLATE1 1€SC_COLLATE2 2
ƒCatalog
{
ƒ¬cat_Link;
‰cat_Pad;
STRPTR cat_Language;
—cat_CodeSet;
‰cat_Version;
‰cat_Revision;
};‡
ÄàCLIB_LOCALE_PROTOS_HÄCLIB_LOCALE_PROTOS_HàEXEC_TYPES_Hå<exec/types.h>áàLIBRARIES_LOCALE_Hå<libraries/locale.h>áàDOS_DOS_Hå<dos/dos.h>áàUTILITY_HOOKS_Hå<utility/hooks.h>áàUTILITY_TAGITEM_Hå<utility/tagitem.h>áàREXX_STORAGE_Hå<rexx/storage.h>á
ãCloseCatalog(ÉCatalog*catalog);
ãCloseLocale(ÉLocale*locale);
óConvToLower(ÉLocale*locale,éçcharacter);
óConvToUpper(ÉLocale*locale,éçcharacter);
ãFormatDate(ÉLocale*locale,STRPTR fmtTemplate,
ÉDateStamp*date,ÉHook*putCharFunc);
îFormatString(ÉLocale*locale,STRPTR fmtTemplate,îdataStream,
ÉHook*putCharFunc);
STRPTR GetCatalogStr(ÉCatalog*catalog,çstringNum,
STRPTR defaultString);
STRPTR GetLocaleStr(ÉLocale*locale,éçstringNum);
BOOL IsAlNum(ÉLocale*locale,éçcharacter);
BOOL IsAlpha(ÉLocale*locale,éçcharacter);
BOOL IsCntrl(ÉLocale*locale,éçcharacter);
BOOL IsDigit(ÉLocale*locale,éçcharacter);
BOOL IsGraph(ÉLocale*locale,éçcharacter);
BOOL IsLower(ÉLocale*locale,éçcharacter);
BOOL IsPrint(ÉLocale*locale,éçcharacter);
BOOL IsPunct(ÉLocale*locale,éçcharacter);
BOOL IsSpace(ÉLocale*locale,éçcharacter);
BOOL IsUpper(ÉLocale*locale,éçcharacter);
BOOL IsXDigit(ÉLocale*locale,éçcharacter);
ÉCatalog*OpenCatalogA(ÉLocale*locale,STRPTR name,
ÉTagItem*tags);
ÉCatalog*OpenCatalog(ÉLocale*locale,STRPTR name,Tag tag1,
...);
ÉLocale*OpenLocale(STRPTR name);
BOOL ParseDate(ÉLocale*locale,ÉDateStamp*date,
STRPTR fmtTemplate,ÉHook*getCharFunc);
óStrConvert(ÉLocale*locale,STRPTR string,îbuffer,
éçbufferSize,éçtype);
íStrnCmp(ÉLocale*locale,STRPTR string1,STRPTR string2,
çlength,éçtype);á
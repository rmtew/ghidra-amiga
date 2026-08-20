€ˆ_TIME_H€_TIME_H 1Œ<sys/commnull.h>€CLOCKS_PER_SEC 1000€CLK_TCK CLOCKS_PER_SECŒ<sys/commsize.h>
¥clock_t;Œ<sys/commtime.h>
ƒtm{
‚tm_sec;
‚tm_min;
‚tm_hour;
‚tm_mday;
‚tm_mon;
‚tm_year;
‚tm_wday;
‚tm_yday;
‚tm_isdst;
};
clock_t clock(‹);
difftime(time_t,time_t);
time_t mktime(ƒtm*);
time_t time(time_t*);
„*asctime(constƒtm*);
„*ctime(const time_t*);
ƒtm*gmtime(const time_t*);
ƒtm*localtime(const time_t*);
size_t strftime(„*,size_t,const„*,constƒtm*);ˆ_STRICT_ANSI
‹getclk(„*);
‚chgclk(const„*);
‹utunpk(,„*);
utpack(const„*);
‚timer(‚*);
‚datecmp(constƒDateStamp*,constƒDateStamp*);
time_t __datecvt(constƒDateStamp*);
ƒDateStamp*__timecvt(time_t);
‹__tzset(‹);€timezone __timezone€tzname __tzname€daylight __daylight€tzset __tzset€TZ "CST6"€DAY0 4‡
‚__daylight;
__timezone;
„*__tzname[2];
„__tzstn[4];
„__tzdtn[4];
„*_TZ;‡
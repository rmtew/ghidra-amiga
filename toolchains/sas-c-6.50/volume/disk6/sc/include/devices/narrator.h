€ˆDEVICES_NARRATOR_H€DEVICES_NARRATOR_HˆºŒ"exec/io.h"‡€NDB_NEWIORB 0€NDB_WORDSYNC 1€NDB_SYLSYNC 2€NDF_NEWIORB (1<<NDB_NEWIORB)€NDF_WORDSYNC (1<<NDB_WORDSYNC)€NDF_SYLSYNC (1<<NDB_SYLSYNC)€ND_NoMem -2€ND_NoAudLib -3€ND_MakeBad -4€ND_UnitErr -5€ND_CantAlloc -6€ND_Unimpl -7€ND_NoWrite -8€ND_Expunged -9€ND_PhonErr -20€ND_RateErr -21€ND_PitchErr -22€ND_SexErr -23€ND_ModeErr -24€ND_FreqErr -25€ND_VolErr -26€ND_DCentErr -27€ND_CentPhonErr -28€DEFPITCH 110€DEFRATE 150€DEFVOL 64€DEFFREQ 22200€MALE 0€FEMALE 1€NATURALF0 0€ROBOTICF0 1€MANUALF0 2€DEFSEX MALE€DEFMODE NATURALF0€DEFARTIC 100€DEFCENTRAL 0€DEFF0PERT 0€DEFF0ENTHUS 32€DEFPRIORITY 100€MINRATE 40€MAXRATE 400€MINPITCH 65€MAXPITCH 320€MINFREQ 5000€MAXFREQ 28000€MINVOL 0€MAXVOL 64€MINCENT 0€MAXCENT 100
ƒnarrator_rb{
ƒIOStdReq message;
‰rate;
‰pitch;
‰mode;
‰sex;
Š*ch_masks;
‰nm_masks;
‰volume;
‰sampfreq;
Šmouths;
Šchanmask;
Šnumchan;
Šflags;
ŠF0enthusiasm;
ŠF0perturb;
šF1adj;
šF2adj;
šF3adj;
šA1adj;
šA2adj;
šA3adj;
Šarticulate;
Šcentralize;
„*centphon;
šAVbias;
šAFbias;
špriority;
špad1;
};
ƒmouth_rb{
ƒnarrator_rb voice;
Šwidth;
Šheight;
Šshape;
Šsync;
};‡
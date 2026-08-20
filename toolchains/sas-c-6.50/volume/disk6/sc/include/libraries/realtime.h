€ˆLIBRARIES_REALTIME_H€LIBRARIES_REALTIME_HˆEXEC_TYPES_HŒ<exec/types.h>‡ˆ·Œ<exec/lists.h>‡ˆEXEC_LIBRARIES_HŒ<exec/libraries.h>‡ˆUTILITY_TAGITEM_HŒ<utility/tagitem.h>‡ˆUTILITY_HOOKS_HŒ<utility/hooks.h>‡€TICK_FREQ 1200
ƒConductor
{
ƒ¬cdt_Link;
‰cdt_Reserved0;
ƒMinList cdt_Players;
—cdt_ClockTime;
—cdt_StartTime;
—cdt_ExternalTime;
—cdt_MaxExternalTime;
—cdt_Metronome;
‰cdt_Reserved1;
‰cdt_Flags;
Šcdt_State;
};€CONDUCTF_EXTERNAL (1<<0)€CONDUCTF_GOTTICK (1<<1)€CONDUCTF_METROSET (1<<2)€CONDUCTF_PRIVATE (1<<3)€CONDUCTB_EXTERNAL 0€CONDUCTB_GOTTICK 1€CONDUCTB_METROSET 2€CONDUCTB_PRIVATE 3€CONDSTATE_STOPPED 0€CONDSTATE_PAUSED 1€CONDSTATE_LOCATE 2€CONDSTATE_RUNNING 3€CONDSTATE_METRIC -1€CONDSTATE_SHUTTLE -2€CONDSTATE_LOCATE_SET -3
ƒPlayer
{
ƒ¬pl_Link;
špl_Reserved0;
špl_Reserved1;
ƒHook*pl_Hook;
ƒConductor*pl_Source;
ƒTask*pl_Task;
’pl_MetricTime;
’pl_AlarmTime;
‹*pl_UserData;
‰pl_PlayerID;
‰pl_Flags;
};€PLAYERF_READY (1<<0)€PLAYERF_ALARMSET (1<<1)€PLAYERF_QUIET (1<<2)€PLAYERF_CONDUCTED (1<<3)€PLAYERF_EXTSYNC (1<<4)€PLAYERB_READY 0€PLAYERB_ALARMSET 1€PLAYERB_QUIET 2€PLAYERB_CONDUCTED 3€PLAYERB_EXTSYNC 4€PLAYER_Base (TAG_USER+64)€PLAYER_Hook (PLAYER_Base+1)€PLAYER_Name (PLAYER_Base+2)€PLAYER_Priority (PLAYER_Base+3)€PLAYER_Conductor (PLAYER_Base+4)€PLAYER_Ready (PLAYER_Base+5)€PLAYER_AlarmTime (PLAYER_Base+12)€PLAYER_Alarm (PLAYER_Base+13)€PLAYER_AlarmSigTask (PLAYER_Base+6)€PLAYER_AlarmSigBit (PLAYER_Base+8)€PLAYER_Conducted (PLAYER_Base+7)€PLAYER_Quiet (PLAYER_Base+9)€PLAYER_UserData (PLAYER_Base+10)€PLAYER_ID (PLAYER_Base+11)€PLAYER_ExtSync (PLAYER_Base+14)€PLAYER_ErrorCode (PLAYER_Base+15)€PM_TICK 0€PM_STATE 1€PM_POSITION 2€PM_SHUTTLE 3
ƒpmTime
{
—pmt_Method;
—pmt_Time;
};
ƒpmState
{
—pms_Method;
—pms_OldState;
};€RT_CONDUCTORS 0€RTE_NOMEMORY‘€RTE_NOCONDUCTOR´€RTE_NOTIMER 803€RTE_PLAYING 804
ƒRealTimeBase
{
ƒLibrary rtb_LibNode;
Šrtb_Reserved0[2];
—rtb_Time;
—rtb_TimeFrac;
‰rtb_Reserved1;
˜rtb_TickErr;
};€RealTime_TickErr_Min -705€RealTime_TickErr_Max 705‡
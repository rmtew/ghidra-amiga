#ifndef LIBRARIES_EXPANSION_H
#include <libraries/expansion.h>
#endif
#ifndef LIBRARIES_EXPANSIONBASE_H
#include <libraries/expansionbase.h>
#endif
void AddConfigDev(long);
void AllocBoardMem(long);
struct ConfigDev *AllocConfigDev(void);
CPTR AllocExpansionMem(long, long, long);
void ConfigBoard(long, long);
void ConfigChain(long);
struct ConfigDev *FindConfigDev(long, long, long);
void FreeBoardMem(long, long);
void FreeConfigDev(long);
void FreeExpansionMem(long, long);
void ReadExpansionByte(long, long);
void ReadExpansionRom(long, long);
void RemConfigDev(long);
void WriteExpansionByte(long, long, long);
void ObtainConfigBinding(void);
void ReleaseConfigBinding(void);
void SetCurrentBinding(long, long);
void GetCurrentBinding(long, long);
struct DeviceNode *MakeDosNode(long *);
void AddDosNode(long, long, struct dosNode *);
#ifndef  NO_PRAGMAS
#pragma libcall ExpansionBase AddConfigDev 1e 801
#pragma libcall ExpansionBase AllocBoardMem 2a 1
#pragma libcall ExpansionBase AllocConfigDev 30 0
#pragma libcall ExpansionBase AllocExpansionMem 36 21003
#pragma libcall ExpansionBase ConfigBoard 3c 9802
#pragma libcall ExpansionBase ConfigChain 42 801
#pragma libcall ExpansionBase FindConfigDev 48 10803
#pragma libcall ExpansionBase FreeBoardMem 4e 1002
#pragma libcall ExpansionBase FreeConfigDev 54 801
#pragma libcall ExpansionBase FreeExpansionMem 5a 1002
#pragma libcall ExpansionBase ReadExpansionByte 60 802
#pragma libcall ExpansionBase ReadExpansionRom 66 9802
#pragma libcall ExpansionBase RemConfigDev 6c 801
#pragma libcall ExpansionBase WriteExpansionByte 72 10803
#pragma libcall ExpansionBase ObtainConfigBinding 78 0
#pragma libcall ExpansionBase ReleaseConfigBinding 7e 0
#pragma libcall ExpansionBase SetCurrentBinding 84 802
#pragma libcall ExpansionBase GetCurrentBinding 8a 802
#pragma libcall ExpansionBase MakeDosNode 90 801
#pragma libcall ExpansionBase AddDosNode 96 81003
#endif

/*
 * CParser roots for declarations which the NDK headers define but no other
 * parsed declaration references.  Keep this list minimal and explanatory:
 * the declarations themselves remain in the vendored NDK headers.
 */

#include <exec/io.h>

struct IOStdReq *ghidra_ndk39_root_iostdreq;

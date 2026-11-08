// Entry translation unit of the WASM module.
//
// It exists so the module has its own compilation unit (and its own flags)
// without depending on any host runtime. The exported surface is the C ABI in
// <esp32sim/abi.h>; the linker keeps those symbols alive through
// -sEXPORTED_FUNCTIONS, not through anything referenced here.
//
// ABI contract checks live in core/src/abi.cpp so the native build sees them too.

#include "esp32sim/abi.h"

#ifndef framework_hpp
#define framework_hpp

#include <ntifs.h>
#include <ntddk.h>
#include <intrin.h>
#include <stdarg.h>
#include <cstdint>
#include <stdlib.h>

#include <types/ketypes.hpp>
#include <types/ex.hpp>
#include <types/haltypes.hpp>
#include <types/ia32.hpp>

extern "C" PLIST_ENTRY PsLoadedModuleList;
extern "C" PHAL_PRIVATE_DISPATCH HalPrivateDispatchTable;
extern "C" PPEB PsGetProcessPeb(PEPROCESS);
extern "C" void* PsGetProcessSectionBaseAddress(PEPROCESS);

#endif // !framework_hpp
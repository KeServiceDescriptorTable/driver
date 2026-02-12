#ifndef ex_hpp
#define ex_hpp

#include <framework.hpp>

typedef struct _EX_FAST_REF {
    union {
        void* Object;
        uint64_t RefCnt : 4;
        uint64_t Value;
    };
} EX_FAST_REF, * PEX_FAST_REF;

typedef NTSTATUS(*PEX_CALLBACK_FUNCTION) (
    void* CallbackContext,
    void* Argument1,
    void* Argument2
);

typedef struct _EX_CALLBACK_ROUTINE_BLOCK {
    EX_RUNDOWN_REF RundownProtect;
    PEX_CALLBACK_FUNCTION Function;
    void* Context;
} EX_CALLBACK_ROUTINE_BLOCK, * PEX_CALLBACK_ROUTINE_BLOCK;

typedef struct _EX_CALLBACK {
    EX_FAST_REF RoutineBlock;
} EX_CALLBACK, * PEX_CALLBACK;

#endif // !ex_hpp
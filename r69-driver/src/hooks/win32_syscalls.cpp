#include <hooks/hooks.hpp>

NTSTATUS hooks::win32_syscalls::hook_handler(void* context, void* arg_1, void* arg_2) {
    return o_callback(context, arg_1, arg_2);
}
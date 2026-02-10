#include <framework.hpp>
#include <hooks/hooks.hpp>

NTSTATUS FxDriverEntry(PDRIVER_OBJECT driver_object) {
    hooks::initialize();
    return STATUS_SUCCESS;
}
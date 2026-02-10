#include <hooks/hooks.hpp>
#include <communication/communication.hpp>

NTSTATUS hooks::hal_timer_query_auxiliary_counter_frequency::hook_handler(uint64_t*) {
	return communication::communication_handler(KeGetCurrentThread()->TrapFrame);
}
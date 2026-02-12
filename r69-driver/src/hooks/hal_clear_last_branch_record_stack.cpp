#include <hooks/hooks.hpp>

uint8_t hooks::hal_clear_last_branch_record_stack::hook_handler() {
	auto* process = IoGetCurrentProcess();
	if (process)
		process->DirectoryTableBase = __readcr3();

	return o_hal_clear_last_branch_record_stack();
}
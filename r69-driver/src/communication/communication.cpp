#include <communication/communication.hpp>

NTSTATUS communication::communication_handler(PKTRAP_FRAME trap_frame) {
	auto* packet = (c_packet*)trap_frame->Rdx;
	if (!MmIsAddressValid(packet))
		return STATUS_INVALID_PARAMETER;

	auto status = STATUS_SUCCESS;
	const auto syscall = packet->get_syscall();

	switch (syscall) {
	case e_syscall::read_process_memory:
		status = handler::read_process_memory(packet->get<copy_process_memory_packet>());
		break;
	case e_syscall::write_process_memory:
		status = handler::write_process_memory(packet->get<copy_process_memory_packet>());
		break;
	case e_syscall::query_process_data:
		status = handler::query_process_data(packet->get<query_process_data_packet>());
		break;
	case e_syscall::null:
	default:
		status = STATUS_INVALID_PARAMETER;
	}

	return status;
}
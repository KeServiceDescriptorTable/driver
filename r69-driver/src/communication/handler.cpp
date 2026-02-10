#include <communication/handler.hpp>

NTSTATUS handler::read_process_memory(copy_process_memory_packet* input) {
    if (!MmIsAddressValid(input))
        return STATUS_ACCESS_VIOLATION;

    PEPROCESS process = nullptr;
    if (!NT_SUCCESS(PsLookupProcessByProcessId(ULongToHandle(input->process_id), &process)))
        return STATUS_UNSUCCESSFUL;

    const auto directory_table_base = process->DirectoryTableBase;
	const auto callers_directory_table_base = __readcr3();

    ObfDereferenceObject(process);

	auto size = input->size;
	auto* buffer = (uint8_t*)input->dest;
	auto address = input->source;

	while (size) {
		if (!MmIsAddressValid(buffer) || !utils::paging::translate_virtual_to_physical(callers_directory_table_base, (uint64_t)buffer))
			return STATUS_UNSUCCESSFUL;

		auto size_to_read = min(size, PAGE_SIZE);

		auto physical_address = utils::paging::translate_virtual_to_physical(directory_table_base, address);
		if (!physical_address)
			return STATUS_UNSUCCESSFUL;

		if (!utils::physical_memory::read((void*)physical_address, buffer, size_to_read))
			return STATUS_UNSUCCESSFUL;

		size -= size_to_read;
		buffer += size_to_read;
		address += size_to_read;
	}

    return STATUS_SUCCESS;
}

NTSTATUS handler::write_process_memory(copy_process_memory_packet* input) {
	if (!MmIsAddressValid(input))
		return STATUS_ACCESS_VIOLATION;

	PEPROCESS process = nullptr;
	if (!NT_SUCCESS(PsLookupProcessByProcessId(ULongToHandle(input->process_id), &process)))
		return STATUS_UNSUCCESSFUL;

	const auto directory_table_base = process->DirectoryTableBase;
	const auto callers_directory_table_base = __readcr3();

	ObfDereferenceObject(process);

	auto size = input->size;
	auto* buffer = (uint8_t*)input->source;
	auto address = (uint64_t)input->dest;

	while (size) {
		if (!MmIsAddressValid(buffer) || !utils::paging::translate_virtual_to_physical(callers_directory_table_base, (uint64_t)buffer))
			return STATUS_UNSUCCESSFUL;

		auto size_to_write = min(size, PAGE_SIZE);

		auto physical_address = utils::paging::translate_virtual_to_physical(directory_table_base, address);
		if (!physical_address)
			return STATUS_UNSUCCESSFUL;

		size_t bytes_transfered = 0;
		if (!utils::physical_memory::write(buffer, (void*)physical_address, size_to_write,
			&bytes_transfered) || size_to_write != bytes_transfered)
			return STATUS_UNSUCCESSFUL;

		size -= size_to_write;
		buffer += size_to_write;
		address += size_to_write;
	}

	return STATUS_SUCCESS;
}

NTSTATUS handler::query_process_data(query_process_data_packet* input) {
	if (!MmIsAddressValid(input))
		return STATUS_ACCESS_VIOLATION;

	PEPROCESS process = nullptr;
	if (!NT_SUCCESS(PsLookupProcessByProcessId(ULongToHandle(input->process_id), &process)))
		return STATUS_UNSUCCESSFUL;

	input->peb = PsGetProcessPeb(process);
	input->base_address = PsGetProcessSectionBaseAddress(process);
	input->cr3 = process->DirectoryTableBase;

	ObfDereferenceObject(process);

	return STATUS_SUCCESS;
}
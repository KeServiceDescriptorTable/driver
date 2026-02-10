#include <r69-driver/r69_driver.hpp>

c_r69::~c_r69() {
	detach();
}

bool c_r69::attach(uint32_t process_id) {
	return query_process_data(process_id, &m_process_data);
}

bool c_r69::attach(std::wstring process_name) {
	return attach(get_process_id(process_name));
}

void c_r69::detach() {
	m_process_data = {};
}

bool c_r69::read(uint64_t src, void* dest, uint64_t size) {
	if (!src || !dest || !size)
		return false;

	copy_process_memory_packet input = {};
	input.process_id = m_process_data.process_id;
	input.source = src;
	input.dest = dest;
	input.size = size;

	auto packet = c_packet(e_syscall::read_process_memory, &input, sizeof(input));

	return NT_SUCCESS(issue_syscall(&packet));
}

bool c_r69::write(void* src, uint64_t dest, uint64_t size) {
	if (!src || !dest || !size)
		return false;

	copy_process_memory_packet input = {};
	input.process_id = m_process_data.process_id;
	input.source = (uint64_t)src;
	input.dest = (void*)dest;
	input.size = size;

	auto packet = c_packet(e_syscall::write_process_memory, &input, sizeof(input));

	return NT_SUCCESS(issue_syscall(&packet));
}

bool c_r69::query_process_data(uint32_t process_id, query_process_data_packet* output_data) {
	if (!process_id || !output_data)
		return false;

	output_data->process_id = process_id;

	auto packet = c_packet(e_syscall::query_process_data, output_data, sizeof(query_process_data_packet));

	return NT_SUCCESS(issue_syscall(&packet));
}

const query_process_data_packet* c_r69::get_attached_process() const {
	return &m_process_data;
}

uint32_t c_r69::get_process_id(std::wstring process_name) const {
	auto snapshot_handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (snapshot_handle == INVALID_HANDLE_VALUE)
		return 0;

	PROCESSENTRY32W process_entry = { sizeof(process_entry) };
	if (!Process32FirstW(snapshot_handle, &process_entry)) {
		CloseHandle(snapshot_handle);
		return 0;
	}

	while (Process32NextW(snapshot_handle, &process_entry)) {
		if (std::wstring(process_entry.szExeFile) != process_name)
			continue;

		CloseHandle(snapshot_handle);
		return process_entry.th32ProcessID;
	}

	CloseHandle(snapshot_handle);
	return 0;
}

NTSTATUS c_r69::issue_syscall(c_packet* packet) {
	if (!packet)
		return STATUS_ACCESS_VIOLATION;

	if (!m_NtQueryAuxiliaryCounterFrequency) {
		auto* ntdll = GetModuleHandleA("ntdll.dll");
		m_NtQueryAuxiliaryCounterFrequency = (NTSTATUS(*)(uint64_t*, c_packet*))GetProcAddress(ntdll, "NtQueryAuxiliaryCounterFrequency");
	}

	uint64_t x = 0;
	return m_NtQueryAuxiliaryCounterFrequency(&x, packet);
}
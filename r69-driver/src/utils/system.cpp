#include <utils/system.hpp>

utils::kmodule::kmodule(PKLDR_DATA_TABLE_ENTRY data_table_entry) {
	m_data_table_entry = data_table_entry;
}

void* utils::kmodule::image_base() const {
	if (!valid())
		return nullptr;

	return m_data_table_entry->DllBase;
}

uint64_t utils::kmodule::image_size() const {
	if (!valid())
		return 0;

	return m_data_table_entry->SizeOfImage;
}

bool utils::kmodule::valid() const {
	if (!MmIsAddressValid(m_data_table_entry))
		return false;

	return MmIsAddressValid(m_data_table_entry->DllBase);
}

PKLDR_DATA_TABLE_ENTRY utils::kmodule::get_data_table_entry() const {
	if (!valid())
		return nullptr;

	return m_data_table_entry;
}

PKLDR_DATA_TABLE_ENTRY utils::system::get_module_entry(const wchar_t* image_name) {
	auto* list_entry = PsLoadedModuleList->Flink;

	while (list_entry != PsLoadedModuleList) {
		auto* entry = CONTAINING_RECORD(list_entry, KLDR_DATA_TABLE_ENTRY, InLoadOrderLinks);
		
		if (!wcscmp(image_name, entry->BaseDllName.Buffer))
			return entry;

		list_entry = list_entry->Flink;
	}
	
	return nullptr;
}

uint64_t utils::system::pattern_scan(const wchar_t* image_name, const char* pattern_string, const char* pattern_mask) {
	if (!image_name || !pattern_string || !pattern_mask)
		return 0;

	auto image = kmodule(get_module_entry(image_name));
	if (!image.valid())
		return 0;

	const uint64_t start = (uint64_t)image.image_base();
	const uint64_t end = start + image.image_size();

	const auto check_mask = [](const char* base, const char* pattern, const char* mask) -> bool {
		for (; *mask; ++base, ++pattern, ++mask)
			if (*mask == 'x' && *base != *pattern)
				return false;
		return true;
	};
	
	for (auto address = start; address < end; address++) {
		if (!MmIsAddressValid((void*)address))
			break;

		if (!check_mask((const char*)address, pattern_string, pattern_mask))
			continue;

		return address;
	}

	return 0;
}

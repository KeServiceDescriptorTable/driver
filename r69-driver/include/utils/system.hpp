#include <framework.hpp>

namespace utils {
	class kmodule {
	public:
		kmodule(PKLDR_DATA_TABLE_ENTRY data_table_entry);
		~kmodule() = default;

		void* image_base() const;

		uint64_t image_size() const;

		bool valid() const;

		PKLDR_DATA_TABLE_ENTRY get_data_table_entry() const;
	private:
		PKLDR_DATA_TABLE_ENTRY m_data_table_entry = nullptr;
	};

	class system {
	public:
		system() = default;
		~system() = default;

		static PKLDR_DATA_TABLE_ENTRY get_module_entry(const wchar_t* image_name);

		static uint64_t pattern_scan(const wchar_t* image_name, const char* pattern_string, const char* pattern_mask);
	};
}
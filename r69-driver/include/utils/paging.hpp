#ifndef paging_hpp
#define paging_hpp

#include <utils/memory.hpp>

namespace utils {
	class paging {
	public:
		paging() = default;
		~paging() = default;

		static constexpr auto page_4kb_size = 0x1000ull;
		static constexpr auto page_2mb_size = 0x200000ull;
		static constexpr auto page_1gb_size = 0x40000000ull;

		static constexpr auto page_shift = 12ull;
		static constexpr auto page_2mb_shift = 21ull;
		static constexpr auto page_1gb_shift = 30ull;

		static constexpr auto page_4kb_mask = 0xFFFull;
		static constexpr auto page_2mb_mask = 0x1FFFFFull;
		static constexpr auto page_1gb_mask = 0x3FFFFFFFull;

		static uint64_t translate_virtual_to_physical(uint64_t directory_table_base, uint64_t address);
	};
}

#endif // !paging_hpp
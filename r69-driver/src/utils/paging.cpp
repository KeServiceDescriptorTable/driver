#include <utils/paging.hpp>

uint64_t utils::paging::translate_virtual_to_physical(uint64_t directory_table_base, uint64_t address) {
	if (!directory_table_base || !address)
		return 0;

	cr3 cr3 = { .flags = directory_table_base };
	virtual_address_t virtual_address = { .flags = address };

	const auto pml4e = utils::physical_memory::read<pml4e_64>((void*)((cr3.address_of_page_directory << page_shift) +
		(virtual_address.pml4_index) * sizeof(pml4e_64)));

	if (!pml4e.present)
		return 0;

	const auto pdpte = utils::physical_memory::read<pdpte_64>((void*)((pml4e.page_frame_number << page_shift)
		+ (virtual_address.pdpt_index * sizeof(pdpte_64))));

	if (!pdpte.present)
		return 0;

	if (pdpte.large_page) {
		const auto* pdpte_1gb = (const pdpte_1gb_64*)(&pdpte);
		return (pdpte_1gb->page_frame_number << page_shift) + (address & page_1gb_mask);
	}

	const auto pde = utils::physical_memory::read<pde_64>((void*)((pdpte.page_frame_number << page_shift)
		+ (virtual_address.pd_index * sizeof(pde_64))));

	if (!pde.present)
		return 0;

	if (pde.large_page) {
		const auto* pde_2mb = (const pde_2mb_64*)(&pde);
		return (pde_2mb->page_frame_number << page_shift) + (address & page_2mb_mask);
	}

	const auto pte = utils::physical_memory::read<pte_64>((void*)((pde.page_frame_number << page_shift)
		+ (virtual_address.pt_index * sizeof(pte_64))));

	if (!pte.present)
		return 0;

	return (pte.page_frame_number << page_shift) + (address & page_4kb_mask);
}
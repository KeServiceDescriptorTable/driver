#include <utils/memory.hpp>

bool utils::physical_memory::read(void* src, void* dst, const size_t size,
    size_t* bytes_transferred) {
    if (!src || !dst || !size)
        return false;

    size_t temp_bytes_transferred = 0;

    MM_COPY_ADDRESS mm_copy_address = {};
    mm_copy_address.PhysicalAddress.QuadPart = (uint64_t)src;
    const auto status = NT_SUCCESS(MmCopyMemory(dst, mm_copy_address, size,
        MM_COPY_MEMORY_PHYSICAL,
        &temp_bytes_transferred));

    if (bytes_transferred)
        *bytes_transferred = temp_bytes_transferred;

    return status;
}

bool utils::physical_memory::write(void* src, void* dst, const size_t size,
    size_t* bytes_transferred) {
    if (!src || !dst || !size)
        return false;

    PHYSICAL_ADDRESS physical_address = {};
    physical_address.QuadPart = (uint64_t)dst;

    auto* mapped_memory = MmMapIoSpaceEx(physical_address, size, PAGE_READWRITE);
    if (!mapped_memory)
        return false;

    if (memcpy(mapped_memory, src, size) != mapped_memory) {
        MmUnmapIoSpace(mapped_memory, size);
        return false;
    }

    MmUnmapIoSpace(mapped_memory, size);

    if (bytes_transferred)
        *bytes_transferred = size;

    return true;
}

bool utils::physical_memory::read(void* src, void* dst, const size_t size) {
    return read(src, dst, size, nullptr);
}

bool utils::physical_memory::write(void* src, void* dst, const size_t size) {
    return write(src, dst, size, 
        nullptr);
}

void* utils::physical_memory::get_physical_for_virtual(void* virtual_address) {
    return (void*)MmGetPhysicalAddress(virtual_address).QuadPart;
}

void* utils::physical_memory::get_virtual_for_physical(void* physical_address) {
    return MmGetVirtualForPhysical(PHYSICAL_ADDRESS{ .QuadPart = (int64_t)physical_address});
}

bool utils::virtual_memory::read(void* src, void* dst, const size_t size,
    size_t* bytes_transferred) {
    if (!src || !dst || !size)
        return false;

    MM_COPY_ADDRESS mm_copy_address = {};
    mm_copy_address.VirtualAddress = src;
    return NT_SUCCESS(MmCopyMemory(dst, mm_copy_address, size,
        MM_COPY_MEMORY_VIRTUAL,
        bytes_transferred));
}

bool utils::virtual_memory::write(void* src, void* dst, const size_t size,
    size_t* bytes_transferred) {
    if (!src || !dst || !size)
        return false;

    MM_COPY_ADDRESS mm_copy_address = {};
    mm_copy_address.VirtualAddress = src;
    return NT_SUCCESS(MmCopyMemory(dst, mm_copy_address, size,
        MM_COPY_MEMORY_VIRTUAL,
        bytes_transferred));
}

bool utils::virtual_memory::read(void* src, void* dst, const size_t size) {
    uint64_t bytes_transfered = 0;
    return read(src, dst, size, &bytes_transfered);
}

bool utils::virtual_memory::write(void* src, void* dst, const size_t size) {
    uint64_t bytes_transfered = 0;
    return write(src, dst, size, &bytes_transfered);
}
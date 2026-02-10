#ifndef communication_types_hpp
#define communication_types_hpp

#include <framework.hpp>

enum class e_syscall {
	null = 0,
	read_process_memory,
	write_process_memory,
	query_process_data
};

class c_packet {
public:
	c_packet(e_syscall syscall, void* buffer, uint64_t size) {
		m_buffer = buffer;
		m_size = size;
		m_syscall = syscall;
	}

	~c_packet() = default;

	template <typename T> T* get() const {
		if (!m_buffer || !m_size)
			return nullptr;

		if (sizeof(T) != m_size)
			return nullptr;

		return (T*)m_buffer;
	}

	const e_syscall get_syscall() const {
		return m_syscall;
	}
private:
	void* m_buffer = nullptr;
	uint64_t m_size = 0;
	e_syscall m_syscall = e_syscall::null;
};

typedef struct _copy_process_memory_packet {
	uint32_t process_id = 0;
	uint64_t source = 0;
	void* dest = nullptr;
	uint64_t size = 0;
}copy_process_memory_packet, * pcopy_process_memory_packet;

typedef struct _query_process_data_packet {
	uint32_t process_id = 0;
	void* peb = nullptr;
	void* base_address = nullptr;
	uint64_t cr3 = 0;
}query_process_data_packet, * pquery_process_data_packet;

#endif // !communication_types_hpp
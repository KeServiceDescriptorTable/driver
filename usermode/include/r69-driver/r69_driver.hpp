#ifndef r69_driver_hpp
#define r69_driver_hpp

#include <framework.hpp>
#include <r69-driver/communication_types.hpp>

#define NT_SUCCESS(Status)  (((NTSTATUS)(Status)) >= 0)

class c_r69 {
public:
	c_r69() = default;
	~c_r69();

	bool attach(uint32_t process_id);

	bool attach(std::wstring process_name);

	void detach();

	bool read(uint64_t src, void* dest, uint64_t size);

	bool write(void* src, uint64_t dest, uint64_t size);

	template <typename T> T read(uint64_t src) {
		T buffer = {};
		if (!read(src, &buffer, sizeof(T)))
			return T{};

		return buffer;
	}

	template <typename T> bool write(uint64_t dest, T buffer) {
		return write(&buffer, dest, sizeof(T));
	}

	template <typename T> bool write(uint64_t dest, T& buffer) {
		return write(buffer, dest, sizeof(T));
	}

	bool query_process_data(uint32_t process_id, query_process_data_packet* output_data);

	uint32_t get_process_id(std::wstring process_name) const;

	uint64_t get_process_module_base(std::wstring module_name);

	const query_process_data_packet* get_attached_process() const;
private:
	NTSTATUS issue_syscall(c_packet* packet);

	query_process_data_packet m_process_data = {};
	NTSTATUS(*m_NtQueryAuxiliaryCounterFrequency)(uint64_t*, c_packet*) = nullptr;
};

inline auto r69 = std::make_unique<c_r69>();

#endif // !r69_driver_hpp
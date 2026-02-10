#ifndef handler_hpp
#define handler_hpp

#include <framework.hpp>
#include <communication/communication_types.hpp>
#include <utils/paging.hpp>

class handler {
public:
	handler() = default;
	~handler() = default;

	static NTSTATUS read_process_memory(copy_process_memory_packet* input);

	static NTSTATUS write_process_memory(copy_process_memory_packet* input);

	static NTSTATUS query_process_data(query_process_data_packet* input);
};

#endif // !handler_hpp
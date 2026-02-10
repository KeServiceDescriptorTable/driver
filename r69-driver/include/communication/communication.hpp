#ifndef communication_hpp
#define communication_hpp

#include <framework.hpp>
#include <communication/communication_types.hpp>
#include <communication/handler.hpp>

class communication {
public:
	communication() = default;
	~communication() = default;

	static NTSTATUS communication_handler(PKTRAP_FRAME trap_frame);
};

#endif // !communication_hpp
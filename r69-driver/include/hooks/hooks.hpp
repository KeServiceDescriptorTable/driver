#ifndef hooks_hpp
#define hooks_hpp

#include <framework.hpp>
#include <utils/system.hpp>

namespace hooks {
	bool initialize();

	bool shutdown();

	class hal_timer_query_auxiliary_counter_frequency {
	public:
		hal_timer_query_auxiliary_counter_frequency() = default;
		~hal_timer_query_auxiliary_counter_frequency() = default;

		static bool initialize();

		static bool shutdown();
	private:
		static NTSTATUS hook_handler(uint64_t*);

		using function_t = NTSTATUS(__fastcall*)(uint64_t*);

		static inline function_t o_function = nullptr;
	};

	class win32_syscalls {
	public:
		win32_syscalls() = default;
		~win32_syscalls() = default;

		static bool initialize();

		static bool shutdown();
	private:
		static NTSTATUS hook_handler(void* context, void* arg_1, void* arg_2);

		static inline PEX_CALLBACK_FUNCTION o_callback = nullptr;

		static inline PEX_CALLBACK_ROUTINE_BLOCK ex_callback_routine_block = nullptr;
	};
}

#endif // !hooks_hpp
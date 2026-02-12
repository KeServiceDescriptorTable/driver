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

	class hal_clear_last_branch_record_stack {
	public:
		hal_clear_last_branch_record_stack() = default;
		~hal_clear_last_branch_record_stack() = default;

		static bool initialize();

		static bool shutdown();
	private:
		static uint8_t hook_handler();

		static inline uint32_t* ki_cpu_tracing_flags = nullptr;
		static inline uint8_t (*o_hal_clear_last_branch_record_stack)() = nullptr;
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
#ifndef memory_hpp
#define memory_hpp

#include <framework.hpp>

namespace utils {
	class physical_memory {
	public:
		physical_memory() = default;
		~physical_memory() = default;

		static bool read(void* src, void* dst, const size_t size,
			size_t* bytes_transferred);

		static bool write(void* src, void* dst, const size_t size,
			size_t* bytes_transferred);

		static bool read(void* src, void* dst, const size_t size);

		static bool write(void* src, void* dst, const size_t size);

		static void* get_physical_for_virtual(void* virtual_address);

		static void* get_virtual_for_physical(void* physical_address);

		template <typename T>
		static inline T read(void* src) {
			T buffer = {};
			if (!read(src, &buffer, sizeof(T))) {
				DbgPrint("(+) failed read");
				return T{};
			}

			return buffer;
		}

		template <typename T>
		static inline bool write(void* dst, T value) {
			return write(&value, dst, sizeof(T),
				nullptr);
		}

		template <typename T>
		static inline bool write(void* dst, T& value) {
			return write(&value, dst, sizeof(T),
				nullptr);
		}
	};

	class virtual_memory {
	public:
		virtual_memory() = default;
		~virtual_memory() = default;

		static bool read(void* src, void* dst, const size_t size,
			size_t* bytes_transferred);

		static bool write(void* src, void* dst, const size_t size,
			size_t* bytes_transferred);

		static bool read(void* src, void* dst, const size_t size);

		static bool write(void* src, void* dst, const size_t size);

		template <typename T>
		static inline T read(void* src) {
			T buffer = {};
			if (!read(src, &buffer, sizeof(T),
				nullptr))
				return T{};

			return buffer;
		}

		template <typename T>
		static inline bool write(void* dst, T value) {
			return write(&value, dst, sizeof(T),
				nullptr);
		}

		template <typename T>
		static inline bool write(void* dst, T& value) {
			return write(&value, dst, sizeof(T),
				nullptr);
		}
	};

}

#endif // !memory_hpp
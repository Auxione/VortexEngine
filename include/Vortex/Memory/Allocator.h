#pragma once

namespace Vortex {
	class Allocator {
	protected:
		virtual ~Allocator() = default;

	public:
		virtual void* Allocate(SizeType size) = 0;
		virtual void Deallocate(void* ptr) = 0;

	private:
		inline static Byte* GetBasePtr(void* ptr) {
			return static_cast<Byte*>(ptr) - sizeof(UInt64);
		}
		inline static UInt64& Count(void* ptr) {
			auto* byte_ptr = GetBasePtr(ptr);
			return *reinterpret_cast<UInt64*>(byte_ptr);
		}

	public:
		template<typename T, typename ...Args>
		inline T* New(Args&& ...args) {
			auto alloc_size = sizeof(UInt64) + sizeof(T);
			auto* ptr = Allocate(alloc_size);
			auto* byte_ptr = static_cast<Byte*>(ptr);

			new(byte_ptr) UInt64(1);
			return new(byte_ptr + sizeof(UInt64)) T(std::forward<Args>(args)...);
		}

		template<typename T>
		inline T* NewArray(SizeType count) {
			auto alloc_size = sizeof(UInt64) + sizeof(T) * count;
			auto* ptr = Allocate(alloc_size);
			auto* byte_ptr = static_cast<Byte*>(ptr);

			new(byte_ptr) UInt64(count);
			return new(byte_ptr + sizeof(UInt64)) T[count];
		}

		template<typename T>
		inline void Delete(T* ptr) {
			if (ptr != nullptr) {
				auto count = Count(ptr);
				for (int i = 0; i < count; ++i) {
					ptr[i].~T();
				}

				Deallocate(GetBasePtr(ptr));
			}
		}
	};
}
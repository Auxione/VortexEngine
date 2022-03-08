#pragma once

#include "Vortex/Memory/Memory.h"
#include "Vortex/Memory/HeapAllocator.h"
#include "Vortex/Debug/Assert.h"
#include "Vortex/Containers/Iterator.h"

namespace Vortex {
	template<typename T>
	class Vector {
	public:
		using Type = Vector<T>;

	protected:
		inline virtual SizeType GetNewCapacity(SizeType old_size) {
			if (old_size == 0) {
				return 6;
			} else {
				return old_size * 2;
			}
		}

	public:
		constexpr Vector()
			: m_Size{0}, m_Capacity{6}, m_Array{nullptr} {

			m_Array = reinterpret_cast<T*>(GetHeapAllocator()->Allocate(sizeof(T) * m_Capacity));
		}
		constexpr explicit Vector(SizeType capacity)
			: m_Size{0}, m_Capacity{capacity}, m_Array{nullptr} {
			VORTEX_ASSERT(m_Capacity > 0)

			m_Array = reinterpret_cast<T*>(GetHeapAllocator()->Allocate(sizeof(T) * m_Capacity));
		}
		template<SizeType N>
		constexpr explicit Vector(const T(& data)[N])
			:  m_Size{N}, m_Capacity{N}, m_Array{nullptr} {

			m_Array = reinterpret_cast<T*>(GetHeapAllocator()->Allocate(sizeof(T) * N));
			std::memcpy(m_Array, data, sizeof(T) * N);
		}

		Vector(const T* data, SizeType count)
			: m_Size{count}, m_Capacity{count}, m_Array{nullptr} {
			VORTEX_ASSERT(data != nullptr)

			m_Array = reinterpret_cast<T*>(GetHeapAllocator()->Allocate(sizeof(T) * count));
			for (SizeType i = 0; i < count; ++i) {
				m_Array[i] = data[i];
			}
		}

		constexpr Vector(const Type& other)
			: m_Size{other.m_Size}, m_Capacity{other.m_Capacity}, m_Array{nullptr} {

			m_Array = reinterpret_cast<T*>(GetHeapAllocator()->Allocate(sizeof(T) * m_Capacity));
			for (SizeType i = 0; i < m_Size; ++i) {
				m_Array[i] = other.m_Array[i];
			}
		}
		constexpr Vector(Type&& other) noexcept
			: m_Size{other.m_Size}, m_Capacity{other.m_Capacity}, m_Array{other.m_Array} {

			other.m_Size = 0;
			other.m_Capacity = 0;
			other.m_Array = nullptr;
		}

		~Vector() {
			if (m_Array) {
				for (SizeType i = 0; i < m_Size; ++i) {
					m_Array[i].~T();
				}
				GetHeapAllocator()->Deallocate(m_Array);
			}
		}

	public:
		constexpr bool operator==(const Type& other) const {
			return other.m_Size == m_Size
				&& other.m_Capacity == m_Capacity
				&& other.m_Array == m_Array;
		}

		inline Vector& operator=(const Type& other) {
			if (this == &other) return *this;

			auto new_size = other.m_Size;
			auto new_cap = other.m_Capacity;

			if (m_Array != nullptr) {
				for (SizeType i = 0; i < m_Size; ++i) {
					m_Array[i].~T();
				}
				GetHeapAllocator()->Deallocate(m_Array);
			}

			m_Size = new_size;
			m_Capacity = new_cap;
			m_Array = reinterpret_cast<T*>(GetHeapAllocator()->Allocate(sizeof(T) * new_cap));

			for (SizeType i = 0; i < new_size; ++i) {
				m_Array[i] = other.m_Array[i];
			}

			return *this;
		}

		template<SizeType N>
		inline Vector& operator=(const T(& data)[N]) {
			if (m_Array != nullptr) {
				for (SizeType i = 0; i < m_Size; ++i) {
					m_Array[i].~T();
				}
				GetHeapAllocator()->Deallocate(m_Array);
			}

			m_Size = N;
			m_Capacity = N;
			m_Array = reinterpret_cast<T*>(GetHeapAllocator()->Allocate(sizeof(T) * m_Capacity));
			std::memcpy(m_Array, data, sizeof(T) * m_Size);

			return *this;
		}

		inline Vector& operator=(Type&& other) noexcept {
			if (this == &other) return *this;

			auto new_size = other.m_Size;
			auto new_cap = other.m_Capacity;
			auto* buffer = other.m_Array;

			if (m_Array != nullptr) {
				for (SizeType i = 0; i < m_Size; ++i) {
					m_Array[i].~T();
				}
				GetHeapAllocator()->Deallocate(m_Array);
			}

			m_Size = new_size;
			m_Capacity = new_cap;
			m_Array = buffer;

			other.m_Size = 0;
			other.m_Capacity = 0;
			other.m_Array = nullptr;
			return *this;
		}

		constexpr T& operator[](SizeType index) {
			VORTEX_ASSERT(m_Array != nullptr)
			VORTEX_ASSERT_MSG(index < m_Size, "Index out of bounds : %i > %i", index, m_Size)
			return m_Array[index];
		}
		constexpr const T& operator[](SizeType index) const {
			VORTEX_ASSERT(m_Array != nullptr)
			VORTEX_ASSERT_MSG(index < m_Size, "Index out of bounds : %i > %i", index, m_Size)
			return m_Array[index];
		}

	public:
		constexpr T& Front() {
			VORTEX_ASSERT(m_Array != nullptr)
			return m_Array[0];
		}
		constexpr const T& Front() const {
			VORTEX_ASSERT(m_Array != nullptr)
			return m_Array[0];
		}

		constexpr T& Back() {
			VORTEX_ASSERT(m_Array != nullptr)
			return m_Array[m_Size - 1];
		}
		constexpr const T& Back() const {
			VORTEX_ASSERT(m_Array != nullptr)
			return m_Array[m_Size - 1];
		}

		template<typename... Args>
		inline void Emplace(SizeType index, Args&& ...args) {
			VORTEX_ASSERT(index <= m_Size)
			VORTEX_ASSERT(m_Array != nullptr)

			if (index == m_Size) {
				if (m_Size >= m_Capacity) {
					Grow(GetNewCapacity(m_Capacity));
				}
				m_Size++;
			}

			VORTEX_ASSERT(m_Array != nullptr)

			new(m_Array + index)T(std::forward<Args>(args)...);
		}
		template<typename... Args>
		inline void EmplaceBack(Args&& ...args) { Emplace(m_Size, std::forward<Args>(args)...); }

		inline void Push(SizeType index, const T& other) {
			VORTEX_ASSERT(index <= m_Size)
			VORTEX_ASSERT(m_Array != nullptr)

			if (index == m_Size) {
				if (m_Size >= m_Capacity) {
					Grow(GetNewCapacity(m_Capacity));
				}
				m_Size++;
			}

			VORTEX_ASSERT(m_Array != nullptr)

			m_Array[index] = other;
		}
		inline void PushBack(const T& other) { Push(m_Size, other); }

		inline void Erase(SizeType index) {
			VORTEX_ASSERT(index < m_Size)
			VORTEX_ASSERT(m_Array != nullptr)

			m_Array[index].~T();

			if (index != m_Size) {
				auto* pos = m_Array + index;
				std::memcpy(pos, pos + 1, (m_Size - index) * sizeof(T));
			}

			--m_Size;
		}
		inline void Erase(SizeType index, SizeType count) {
			VORTEX_ASSERT(index + count > m_Size)
			VORTEX_ASSERT(m_Array != nullptr)

			for (int i = 0; i < count; ++i) {
				m_Array[index + i].~T();
			}

			if (index != m_Size) {
				auto* pos = m_Array + index;
				std::memcpy(pos, pos + count, (m_Size - (index + count)) * sizeof(T));
			}

			m_Size -= count;
		}
		inline void Pop() { Erase(m_Size); }

	public:
		inline void Clear() {
			VORTEX_ASSERT(m_Array != nullptr)

			if (m_Size > 0) {
				for (SizeType i = 0; i < m_Size; ++i) {
					m_Array[i].~T();
				}
				std::memset(m_Array, 0, m_Size * sizeof(T));
				m_Size = 0;
			}
		}

		inline void Set(const T* data, SizeType count) {
			VORTEX_ASSERT(data != nullptr)

			if (m_Array != nullptr) {
				for (SizeType i = 0; i < m_Size; ++i) {
					m_Array[i].~T();
				}
				GetHeapAllocator()->Deallocate(m_Array);
			}

			m_Array = reinterpret_cast<T*>(GetHeapAllocator()->Allocate(count * sizeof(T)));
			std::memcpy(m_Array, data, count * sizeof(T));

			m_Capacity = count;
			m_Size = count;
		}

		inline void Shrink() {
			VORTEX_ASSERT(m_Array != nullptr)

			if (m_Capacity > m_Size) {
				GetHeapAllocator()->Deallocate(m_Array);
				m_Array = reinterpret_cast<T*>(GetHeapAllocator()->Allocate(m_Size * sizeof(T)));
				m_Capacity = m_Size;
			}
		}

		inline void Grow(SizeType new_cap) {
			VORTEX_ASSERT(m_Array != nullptr)

			if (new_cap > m_Capacity) {
				GetHeapAllocator()->Deallocate(m_Array);
				m_Array = reinterpret_cast<T*>(GetHeapAllocator()->Allocate(new_cap * sizeof(T)));
				m_Capacity = new_cap;
			}
		}

	public:
		using iterator = Iterator<T>;
		using const_iterator = ConstIterator<T>;

	public:
		iterator begin() const { return iterator{m_Array}; }
		iterator end() const { return iterator{m_Array} + m_Size; }
		const_iterator cbegin() const { return const_iterator{m_Array}; }
		const_iterator cend() const { return const_iterator{m_Array} + m_Size; }

	public:
		constexpr SizeType Size() const { return m_Size; }
		constexpr SizeType Capacity() const { return m_Capacity; }
		constexpr T* Data() const { return m_Array; }
		constexpr bool Empty() const { return m_Size == 0; }

	protected:
		SizeType m_Size;
		SizeType m_Capacity;
		T* m_Array;
	};
}
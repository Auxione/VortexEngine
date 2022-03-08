#pragma once
#include "Vortex/Memory/Memory.h"
#include "Vortex/Memory/HeapAllocator.h"
#include "Vortex/Debug/Assert.h"
#include "Vortex/Containers/Iterator.h"

namespace Vortex {
	template<typename T>
	class DynamicArray {
	public:
		inline explicit DynamicArray()
			: m_Capacity{0}, m_Array{nullptr} {
		}

		inline explicit DynamicArray(SizeType capacity)
			: m_Capacity{capacity}, m_Array{nullptr} {
			VORTEX_ASSERT(m_Capacity > 0)

			m_Array = GetHeapAllocator()->NewArray<T>(m_Capacity);
		}

	public:
		template<SizeType N>
		constexpr explicit DynamicArray(const T(& data)[N])
			:   m_Capacity{N}, m_Array{nullptr} {

			m_Array = GetHeapAllocator()->NewArray<T>(m_Capacity);
			std::memcpy(m_Array, data, sizeof(T) * N);
		}

		inline DynamicArray(const T* data, SizeType count)
			: m_Capacity{count}, m_Array{nullptr} {
			VORTEX_ASSERT(data != nullptr)

			m_Array = GetHeapAllocator()->NewArray<T>(m_Capacity);
			for (SizeType i = 0; i < count; ++i) {
				m_Array[i] = data[i];
			}
		}

		inline DynamicArray(const DynamicArray& other)
			: m_Capacity{other.m_Capacity}, m_Array{nullptr} {
			VORTEX_ASSERT(other.m_Array != nullptr)

			m_Array = GetHeapAllocator()->NewArray<T>(m_Capacity);
			for (SizeType i = 0; i < other.m_Capacity; ++i) {
				m_Array[i] = other.m_Array[i];
			}
		}

		inline DynamicArray(DynamicArray&& other) noexcept
			: m_Capacity{other.m_Capacity}, m_Array{other.m_Array} {
			VORTEX_ASSERT(other.m_Array != nullptr)

			other.m_Capacity = 0;
			other.m_Array = nullptr;
		}

	public:
		virtual ~DynamicArray() {
			GetHeapAllocator()->Delete(m_Array);
		}

	public:
		inline DynamicArray& operator=(const DynamicArray& other) {
			if (this == &other) return *this;

			GetHeapAllocator()->Delete(m_Array);

			m_Capacity = other.m_Capacity;
			m_Array = GetHeapAllocator()->NewArray<T>(m_Capacity);

			for (SizeType i = 0; i < other.m_Capacity; ++i) {
				m_Array[i] = other.m_Array[i];
			}

			return *this;
		}

		template<SizeType N>
		inline DynamicArray& operator=(const T(& data)[N]) {
			GetHeapAllocator()->Delete(m_Array);

			m_Capacity = N;
			m_Array = GetHeapAllocator()->NewArray<T>(m_Capacity);

			std::memcpy(m_Array, data, sizeof(T) * m_Capacity);

			return *this;
		}

		inline DynamicArray& operator=(DynamicArray&& other) noexcept {
			if (this == &other) return *this;
			GetHeapAllocator()->Delete(m_Array);

			m_Capacity = other.m_Capacity;
			m_Array = other.m_Array;

			other.m_Capacity = 0;
			other.m_Buffer = nullptr;

			return *this;
		}

		constexpr T& operator[](SizeType index) {
			VORTEX_ASSERT(m_Array != nullptr)
			VORTEX_ASSERT_MSG(index < m_Capacity, "Index out of bounds : %i > %i", index, m_Capacity)
			return m_Array[index];
		}
		constexpr const T& operator[](SizeType index) const {
			VORTEX_ASSERT(m_Array != nullptr)
			VORTEX_ASSERT_MSG(index < m_Capacity, "Index out of bounds : %i > %i", index, m_Capacity)
			return m_Array[index];
		}

	public:
		inline T& Front() {
			VORTEX_ASSERT(m_Array != nullptr)
			return m_Array[0];
		}
		inline const T& Front() const {
			VORTEX_ASSERT(m_Array != nullptr)
			return m_Array[0];
		}

		inline T& Back() {
			VORTEX_ASSERT(m_Array != nullptr)
			return m_Array[m_Capacity - 1];
		}
		inline const T& Back() const {
			VORTEX_ASSERT(m_Array != nullptr)
			return m_Array[m_Capacity - 1];
		}

	public:
		inline void Set(const T* data, SizeType count) {
			VORTEX_ASSERT(data != nullptr)

			GetHeapAllocator()->Delete(m_Array);

			m_Capacity = count;
			m_Array = GetHeapAllocator()->NewArray<T>(m_Capacity);

			for (SizeType i = 0; i < m_Capacity; ++i) {
				m_Array[i] = data[i];
			}
		}

	public:
		constexpr SizeType Capacity() const { return m_Capacity; }
		constexpr T* Data() const { return m_Array; }

	public:
		using iterator = Iterator<T>;
		using const_iterator = ConstIterator<T>;

	public:
		iterator begin() const { return iterator{m_Array}; }
		iterator end() const { return iterator{m_Array} + m_Capacity; }
		const_iterator cbegin() const { return const_iterator{m_Array}; }
		const_iterator cend() const { return const_iterator{m_Array} + m_Capacity; }

	protected:
		T* m_Array;
		SizeType m_Capacity;
	};
}
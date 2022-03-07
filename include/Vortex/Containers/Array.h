#pragma once
#include "Vortex/Memory/Memory.h"
#include "Vortex/Memory/HeapAllocator.h"
#include "Vortex/Debug/Assert.h"

namespace Vortex {
	template<typename T>
	class Array {
	public:
		inline explicit Array()
			: m_Capacity{0}, m_Array{nullptr} {
		}

		inline explicit Array(SizeType capacity)
			: m_Capacity{capacity}, m_Array{nullptr} {
			VORTEX_ASSERT(m_Capacity > 0)

			m_Array = GetHeapAllocator()->NewArray<T>(m_Capacity);
		}

	public:
		template<SizeType N>
		constexpr explicit Array(const T(& data)[N])
			:   m_Capacity{N}, m_Array{nullptr} {

			m_Array = GetHeapAllocator()->NewArray<T>(m_Capacity);
			std::memcpy(m_Array, data, sizeof(T) * N);
		}

		inline Array(const T* data, SizeType count)
			: m_Capacity{count}, m_Array{nullptr} {
			VORTEX_ASSERT(data != nullptr)

			m_Array = GetHeapAllocator()->NewArray<T>(m_Capacity);
			for (SizeType i = 0; i < count; ++i) {
				m_Array[i] = data[i];
			}
		}

		inline Array(const Array& other)
			: m_Capacity{other.m_Capacity}, m_Array{nullptr} {
			VORTEX_ASSERT(other.m_Array != nullptr)

			m_Array = GetHeapAllocator()->NewArray<T>(m_Capacity);
			for (SizeType i = 0; i < other.m_Capacity; ++i) {
				m_Array[i] = other.m_Array[i];
			}
		}

		inline Array(Array&& other) noexcept
			: m_Capacity{other.m_Capacity}, m_Array{other.m_Array} {
			VORTEX_ASSERT(other.m_Array != nullptr)

			other.m_Capacity = 0;
			other.m_Array = nullptr;
		}

	public:
		~Array() {
			GetHeapAllocator()->Delete(m_Array);
		}

	public:
		inline Array& operator=(const Array& other) {
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
		inline Array& operator=(const T(& data)[N]) {
			GetHeapAllocator()->Delete(m_Array);

			m_Capacity = N;
			m_Array = GetHeapAllocator()->NewArray<T>(m_Capacity);

			std::memcpy(m_Array, data, sizeof(T) * m_Capacity);

			return *this;
		}

		inline Array& operator=(Array&& other) noexcept {
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
		struct Iterator {
			using difference_type = ptrdiff_t;
			using value_type = T;
			using pointer = T*;
			using reference = T&;
			using iterator_category = std::random_access_iterator_tag;

		public:
			constexpr explicit Iterator(T* ptr): m_ptr(ptr) {}
			constexpr Iterator(const Iterator&) = default;
			constexpr Iterator(Iterator&&) noexcept = default;
			virtual ~Iterator() = default;

		public:
			constexpr Iterator& operator=(const Iterator&) = default;
			constexpr Iterator& operator=(Iterator&&) noexcept = default;

		public:
			constexpr Iterator& operator++() {
				this->m_ptr++;
				return *this;
			}
			constexpr const Iterator operator++(int) {
				Iterator tmp{*this};
				tmp.m_ptr++;
				return tmp;
			}
			constexpr Iterator& operator+=(int offset) {
				m_ptr += offset;
				return *this;
			}
			constexpr Iterator& operator-=(int offset) {
				m_ptr -= offset;
				return *this;
			}
			constexpr friend Iterator& operator+=(int offset, Iterator& it) { return it += offset; }
			constexpr friend Iterator& operator-=(int offset, Iterator& it) { return it -= offset; }

			constexpr Iterator operator+(int offset) const {
				Iterator tmp{*this};
				tmp += offset;
				return tmp;
			}
			constexpr Iterator& operator-(int offset) const {
				Iterator tmp{*this};
				tmp -= offset;
				return tmp;
			}
			constexpr friend Iterator& operator+(int offset, Iterator& it) { return it + offset; }
			constexpr friend Iterator& operator-(int offset, Iterator& it) { return it - offset; }

			constexpr reference operator[](int offset) { return m_ptr[offset]; }
		public:
			constexpr bool operator==(const Iterator& other) { return m_ptr == other.m_ptr; };
			constexpr bool operator!=(const Iterator& other) { return m_ptr != other.m_ptr; };
			constexpr bool operator>=(const Iterator& other) { return m_ptr >= other.m_ptr; };
			constexpr bool operator<=(const Iterator& other) { return m_ptr <= other.m_ptr; };
			constexpr bool operator>(const Iterator& other) { return m_ptr > other.m_ptr; };
			constexpr bool operator<(const Iterator& other) { return m_ptr < other.m_ptr; };

			reference operator*() const { return *m_ptr; }
			pointer operator->() { return m_ptr; }
			pointer Get() { return m_ptr; }

		protected:
			T* m_ptr;
		};

		struct ConstIterator {
		public:
			using difference_type = ptrdiff_t;
			using value_type = T;
			using pointer = const T*;
			using reference = const T&;
			using iterator_category = std::random_access_iterator_tag;

		public:
			constexpr explicit ConstIterator(T* ptr): m_ptr(ptr) {}
			constexpr ConstIterator(const ConstIterator&) = default;
			constexpr ConstIterator(ConstIterator&&) noexcept = default;
			virtual ~ConstIterator() = default;

		public:
			constexpr ConstIterator& operator=(const ConstIterator&) = default;
			constexpr ConstIterator& operator=(ConstIterator&&) noexcept = default;

		public:
			constexpr ConstIterator& operator++() {
				this->m_ptr++;
				return *this;
			}
			constexpr ConstIterator operator++(int) {
				ConstIterator tmp{*this};
				tmp.m_ptr++;
				return tmp;
			}
			constexpr ConstIterator& operator+=(int offset) {
				m_ptr += offset;
				return *this;
			}
			constexpr ConstIterator& operator-=(int offset) {
				m_ptr -= offset;
				return *this;
			}
			constexpr friend ConstIterator& operator+=(int offset, ConstIterator& it) { return it += offset; }
			constexpr friend ConstIterator& operator-=(int offset, ConstIterator& it) { return it -= offset; }

			constexpr ConstIterator operator+(int offset) const {
				ConstIterator tmp{*this};
				tmp += offset;
				return tmp;
			}
			constexpr ConstIterator& operator-(int offset) const {
				ConstIterator tmp{*this};
				tmp -= offset;
				return tmp;
			}
			constexpr friend ConstIterator& operator+(int offset, ConstIterator& it) { return it + offset; }
			constexpr friend ConstIterator& operator-(int offset, ConstIterator& it) { return it - offset; }

			constexpr reference operator[](int offset) const { return m_ptr[offset]; }
		public:
			constexpr bool operator==(const ConstIterator& other) { return m_ptr == other.m_ptr; };
			constexpr bool operator!=(const ConstIterator& other) { return m_ptr != other.m_ptr; };
			constexpr bool operator>=(const ConstIterator& other) { return m_ptr >= other.m_ptr; };
			constexpr bool operator<=(const ConstIterator& other) { return m_ptr <= other.m_ptr; };
			constexpr bool operator>(const ConstIterator& other) { return m_ptr > other.m_ptr; };
			constexpr bool operator<(const ConstIterator& other) { return m_ptr < other.m_ptr; };

			reference operator*() const { return *m_ptr; }
			pointer operator->() const { return m_ptr; }

		protected:
			mutable T* m_ptr;
		};

	public:
		using iterator = Iterator;
		using const_iterator = ConstIterator;

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
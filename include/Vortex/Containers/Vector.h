#pragma once
#include <initializer_list>

#include "Vortex/Memory/Memory.h"
#include "Vortex/Memory/HeapAllocator.h"
#include "Vortex/Debug/Assert.h"

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
			: m_Size{0}, m_Capacity{6}, m_Buffer{nullptr} {

			m_Buffer = reinterpret_cast<T*>(GetHeapAllocator()->Allocate(sizeof(T) * m_Capacity));
		}
		constexpr explicit Vector(SizeType capacity)
			: m_Size{0}, m_Capacity{capacity}, m_Buffer{nullptr} {
			VORTEX_ASSERT(m_Capacity > 0)

			m_Buffer = reinterpret_cast<T*>(GetHeapAllocator()->Allocate(sizeof(T) * m_Capacity));
		}
		template<SizeType N>
		constexpr explicit Vector(const T(& data)[N])
			:  m_Size{N}, m_Capacity{N}, m_Buffer{nullptr} {

			m_Buffer = reinterpret_cast<T*>(GetHeapAllocator()->Allocate(sizeof(T) * N));
			std::memcpy(m_Buffer, data, sizeof(T) * N);
		}

		Vector(const T* data, SizeType count)
			: m_Size{count}, m_Capacity{count}, m_Buffer{nullptr} {
			VORTEX_ASSERT(data != nullptr)

			m_Buffer = reinterpret_cast<T*>(GetHeapAllocator()->Allocate(sizeof(T) * count));
			for (SizeType i = 0; i < count; ++i) {
				m_Buffer[i] = data[i];
			}
		}

		constexpr Vector(const Type& other)
			: m_Size{other.m_Size}, m_Capacity{other.m_Capacity}, m_Buffer{nullptr} {

			m_Buffer = reinterpret_cast<T*>(GetHeapAllocator()->Allocate(sizeof(T) * m_Capacity));
			for (SizeType i = 0; i < m_Size; ++i) {
				m_Buffer[i] = other.m_Buffer[i];
			}
		}
		constexpr Vector(Type&& other) noexcept
			: m_Size{other.m_Size}, m_Capacity{other.m_Capacity}, m_Buffer{other.m_Buffer} {

			other.m_Size = 0;
			other.m_Capacity = 0;
			other.m_Buffer = nullptr;
		}

		~Vector() {
			if (m_Buffer) {
				for (SizeType i = 0; i < m_Size; ++i) {
					m_Buffer[i].~T();
				}
				GetHeapAllocator()->Deallocate(m_Buffer);
			}
		}

	public:
		constexpr bool operator==(const Type& other) const {
			return other.m_Size == m_Size
				&& other.m_Capacity == m_Capacity
				&& other.m_Buffer == m_Buffer;
		}

		inline Vector& operator=(const Type& other) {
			if (this == &other) return *this;

			auto new_size = other.m_Size;
			auto new_cap = other.m_Capacity;

			if (m_Buffer != nullptr) {
				for (SizeType i = 0; i < m_Size; ++i) {
					m_Buffer[i].~T();
				}
				GetHeapAllocator()->Deallocate(m_Buffer);
			}

			m_Size = new_size;
			m_Capacity = new_cap;
			m_Buffer = reinterpret_cast<T*>(GetHeapAllocator()->Allocate(sizeof(T) * new_cap));

			for (SizeType i = 0; i < new_size; ++i) {
				m_Buffer[i] = other.m_Buffer[i];
			}

			return *this;
		}

		template<SizeType N>
		inline Vector& operator=(const T(& data)[N]) {
			if (m_Buffer != nullptr) {
				for (SizeType i = 0; i < m_Size; ++i) {
					m_Buffer[i].~T();
				}
				GetHeapAllocator()->Deallocate(m_Buffer);
			}

			m_Size = N;
			m_Capacity = N;
			m_Buffer = reinterpret_cast<T*>(GetHeapAllocator()->Allocate(sizeof(T) * m_Capacity));
			std::memcpy(m_Buffer, data, sizeof(T) * m_Size);

			return *this;
		}

		inline Vector& operator=(Type&& other) noexcept {
			if (this == &other) return *this;

			auto new_size = other.m_Size;
			auto new_cap = other.m_Capacity;
			auto* buffer = other.m_Buffer;

			if (m_Buffer != nullptr) {
				for (SizeType i = 0; i < m_Size; ++i) {
					m_Buffer[i].~T();
				}
				GetHeapAllocator()->Deallocate(m_Buffer);
			}

			m_Size = new_size;
			m_Capacity = new_cap;
			m_Buffer = buffer;

			other.m_Size = 0;
			other.m_Capacity = 0;
			other.m_Buffer = nullptr;
			return *this;
		}

		constexpr T& operator[](SizeType index) {
			VORTEX_ASSERT(m_Buffer != nullptr)
			VORTEX_ASSERT_MSG(index < m_Size, "Index out of bounds : %i > %i", index, m_Size)
			return m_Buffer[index];
		}
		constexpr const T& operator[](SizeType index) const {
			VORTEX_ASSERT(m_Buffer != nullptr)
			VORTEX_ASSERT_MSG(index < m_Size, "Index out of bounds : %i > %i", index, m_Size)
			return m_Buffer[index];
		}

	public:
		constexpr T& Front() {
			VORTEX_ASSERT(m_Buffer != nullptr)
			return m_Buffer[0];
		}
		constexpr const T& Front() const {
			VORTEX_ASSERT(m_Buffer != nullptr)
			return m_Buffer[0];
		}

		constexpr T& Back() {
			VORTEX_ASSERT(m_Buffer != nullptr)
			return m_Buffer[m_Size - 1];
		}
		constexpr const T& Back() const {
			VORTEX_ASSERT(m_Buffer != nullptr)
			return m_Buffer[m_Size - 1];
		}

		template<typename... Args>
		inline void Emplace(SizeType index, Args&& ...args) {
			VORTEX_ASSERT(index <= m_Size)
			VORTEX_ASSERT(m_Buffer != nullptr)

			if (index == m_Size) {
				if (m_Size >= m_Capacity) {
					Grow(GetNewCapacity(m_Capacity));
				}
				m_Size++;
			}

			VORTEX_ASSERT(m_Buffer != nullptr)

			new(m_Buffer + index)T(std::forward<Args>(args)...);
		}
		template<typename... Args>
		inline void EmplaceBack(Args&& ...args) { Emplace(m_Size, std::forward<Args>(args)...); }

		inline void Push(SizeType index, const T& other) {
			VORTEX_ASSERT(index <= m_Size)
			VORTEX_ASSERT(m_Buffer != nullptr)

			if (index == m_Size) {
				if (m_Size >= m_Capacity) {
					Grow(GetNewCapacity(m_Capacity));
				}
				m_Size++;
			}

			VORTEX_ASSERT(m_Buffer != nullptr)

			m_Buffer[index] = other;
		}
		inline void PushBack(const T& other) { Push(m_Size, other); }

		inline void Erase(SizeType index) {
			VORTEX_ASSERT(index < m_Size)
			VORTEX_ASSERT(m_Buffer != nullptr)

			m_Buffer[index].~T();

			if (index != m_Size) {
				auto* pos = m_Buffer + index;
				std::memcpy(pos, pos + 1, (m_Size - index) * sizeof(T));
			}

			--m_Size;
		}
		inline void Erase(SizeType index, SizeType count) {
			VORTEX_ASSERT(index + count > m_Size)
			VORTEX_ASSERT(m_Buffer != nullptr)

			for (int i = 0; i < count; ++i) {
				m_Buffer[index + i].~T();
			}

			if (index != m_Size) {
				auto* pos = m_Buffer + index;
				std::memcpy(pos, pos + count, (m_Size - (index + count)) * sizeof(T));
			}

			m_Size -= count;
		}
		inline void Pop() { Erase(m_Size); }

	public:
		inline void Clear() {
			VORTEX_ASSERT(m_Buffer != nullptr)

			if (m_Size > 0) {
				for (SizeType i = 0; i < m_Size; ++i) {
					m_Buffer[i].~T();
				}
				std::memset(m_Buffer, 0, m_Size * sizeof(T));
				m_Size = 0;
			}
		}

		inline void Set(const T* data, SizeType count) {
			VORTEX_ASSERT(data != nullptr)

			if (m_Buffer != nullptr) {
				for (SizeType i = 0; i < m_Size; ++i) {
					m_Buffer[i].~T();
				}
				GetHeapAllocator()->Deallocate(m_Buffer);
			}

			m_Buffer = reinterpret_cast<T*>(GetHeapAllocator()->Allocate(count * sizeof(T)));
			std::memcpy(m_Buffer, data, count * sizeof(T));

			m_Capacity = count;
			m_Size = count;
		}

		inline void Shrink() {
			VORTEX_ASSERT(m_Buffer != nullptr)

			if (m_Capacity > m_Size) {
				GetHeapAllocator()->Deallocate(m_Buffer);
				m_Buffer = reinterpret_cast<T*>(GetHeapAllocator()->Allocate(m_Size * sizeof(T)));
				m_Capacity = m_Size;
			}
		}

		inline void Grow(SizeType new_cap) {
			VORTEX_ASSERT(m_Buffer != nullptr)

			if (new_cap > m_Capacity) {
				GetHeapAllocator()->Deallocate(m_Buffer);
				m_Buffer = reinterpret_cast<T*>(GetHeapAllocator()->Allocate(new_cap * sizeof(T)));
				m_Capacity = new_cap;
			}
		}

	public:
		constexpr SizeType Size() const { return m_Size; }
		constexpr SizeType Capacity() const { return m_Capacity; }
		constexpr T* Data() const { return m_Buffer; }
		constexpr bool Empty() const { return m_Size == 0; }

	protected:
		SizeType m_Size;
		SizeType m_Capacity;
		T* m_Buffer;

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
		iterator begin() const { return iterator{m_Buffer}; }
		iterator end() const { return iterator{m_Buffer} + m_Size; }
		const_iterator cbegin() const { return const_iterator{m_Buffer}; }
		const_iterator cend() const { return const_iterator{m_Buffer} + m_Size; }
	};
}
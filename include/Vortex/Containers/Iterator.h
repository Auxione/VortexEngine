#pragma once
#include <iterator>

namespace Vortex {
	template<typename T>
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

	template<typename T>
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
}
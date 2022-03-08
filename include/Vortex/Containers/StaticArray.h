#pragma once
#include "Vortex/Memory/Memory.h"
#include "Vortex/Debug/Assert.h"
#include "Vortex/Containers/Iterator.h"

namespace Vortex {
	template<typename T, SizeType Cap>
	class StaticArray {
	public:
		inline explicit StaticArray()
			: m_Array{} {}

	public:
		template<SizeType N>
		constexpr explicit StaticArray(const T(& data)[N])
			: m_Array{} {
			std::memcpy(m_Array, data, sizeof(T) * N);
		}

		inline StaticArray(const T* data, SizeType count)
			: m_Array{} {
			VORTEX_ASSERT(data != nullptr)
			VORTEX_ASSERT(count <= Cap)

			for (SizeType i = 0; i < count; ++i) {
				m_Array[i] = data[i];
			}
		}

		inline StaticArray(const StaticArray& other)
			: m_Array{} {
			for (SizeType i = 0; i < Cap; ++i) {
				m_Array[i] = other.m_Array[i];
			}
		}

		inline StaticArray(StaticArray&& other) noexcept
			: m_Array{} {
			for (SizeType i = 0; i < Cap; ++i) {
				m_Array[i] = other.m_Array[i];
			}
		}

	public:
		virtual ~StaticArray() = default;

	public:
		inline StaticArray& operator=(const StaticArray& other) {
			if (this == &other) return *this;

			for (SizeType i = 0; i < Cap; ++i) {
				m_Array[i] = other.m_Array[i];
			}

			return *this;
		}

		template<SizeType N>
		inline StaticArray& operator=(const T(& data)[N]) {
			std::memcpy(m_Array, data, N * sizeof(T));

			return *this;
		}

		inline StaticArray& operator=(StaticArray&& other) noexcept {
			if (this == &other) return *this;

			for (SizeType i = 0; i < Cap; ++i) {
				m_Array[i] = other.m_Array[i];
			}

			return *this;
		}

		constexpr T& operator[](SizeType index) {
			VORTEX_ASSERT_MSG(index < Cap, "Index out of bounds : %i > %i", index, Cap)
			return m_Array[index];
		}
		constexpr const T& operator[](SizeType index) const {
			VORTEX_ASSERT_MSG(index < Cap, "Index out of bounds : %i > %i", index, Cap)
			return m_Array[index];
		}

	public:
		inline T& Front() {
			return m_Array[0];
		}
		inline const T& Front() const {
			return m_Array[0];
		}

		inline T& Back() {
			return m_Array[Cap - 1];
		}
		inline const T& Back() const {
			return m_Array[Cap - 1];
		}

	public:
		inline void Set(const T* data, SizeType count) {
			VORTEX_ASSERT(data != nullptr)
			VORTEX_ASSERT(count <= Cap)

			for (SizeType i = 0; i < count; ++i) {
				m_Array[i] = data[i];
			}
		}

	public:
		using iterator = Iterator<T>;
		using const_iterator = ConstIterator<T>;

	public:
		iterator begin() const { return iterator{m_Array}; }
		iterator end() const { return iterator{m_Array} + Cap; }
		const_iterator cbegin() const { return const_iterator{m_Array}; }
		const_iterator cend() const { return const_iterator{m_Array} + Cap; }

	public:
		constexpr SizeType Capacity() const { return Cap; }
		constexpr T* Data() const { return m_Array; }

	protected:
		T m_Array[Cap];
	};
}
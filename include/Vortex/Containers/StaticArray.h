#pragma once
#include "Vortex/Memory/Memory.h"
#include "Vortex/Debug/Assert.h"
#include "Vortex/Containers/Iterator.h"

namespace Vortex {
	template<typename T, SizeType Cap>
	class StaticArray {
	public:
		constexpr explicit StaticArray() = default;

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

		constexpr StaticArray(const StaticArray& other) = default;
		constexpr StaticArray(StaticArray&& other) noexcept = default;

	public:
		virtual ~StaticArray() = default;

	public:
		constexpr StaticArray& operator=(const StaticArray& other) = default;
		constexpr StaticArray& operator=(StaticArray&& other) noexcept = default;

		template<SizeType N>
		constexpr StaticArray& operator=(const T(& data)[N]) {
			std::memcpy(m_Array, data, N * sizeof(T));

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
		constexpr T& Front() { return m_Array[0]; }
		constexpr const T& Front() const { return m_Array[0]; }

		constexpr T& Back() { return m_Array[Cap - 1]; }
		constexpr const T& Back() const { return m_Array[Cap - 1]; }

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
		inline iterator begin() const { return iterator{m_Array}; }
		inline iterator end() const { return iterator{m_Array} + Cap; }
		inline const_iterator cbegin() const { return const_iterator{m_Array}; }
		inline const_iterator cend() const { return const_iterator{m_Array} + Cap; }

	public:
		constexpr SizeType Capacity() const { return Cap; }
		constexpr T* Data() const { return m_Array; }

	protected:
		T m_Array[Cap];
	};
}
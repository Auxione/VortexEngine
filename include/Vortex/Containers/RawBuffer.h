#pragma once
#include "Vortex/Memory/Memory.h"
#include "Vortex/Debug/Assert.h"

namespace Vortex {
	template<SizeType Cap>
	class RawBuffer final {
	public:
		constexpr RawBuffer(): m_Index{0}, m_Data{} {};
		constexpr RawBuffer(const RawBuffer&) = default;
		constexpr RawBuffer(RawBuffer&&) noexcept = default;

		RawBuffer& operator=(const RawBuffer&) = default;
		RawBuffer& operator=(RawBuffer&&) noexcept = default;

		~RawBuffer() = default;

	public:
		template<typename T>
		inline void Write(const T& data) {
			auto size = sizeof(T);
			VORTEX_ASSERT(m_Index + size <= Cap)

			std::memcpy(m_Data + m_Index, &data, size);
			m_Index += size;
		}

		template<typename T, SizeType Count>
		inline void Write(const T* data) {
			VORTEX_STATIC_ASSERT(Count > 0)

			auto size = Count * sizeof(T);
			VORTEX_ASSERT(m_Index + size < Cap)

			std::memcpy(m_Data + m_Index, data, size);
			m_Index += size;
		}

		template<typename T>
		constexpr void Read(T& data) {
			auto size = sizeof(T);
			VORTEX_ASSERT(m_Index + size <= Cap)

			std::memcpy(&data, m_Data + m_Index, size);
			m_Index += size;
		}

		template<typename T, SizeType Count>
		constexpr void Read(T* data) {
			VORTEX_STATIC_ASSERT(Count > 0)

			auto size = Count * sizeof(T);
			VORTEX_ASSERT(m_Index + size < Cap)

			std::memcpy(data, m_Data + m_Index, size);
			m_Index += size;
		}

	public:
		inline void Rewind() { m_Index = 0; }
		constexpr SizeType Capacity() const { return Cap; }
		constexpr SizeType Size() const { return m_Index; }

		inline Byte* Data() { return m_Data; }
		inline const Byte* Data() const { return m_Data; }

	protected:
		Byte m_Data[Cap];
		SizeType m_Index;
	};
}
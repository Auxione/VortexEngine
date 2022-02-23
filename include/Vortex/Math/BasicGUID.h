#pragma once
#include "Vortex/Memory/Memory.h"

namespace Vortex {
	template<typename T>
	struct BasicGUID {
		inline operator T() { return m_Data; }

	public:
		BasicGUID(T p1): m_Data{p1} {}
		BasicGUID(const UInt64* values): m_Data{values[0]} {}

	public:
		friend bool operator==(const BasicGUID<T>& rhs, const BasicGUID<T>& lhs) {
			return rhs.m_Data[0] == lhs.m_Data[0];
		}
		friend bool operator!=(const BasicGUID<T>& rhs, const BasicGUID<T>& lhs) {
			return rhs.m_Data[0] != lhs.m_Data[0];
		}

	public:
		T Get() const { return m_Data; }

	protected:
		T m_Data;
	};
}
namespace std {
	template<typename T>
	struct hash<Vortex::BasicGUID<T>> {
		size_t operator()(const Vortex::BasicGUID<T>& id) const noexcept {
			return hash<T>(id);
		}
	};
}
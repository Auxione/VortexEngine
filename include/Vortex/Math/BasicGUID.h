#pragma once
#include "Vortex/Memory/Memory.h"

namespace Vortex {
	template<typename T>
	struct BasicGUID {
		inline operator T() const { return m_Data; }
		inline T Get() const { return m_Data; }

	public:
		BasicGUID(): m_Data{} {}
		BasicGUID(T p1): m_Data{p1} {}

	public:
		friend bool operator==(const BasicGUID<T>& rhs, const BasicGUID<T>& lhs) {
			return rhs.m_Data[0] == lhs.m_Data[0];
		}
		friend bool operator!=(const BasicGUID<T>& rhs, const BasicGUID<T>& lhs) {
			return rhs.m_Data[0] != lhs.m_Data[0];
		}

	public:

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
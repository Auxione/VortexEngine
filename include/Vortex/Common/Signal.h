#pragma once
#include "Vortex/Memory/Memory.h"
#include "Vortex/Containers/Vector.h"

namespace Vortex {
	template<typename ...Args>
	class Signal {
	public:
		typedef void (* ReceiverFn)(Args...);
		using Handle = SizeType;

	public:
		constexpr Handle Connect(const ReceiverFn& function) {
			for (int i = 0; i < m_Connections.Size(); ++i) {
				if (m_Connections[i] == nullptr) {
					m_Connections[i] = function;
					return i;
				}
			}
			m_Connections.EmplaceBack(function);
			return m_Connections.Size() - 1;
		}

		constexpr void Disconnect(Handle handle) {
			m_Connections[handle] = nullptr;
		}

		constexpr void operator()(Args&& ... args) const {
			for (const auto& connection :  m_Connections) {
				if (connection != nullptr) {
					connection(std::forward<Args>(args)...);
				}
			}
		}

		constexpr void Clear() { m_Connections.Clear(); }

		constexpr SizeType Size() const { return m_Connections.Size(); }
		constexpr SizeType Empty() const { return m_Connections.Size() == 0; }
		constexpr operator bool() const { return !Empty(); }

	private:
		Vortex::Vector<ReceiverFn> m_Connections;
	};

}
#pragma once
#include <vector>

#include "Vortex/Memory/Memory.h"

namespace Vortex {
	template<typename ...Args>
	class Signal {
	public:
		typedef void (* ReceiverFn)(Args...);
		using Handle = SizeType;

	public:
		inline Handle Connect(const ReceiverFn& function) {
			for (int i = 0; i < m_Connections.size(); ++i) {
				if (m_Connections[i] == nullptr) {
					m_Connections[i] = function;
					return i;
				}
			}
			m_Connections.emplace_back(function);
			return m_Connections.size() - 1;
		}

		inline void Disconnect(Handle handle) {
			m_Connections[handle] = nullptr;
		}

		inline void operator()(Args&& ... args) const {
			for (const auto& connection :  m_Connections) {
				if (connection != nullptr) {
					connection(std::forward<Args>(args)...);
				}
			}
		}

		inline void Clear() { m_Connections.Clear(); }

		inline SizeType Size() const { return m_Connections.Size(); }
		inline SizeType Empty() const { return m_Connections.Size() == 0; }
		inline operator bool() const { return !Empty(); }

	private:
		std::vector<ReceiverFn> m_Connections;
	};

}
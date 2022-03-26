#pragma once
#include <queue>

#include "Vortex/Core/Event.h"
#include "Vortex/Common/Timer.h"

namespace Vortex {
	class Application {
	public:
		virtual void OnStart() = 0;
		virtual void OnUpdate(float dt) = 0;
		virtual void OnStop() = 0;
		virtual void OnEvent(Event& event) = 0;

	public:
		void Run();

	public:
		inline static void Quit() { EventCallback(Event::CreateApplicationClose()); }

		inline static void EventCallback(const Event& event) {
			VORTEX_ASSERT(s_Instance != nullptr)
			s_Instance->m_EventQueue.emplace(event);
		}

	protected:
		Application() = default;

		static Application* s_Instance;
		std::queue<Event> m_EventQueue;

	public:
		virtual ~Application() = default;

		Application& operator=(const Application&) = delete;
		Application(const Application&) = delete;
		Application(Application&&) = delete;
		Application& operator=(Application&&) = delete;
	};
}
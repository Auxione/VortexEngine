#include "Vortex/Core/Application.h"
#include "Vortex/Common/Logger.h"

namespace Vortex {
	Application* Application::s_Instance{nullptr};

	void Application::Run() {
		VORTEX_ASSERT(s_Instance == nullptr)
		VORTEX_LOG_INFO("Vortex Engine 0.3");

		s_Instance = this;

		OnStart();

		Timer<float> frame_timer;
		bool running = true;

		do {
			frame_timer.Start();

			while (!m_EventQueue.empty()) {
				auto& event = m_EventQueue.front();
				OnEvent(event);
				if (event.Type == EventType::ApplicationClose) {
					running = false;
				}
				m_EventQueue.pop();
			}

			OnUpdate(frame_timer.Get());

			frame_timer.Stop();
		} while (running);

		OnStop();
		s_Instance = nullptr;
	}
}
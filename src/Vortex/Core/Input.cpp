#include "Vortex/Core/Input.h"
#include "Vortex/Math/VortexMath.h"

#include <limits>

namespace Vortex {
	Input::Input():
		m_LastCursorPosition{},
		m_CursorPosition{},
		m_CursorDelta{},
		CursorLimits{
			std::numeric_limits<float>::min()
			, std::numeric_limits<float>::min()
			, std::numeric_limits<float>::max()
			, std::numeric_limits<float>::max()
		},
		CursorSensivity{1.0f, 1.0f},
		CursorInputMode{CursorInputMode::OS},

		m_ScrollDelta{},

		m_MouseButton{},

		m_Keyboard{},
		m_LastChar{'\0'} {
	}

	Input::~Input() = default;

	void Input::Update() {
		if (CursorInputMode == CursorInputMode::Emulated) {
			m_CursorPosition[0] += m_CursorDelta[0] * CursorSensivity[0];
			m_CursorPosition[1] += m_CursorDelta[1] * CursorSensivity[1];

			m_CursorPosition[0] = Math::Clamp(m_CursorPosition[0], CursorLimits[0], CursorLimits[2]);
			m_CursorPosition[1] = Math::Clamp(m_CursorPosition[1], CursorLimits[1], CursorLimits[3]);
		}

		//Reset states
		for (UInt8& i :  m_Keyboard) {
			i = i & ~StateChanged;
		}

		for (UInt8& i : m_MouseButton) {
			i = i & ~StateChanged;
		}

		m_CursorDelta[0] = 0;
		m_CursorDelta[1] = 0;

		m_ScrollDelta[0] = 0;
		m_ScrollDelta[1] = 0;

		m_LastChar = '\0';
	}

	void Input::OnEvent(const Event& event) {
		switch (event.Type) {
			case EventType::None:
			case EventType::MouseEventCount:
			case EventType::WindowResize:
			case EventType::WindowPathDrop:
			case EventType::WindowEventCount:
			case EventType::WindowFocusLost:
			case EventType::WindowFocusGain:
			case EventType::ApplicationClose:
			case EventType::Count:
			case EventType::KeyboardEventCount:break;

			case EventType::KeyPress: {
				m_Keyboard[event.KeyPress.Keycode] = StateDown | StateChanged;
				break;
			}
			case EventType::KeyRelease: {
				m_Keyboard[event.KeyRelease.Keycode] = StateUp | StateChanged;
				break;
			}
			case EventType::KeyRepeat: {
				m_Keyboard[event.KeyRepeat.Keycode] = StateDown | StateChanged;
				break;
			}
			case EventType::CharInput: {
				m_LastChar = event.CharInput.Character;
				break;
			}
			case EventType::MousePress: {
				m_MouseButton[event.MousePress.Button] = StateDown | StateChanged;
				break;
			}
			case EventType::MouseRelease: {
				m_MouseButton[event.MouseRelease.Button] = StateUp | StateChanged;
				break;
			}
			case EventType::MouseMove: {
				m_CursorDelta[0] = event.MouseMove.Delta[0] - m_LastCursorPosition[0];
				m_CursorDelta[1] = event.MouseMove.Delta[1] - m_LastCursorPosition[1];
				m_LastCursorPosition[0] = event.MouseMove.Delta[0];
				m_LastCursorPosition[1] = event.MouseMove.Delta[1];

				if (CursorInputMode == CursorInputMode::OS) {
					m_CursorPosition[0] = Math::Clamp(event.MouseMove.Delta[0], CursorLimits[0], CursorLimits[2]);
					m_CursorPosition[1] = Math::Clamp(event.MouseMove.Delta[1], CursorLimits[1], CursorLimits[3]);
				}
				break;
			}
			case EventType::ScrollChange: {
				m_ScrollDelta[0] = event.ScrollChange.Delta[0];
				m_ScrollDelta[1] = event.ScrollChange.Delta[1];
				break;
			}
		}
	}
}
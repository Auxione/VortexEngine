#pragma once
#include "Vortex/Core/Keycode.h"
#include "Vortex/Core/Event.h"

namespace Vortex {

	template<typename T>
	class Actor {};

	namespace CursorInputMode {
		enum Enum {
			OS,
			Emulated
		};
	}

	class Input {
	public:
		Input();
		~Input();

	public:
		void Update();
		void OnEvent(const Event& event);

		template<typename T>
		inline void Control(float dt, T& obj) {
			Actor<T> actor;
			actor(*this, dt, obj);
		}

	public: // cursor
		float CursorSensivity[2];
		float CursorLimits[4];
		CursorInputMode::Enum CursorInputMode;

		const float* GetCursorPosition() const { return m_CursorPosition; };
		const float* GetCursorDelta() const { return m_CursorDelta; };
		const float* GetScrollDelta() const { return m_ScrollDelta; };

	public: // mouse & keyboard
		bool ButtonHeld(MouseButton::Enum mouse_button) const { return m_MouseButton[mouse_button] & StateDown; }
		bool ButtonDown(MouseButton::Enum mouse_button) const { return m_MouseButton[mouse_button] == (StateChanged | StateDown); }
		bool ButtonUp(MouseButton::Enum mouse_button) const { return m_MouseButton[mouse_button] == (StateChanged | StateUp); }

		bool ButtonHeld(KeyCode::Enum key_code) const { return m_Keyboard[key_code] & StateDown; }
		bool ButtonDown(KeyCode::Enum key_code) const { return m_Keyboard[key_code] == (StateChanged | StateDown); }
		bool ButtonUp(KeyCode::Enum key_code) const { return m_Keyboard[key_code] == (StateChanged | StateUp); }

		UInt16 LastChar() const { return m_LastChar; }

	protected:
		constexpr static UInt8 StateNormal{0 << 0};
		constexpr static UInt8 StateChanged{1 << 0};
		constexpr static UInt8 StateUp{0 << 1};
		constexpr static UInt8 StateDown{1 << 1};

	protected:
		float m_LastCursorPosition[2];
		float m_CursorPosition[2];
		float m_CursorDelta[2];

		float m_ScrollDelta[2];

		UInt8 m_MouseButton[MouseButton::Count];
		UInt8 m_Keyboard[KeyCode::Count];

		UInt16 m_LastChar;
	};
}
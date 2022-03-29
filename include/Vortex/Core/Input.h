#pragma once
#include "Vortex/Core/Keycode.h"
#include "Vortex/Core/Event.h"
#include "Vortex/Math/VortexMath.h"

namespace Vortex {

	namespace CursorInputMode {
		enum Enum {
			OS,
			Emulated
		};
	}

	class Input {
	public:
		Input();
		virtual ~Input();

	public:
		void Update();
		void OnEvent(const Event& event);

	public: // cursor
		Vector2 CursorSensivity;
		Vector4 CursorLimits;
		CursorInputMode::Enum CursorInputMode;

		[[nodiscard]] const Vector2& GetCursorPosition() const { return m_CursorPosition; };
		[[nodiscard]] const Vector2& GetCursorDelta() const { return m_CursorDelta; };
		[[nodiscard]] const Vector2& GetScrollDelta() const { return m_ScrollDelta; };

	public: // mouse & keyboard
		[[nodiscard]]  bool ButtonHeld(MouseButton::Enum mouse_button) const { return m_MouseButton[mouse_button] & StateDown; }
		[[nodiscard]]  bool ButtonDown(MouseButton::Enum mouse_button) const { return m_MouseButton[mouse_button] == (StateChanged | StateDown); }
		[[nodiscard]]  bool ButtonUp(MouseButton::Enum mouse_button) const { return m_MouseButton[mouse_button] == (StateChanged | StateUp); }

		[[nodiscard]] bool ButtonHeld(KeyCode::Enum key_code) const { return m_Keyboard[key_code] & StateDown; }
		[[nodiscard]] bool ButtonDown(KeyCode::Enum key_code) const { return m_Keyboard[key_code] == (StateChanged | StateDown); }
		[[nodiscard]] bool ButtonUp(KeyCode::Enum key_code) const { return m_Keyboard[key_code] == (StateChanged | StateUp); }

		[[nodiscard]] UInt16 LastChar() const { return m_LastChar; }

	protected:
		constexpr static UInt8 StateNormal{0 << 0};
		constexpr static UInt8 StateChanged{1 << 0};
		constexpr static UInt8 StateUp{0 << 1};
		constexpr static UInt8 StateDown{1 << 1};

	protected:
		Vector2 m_LastCursorPosition;
		Vector2 m_CursorPosition;
		Vector2 m_CursorDelta;

		Vector2 m_ScrollDelta;

		UInt8 m_MouseButton[MouseButton::Count];
		UInt8 m_Keyboard[KeyCode::Count];

		UInt16 m_LastChar;
	};
}
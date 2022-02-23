#pragma once
#include "Vortex/Core/Keycode.h"
#include "Vortex/Memory/Memory.h"

namespace Vortex {
	struct EventType {
		enum Enum {
			None = 0,

			KeyPress,
			KeyRelease,
			KeyRepeat,
			CharInput,

			KeyboardType,

			MousePress,
			MouseRelease,
			MouseMove,
			ScrollChange,

			MouseType,

			WindowResize,
			WindowPathDrop,

			WindowType,

			ApplicationFocusLost,
			ApplicationFocusGain,
			ApplicationClose,

			ApplicationType
		};
		constexpr static const char* ToString[]{
			"None"
			, "KeyPress"
			, "KeyRelease"
			, "KeyRepeat"
			, "CharInput"

			, "KeyboardType"

			, "MousePress"
			, "MouseRelease"
			, "MouseMove"
			, "ScrollChange"

			, "MouseType"

			, "WindowResize"
			, "WindowPathDrop"

			, "WindowType"

			, "ApplicationFocusLost"
			, "ApplicationFocusGain"
			, "ApplicationClose"

			, "ApplicationType"
		};

		constexpr static bool IsKeyboard(EventType::Enum type) { return type < KeyboardType; }
		constexpr static bool IsMouse(EventType::Enum type) { return KeyboardType < type && type < MouseType; }
		constexpr static bool IsWindow(EventType::Enum type) { return MouseType < type && type < WindowType; }
		constexpr static bool IsApplication(EventType::Enum type) { return WindowType < type && type < ApplicationType; }
	};

	struct Event {
		EventType::Enum Type;

		union {
			KeyCode::Enum Keycode;
			UInt16 Character;
			MouseButton::Enum Button;
			float Position[2];
			Int32 Size[2];

			struct {
				SizeType Count;
				const char** Paths;
			} PathDrop;
		};

		inline static Event CreateKeyPress(KeyCode::Enum keycode) {
			Event event{EventType::KeyPress};
			event.Keycode = keycode;
			return event;
		}

		inline static Event CreateKeyRelease(KeyCode::Enum keycode) {
			Event event{EventType::KeyRelease};
			event.Keycode = keycode;
			return event;
		}

		inline static Event CreateKeyRepeat(KeyCode::Enum keycode) {
			Event event{EventType::KeyRepeat};
			event.Keycode = keycode;
			return event;
		}

		inline static Event CreateCharInput(UInt16 character) {
			Event event{EventType::CharInput};
			event.Character = character;
			return event;
		}

		inline static Event CreateMousePress(MouseButton::Enum button) {
			Event event{EventType::MousePress};
			event.Button = button;
			return event;
		}

		inline static Event CreateMouseRelease(MouseButton::Enum button) {
			Event event{EventType::MouseRelease};
			event.Button = button;
			return event;
		}
		inline static Event CreateMouseMove(float x, float y) {
			Event event{EventType::MouseMove};
			event.Position[0] = x;
			event.Position[1] = y;
			return event;
		}
		inline static Event CreateScrollChange(float x, float y) {
			Event event{EventType::ScrollChange};
			event.Position[0] = x;
			event.Position[1] = y;
			return event;
		}

		inline static Event CreateWindowResize(Int32 width, Int32 height) {
			Event event{EventType::WindowResize};
			event.Size[0] = width;
			event.Size[1] = height;
			return event;
		}
		inline static Event CreateWindowPathDrop(std::size_t count, const char** paths) {
			Event event{EventType::WindowPathDrop};
			event.PathDrop.Count = count;
			event.PathDrop.Paths = paths;
			return event;
		}

		inline static Event CreateApplicationFocusGain() {
			return Event{EventType::ApplicationFocusGain};
		}
		inline static Event CreateApplicationFocusLost() {
			return Event{EventType::ApplicationFocusLost};
		}
		inline static Event CreateApplicationClose() {
			return Event{EventType::ApplicationClose};
		}
	};

	typedef void(* EventCallbackFn)(const Event&);
	//using PushEventFn = void(Event);
}
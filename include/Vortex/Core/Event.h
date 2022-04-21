#pragma once
#include "Vortex/Core/Keycode.h"
#include "Vortex/Memory/Memory.h"
#include "Vortex/Math/VortexMath.h"

namespace Vortex {
	struct EventType {
		enum Enum {
			None = 0,

			KeyPress,
			KeyRelease,
			KeyRepeat,
			CharInput,

			KeyboardEventCount,

			MousePress,
			MouseRelease,
			MouseMove,
			ScrollChange,

			MouseEventCount,

			WindowResize,
			WindowPathDrop,
			WindowFocusLost,
			WindowFocusGain,
			WindowClose,

			WindowEventCount,

			ApplicationClose,

			Count
		};
		constexpr static const char* ToString[]{
			"None"
			, "KeyPress"
			, "KeyRelease"
			, "KeyRepeat"
			, "CharInput"

			, "KeyboardEventCount"

			, "MousePress"
			, "MouseRelease"
			, "MouseMove"
			, "ScrollChange"

			, "MouseEventCount"

			, "WindowResize"
			, "WindowPathDrop"
			, "WindowFocusLost"
			, "WindowFocusGain"
			, "WindowClose"

			, "WindowEventCount"

			, "ApplicationClose"

			, "Count"
		};

		constexpr static bool IsKeyboard(EventType::Enum type) { return type < KeyboardEventCount; }
		constexpr static bool IsMouse(EventType::Enum type) { return KeyboardEventCount < type && type < MouseEventCount; }
		constexpr static bool IsInput(EventType::Enum type) { return type < MouseEventCount; }
		constexpr static bool IsWindow(EventType::Enum type) { return MouseEventCount < type && type < WindowEventCount; }
		constexpr static bool IsApplication(EventType::Enum type) { return WindowEventCount < type && type < Count; }
	};

	struct Event {
		EventType::Enum Type;

		union {
			struct {
				UInt16 Handle;
				KeyCode::Enum Keycode;
			} KeyPress;
			struct {
				UInt16 Handle;
				KeyCode::Enum Keycode;
			} KeyRelease;
			struct {
				UInt16 Handle;
				KeyCode::Enum Keycode;
			} KeyRepeat;
			struct {
				UInt16 Handle;
				UInt16 Character;
			} CharInput;

			struct {
				UInt16 Handle;
				MouseButton::Enum Button;
			} MousePress;
			struct {
				UInt16 Handle;
				MouseButton::Enum Button;
			} MouseRelease;
			struct {
				UInt16 Handle;
				Math::Vector2 Delta;
			} MouseMove;
			struct {
				UInt16 Handle;
				Math::Vector2 Delta;
			} ScrollChange;

			struct {
				UInt16 Handle;
				Math::Vector2Int Size;
			} WindowResize;
			struct {
				UInt16 Handle;
				SizeType Count;
				const char** Paths;
			} WindowPathDrop;
			struct {
				UInt16 Handle;
			} WindowFocusGain;
			struct {
				UInt16 Handle;
			} WindowFocusLost;
			struct {
				UInt16 Handle;
			} WindowClose;
		};

		inline static Event CreateKeyPress(UInt16 Handle, KeyCode::Enum keycode) {
			Event event{EventType::KeyPress};
			event.KeyPress.Keycode = keycode;
			event.KeyPress.Handle = Handle;
			return event;
		}

		inline static Event CreateKeyRelease(UInt16 Handle, KeyCode::Enum keycode) {
			Event event{EventType::KeyRelease};
			event.KeyRelease.Keycode = keycode;
			event.KeyRelease.Handle = Handle;
			return event;
		}

		inline static Event CreateKeyRepeat(UInt16 Handle, KeyCode::Enum keycode) {
			Event event{EventType::KeyRepeat};
			event.KeyRepeat.Keycode = keycode;
			event.KeyRepeat.Handle = Handle;
			return event;
		}

		inline static Event CreateCharInput(UInt16 Handle, UInt16 character) {
			Event event{EventType::CharInput};
			event.CharInput.Character = character;
			event.CharInput.Handle = Handle;
			return event;
		}

		inline static Event CreateMousePress(UInt16 Handle, MouseButton::Enum button) {
			Event event{EventType::MousePress};
			event.MousePress.Button = button;
			event.MousePress.Handle = Handle;
			return event;
		}

		inline static Event CreateMouseRelease(UInt16 Handle, MouseButton::Enum button) {
			Event event{EventType::MouseRelease};
			event.MouseRelease.Button = button;
			event.MouseRelease.Handle = Handle;
			return event;
		}
		inline static Event CreateMouseMove(UInt16 Handle, float x, float y) {
			Event event{EventType::MouseMove};
			event.MouseMove.Delta.Set(x, y);
			event.MouseMove.Handle = Handle;
			return event;
		}
		inline static Event CreateScrollChange(UInt16 Handle, float x, float y) {
			Event event{EventType::ScrollChange};
			event.ScrollChange.Delta.Set(x, y);
			event.ScrollChange.Handle = Handle;
			return event;
		}

		inline static Event CreateWindowResize(UInt16 Handle, Int32 width, Int32 height) {
			Event event{EventType::WindowResize};
			event.WindowResize.Handle = Handle;
			event.WindowResize.Size.Set(width, height);
			return event;
		}
		inline static Event CreateWindowPathDrop(UInt16 Handle, std::size_t count, const char** paths) {
			Event event{EventType::WindowPathDrop};
			event.WindowPathDrop.Handle = Handle;
			event.WindowPathDrop.Count = count;
			event.WindowPathDrop.Paths = paths;
			return event;
		}
		inline static Event CreateWindowClose(UInt16 handle) {
			Event event{EventType::WindowClose};
			event.WindowClose.Handle = handle;
			return event;
		}
		inline static Event CreateWindowFocusGain(UInt16 handle) {
			Event event{EventType::WindowFocusGain};
			event.WindowFocusGain.Handle = handle;
			return event;
		}
		inline static Event CreateWindowFocusLost(UInt16 handle) {
			Event event{EventType::WindowFocusLost};
			event.WindowFocusLost.Handle = handle;
			return event;
		}

		inline static Event CreateApplicationClose() {
			return Event{EventType::ApplicationClose};
		}
	};

	using EventCallbackFn = void (*)(const Event&);
}
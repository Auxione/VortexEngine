#pragma once

namespace Vortex {
	struct MouseButton {
		enum Enum {
			Mouse1 = 0,
			Mouse2,
			Mouse3,
			Mouse4,
			Mouse5,

			Count,

			MouseLeft = Mouse1,
			MouseRight = Mouse2,
			MouseMiddle = Mouse3
		};

		constexpr static const char* ToString[]{
			"Mouse1"
			, "Mouse2"
			, "Mouse3"
			, "Mouse4"
			, "Mouse5"

			, "MouseLeft"
			, "MouseRight"
			, "MouseMiddle"
		};
	};
	struct KeyCode {
		enum Enum {
			Space = 32,
			Apostrophe = 39,    /* ' */
			Comma = 44,         /* , */
			Minus = 45,         /* - */
			Period = 46,        /* . */
			Slash = 47,         /* / */

			D0 = 48,            /* 0 */
			D1 = 49,            /* 1 */
			D2 = 50,            /* 2 */
			D3 = 51,            /* 3 */
			D4 = 52,            /* 4 */
			D5 = 53,            /* 5 */
			D6 = 54,            /* 6 */
			D7 = 55,            /* 7 */
			D8 = 56,            /* 8 */
			D9 = 57,            /* 9 */

			Semicolon = 59,        /* ; */
			Equal = 61,        /* = */

			A = 65,
			B = 66,
			C = 67,
			D = 68,
			E = 69,
			F = 70,
			G = 71,
			H = 72,
			I = 73,
			J = 74,
			K = 75,
			L = 76,
			M = 77,
			N = 78,
			O = 79,
			P = 80,
			Q = 81,
			R = 82,
			S = 83,
			T = 84,
			U = 85,
			V = 86,
			W = 87,
			X = 88,
			Y = 89,
			Z = 90,

			LeftBracket = 91,            /* [ */
			Backslash = 92,            /* \ */
			RightBracket = 93,        /* ] */
			GraveAccent = 96,        /* ` */

			World1 = 161, /* non-US #1 */
			World2 = 162, /* non-US #2 */

			/* Function keys */
			Escape = 256,
			Enter = 257,
			Tab = 258,
			Backspace = 259,
			Insert = 260,
			Delete = 261,
			Right = 262,
			Left = 263,
			Down = 264,
			Up = 265,
			PageUp = 266,
			PageDown = 267,
			Home = 268,
			End = 269,
			CapsLock = 280,
			ScrollLock = 281,
			NumLock = 282,
			PrintScreen = 283,
			Pause = 284,
			F1 = 290,
			F2 = 291,
			F3 = 292,
			F4 = 293,
			F5 = 294,
			F6 = 295,
			F7 = 296,
			F8 = 297,
			F9 = 298,
			F10 = 299,
			F11 = 300,
			F12 = 301,
			F13 = 302,
			F14 = 303,
			F15 = 304,
			F16 = 305,
			F17 = 306,
			F18 = 307,
			F19 = 308,
			F20 = 309,
			F21 = 310,
			F22 = 311,
			F23 = 312,
			F24 = 313,
			F25 = 314,

			/* Keypad */
			KP0 = 320,
			KP1 = 321,
			KP2 = 322,
			KP3 = 323,
			KP4 = 324,
			KP5 = 325,
			KP6 = 326,
			KP7 = 327,
			KP8 = 328,
			KP9 = 329,
			KPDecimal = 330,
			KPDivide = 331,
			KPMultiply = 332,
			KPSubtract = 333,
			KPAdd = 334,
			KPEnter = 335,
			KPEqual = 336,

			LeftShift = 340,
			LeftControl = 341,
			LeftAlt = 342,
			LeftSuper = 343,
			RightShift = 344,
			RightControl = 345,
			RightAlt = 346,
			RightSuper = 347,
			Menu = 348,

			Count
		};
		constexpr static const char* ToString[]{
			"Space"
			, "Apostrophe"
			, "Comma"
			, "Minus"
			, "Period"
			, "Slash"

			, "D0"
			, "D1"
			, "D2"
			, "D3"
			, "D4"
			, "D5"
			, "D6"
			, "D7"
			, "D8"
			, "D9"

			, "Semicolon"
			, "Equal"

			, "A"
			, "B"
			, "C"
			, "D"
			, "E"
			, "F"
			, "G"
			, "H"
			, "I"
			, "J"
			, "K"
			, "L"
			, "M"
			, "N"
			, "O"
			, "P"
			, "Q"
			, "R"
			, "S"
			, "T"
			, "U"
			, "V"
			, "W"
			, "X"
			, "Y"
			, "Z"

			, "["           // LeftBracket
			, "\\"         // Backslash */
			, "]"        // RightBracket
			, "`"       // GraveAccent

			, "World1"
			, "World2"

			/* Function keys */
			, "Escape"
			, "Enter"
			, "Tab"
			, "Backspace"
			, "Insert"
			, "Delete"
			, "Right"
			, "Left"
			, "Down"
			, "Up"
			, "PageUp"
			, "PageDown"
			, "Home"
			, "End"
			, "CapsLock"
			, "ScrollLock"
			, "NumLock"
			, "PrintScreen"
			, "Pause"

			, "F1"
			, "F2"
			, "F3"
			, "F4"
			, "F5"
			, "F6"
			, "F7"
			, "F8"
			, "F9"
			, "F10"
			, "F11"
			, "F12"
			, "F13"
			, "F14"
			, "F15"
			, "F16"
			, "F17"
			, "F18"
			, "F19"
			, "F20"
			, "F21"
			, "F22"
			, "F23"
			, "F24"
			, "F25"

			/* Keypad */
			, "Keypad0"
			, "Keypad1"
			, "Keypad2"
			, "Keypad3"
			, "Keypad4"
			, "Keypad5"
			, "Keypad6"
			, "Keypad7"
			, "Keypad8"
			, "Keypad9"
			, "KeypadDecimal"
			, "KeypadDivide"
			, "KeypadMultiply"
			, "KeypadSubtract"
			, "KeypadAdd"
			, "KeypadEnter"
			, "KeypadEqual"

			, "LeftShift"
			, "LeftControl"
			, "LeftAlt"
			, "LeftSuper"
			, "RightShift"
			, "RightControl"
			, "RightAlt"
			, "RightSuper"
			, "Menu"
		};
	};
}
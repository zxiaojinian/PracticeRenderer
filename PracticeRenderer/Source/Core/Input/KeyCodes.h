#pragma once

#include <iostream>

namespace PR
{
	typedef enum class KeyCode : uint16_t
	{
		// From glfw3.h
		Space = 32,
		Apostrophe = 39, /* ' */
		Comma = 44, /* , */
		Minus = 45, /* - */
		Period = 46, /* . */
		Slash = 47, /* / */

		D0 = 48, /* 0 */
		D1 = 49, /* 1 */
		D2 = 50, /* 2 */
		D3 = 51, /* 3 */
		D4 = 52, /* 4 */
		D5 = 53, /* 5 */
		D6 = 54, /* 6 */
		D7 = 55, /* 7 */
		D8 = 56, /* 8 */
		D9 = 57, /* 9 */

		Semicolon = 59, /* ; */
		Equal = 61, /* = */

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

		LeftBracket = 91,  /* [ */
		Backslash = 92,  /* \ */
		RightBracket = 93,  /* ] */
		GraveAccent = 96,  /* ` */

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
		Menu = 348
	} Key;

	inline std::ostream& operator<<(std::ostream& os, KeyCode keyCode)
	{
		os << static_cast<int32_t>(keyCode);
		return os;
	}
}
// From glfw3.h
#define PR_KEY_SPACE           ::PR::Key::Space
#define PR_KEY_APOSTROPHE      ::PR::Key::Apostrophe    /* ' */
#define PR_KEY_COMMA           ::PR::Key::Comma         /* , */
#define PR_KEY_MINUS           ::PR::Key::Minus         /* - */
#define PR_KEY_PERIOD          ::PR::Key::Period        /* . */
#define PR_KEY_SLASH           ::PR::Key::Slash         /* / */
#define PR_KEY_0               ::PR::Key::D0
#define PR_KEY_1               ::PR::Key::D1
#define PR_KEY_2               ::PR::Key::D2
#define PR_KEY_3               ::PR::Key::D3
#define PR_KEY_4               ::PR::Key::D4
#define PR_KEY_5               ::PR::Key::D5
#define PR_KEY_6               ::PR::Key::D6
#define PR_KEY_7               ::PR::Key::D7
#define PR_KEY_8               ::PR::Key::D8
#define PR_KEY_9               ::PR::Key::D9
#define PR_KEY_SEMICOLON       ::PR::Key::Semicolon     /* ; */
#define PR_KEY_EQUAL           ::PR::Key::Equal         /* = */
#define PR_KEY_A               ::PR::Key::A
#define PR_KEY_B               ::PR::Key::B
#define PR_KEY_C               ::PR::Key::C
#define PR_KEY_D               ::PR::Key::D
#define PR_KEY_E               ::PR::Key::E
#define PR_KEY_F               ::PR::Key::F
#define PR_KEY_G               ::PR::Key::G
#define PR_KEY_H               ::PR::Key::H
#define PR_KEY_I               ::PR::Key::I
#define PR_KEY_J               ::PR::Key::J
#define PR_KEY_K               ::PR::Key::K
#define PR_KEY_L               ::PR::Key::L
#define PR_KEY_M               ::PR::Key::M
#define PR_KEY_N               ::PR::Key::N
#define PR_KEY_O               ::PR::Key::O
#define PR_KEY_P               ::PR::Key::P
#define PR_KEY_Q               ::PR::Key::Q
#define PR_KEY_R               ::PR::Key::R
#define PR_KEY_S               ::PR::Key::S
#define PR_KEY_T               ::PR::Key::T
#define PR_KEY_U               ::PR::Key::U
#define PR_KEY_V               ::PR::Key::V
#define PR_KEY_W               ::PR::Key::W
#define PR_KEY_X               ::PR::Key::X
#define PR_KEY_Y               ::PR::Key::Y
#define PR_KEY_Z               ::PR::Key::Z
#define PR_KEY_LEFT_BRACKET    ::PR::Key::LeftBracket   /* [ */
#define PR_KEY_BACKSLASH       ::PR::Key::Backslash     /* \ */
#define PR_KEY_RIGHT_BRACKET   ::PR::Key::RightBracket  /* ] */
#define PR_KEY_GRAVE_ACCENT    ::PR::Key::GraveAccent   /* ` */
#define PR_KEY_WORLD_1         ::PR::Key::World1        /* non-US #1 */
#define PR_KEY_WORLD_2         ::PR::Key::World2        /* non-US #2 */

/* Function keys */
#define PR_KEY_ESCAPE          ::PR::Key::Escape
#define PR_KEY_ENTER           ::PR::Key::Enter
#define PR_KEY_TAB             ::PR::Key::Tab
#define PR_KEY_BACKSPACE       ::PR::Key::Backspace
#define PR_KEY_INSERT          ::PR::Key::Insert
#define PR_KEY_DELETE          ::PR::Key::Delete
#define PR_KEY_RIGHT           ::PR::Key::Right
#define PR_KEY_LEFT            ::PR::Key::Left
#define PR_KEY_DOWN            ::PR::Key::Down
#define PR_KEY_UP              ::PR::Key::Up
#define PR_KEY_PAGE_UP         ::PR::Key::PageUp
#define PR_KEY_PAGE_DOWN       ::PR::Key::PageDown
#define PR_KEY_HOME            ::PR::Key::Home
#define PR_KEY_END             ::PR::Key::End
#define PR_KEY_CAPS_LOCK       ::PR::Key::CapsLock
#define PR_KEY_SCROLL_LOCK     ::PR::Key::ScrollLock
#define PR_KEY_NUM_LOCK        ::PR::Key::NumLock
#define PR_KEY_PRINT_SCREEN    ::PR::Key::PrintScreen
#define PR_KEY_PAUSE           ::PR::Key::Pause
#define PR_KEY_F1              ::PR::Key::F1
#define PR_KEY_F2              ::PR::Key::F2
#define PR_KEY_F3              ::PR::Key::F3
#define PR_KEY_F4              ::PR::Key::F4
#define PR_KEY_F5              ::PR::Key::F5
#define PR_KEY_F6              ::PR::Key::F6
#define PR_KEY_F7              ::PR::Key::F7
#define PR_KEY_F8              ::PR::Key::F8
#define PR_KEY_F9              ::PR::Key::F9
#define PR_KEY_F10             ::PR::Key::F10
#define PR_KEY_F11             ::PR::Key::F11
#define PR_KEY_F12             ::PR::Key::F12
#define PR_KEY_F13             ::PR::Key::F13
#define PR_KEY_F14             ::PR::Key::F14
#define PR_KEY_F15             ::PR::Key::F15
#define PR_KEY_F16             ::PR::Key::F16
#define PR_KEY_F17             ::PR::Key::F17
#define PR_KEY_F18             ::PR::Key::F18
#define PR_KEY_F19             ::PR::Key::F19
#define PR_KEY_F20             ::PR::Key::F20
#define PR_KEY_F21             ::PR::Key::F21
#define PR_KEY_F22             ::PR::Key::F22
#define PR_KEY_F23             ::PR::Key::F23
#define PR_KEY_F24             ::PR::Key::F24
#define PR_KEY_F25             ::PR::Key::F25

/* Keypad */
#define PR_KEY_KP_0            ::PR::Key::KP0
#define PR_KEY_KP_1            ::PR::Key::KP1
#define PR_KEY_KP_2            ::PR::Key::KP2
#define PR_KEY_KP_3            ::PR::Key::KP3
#define PR_KEY_KP_4            ::PR::Key::KP4
#define PR_KEY_KP_5            ::PR::Key::KP5
#define PR_KEY_KP_6            ::PR::Key::KP6
#define PR_KEY_KP_7            ::PR::Key::KP7
#define PR_KEY_KP_8            ::PR::Key::KP8
#define PR_KEY_KP_9            ::PR::Key::KP9
#define PR_KEY_KP_DECIMAL      ::PR::Key::KPDecimal
#define PR_KEY_KP_DIVIDE       ::PR::Key::KPDivide
#define PR_KEY_KP_MULTIPLY     ::PR::Key::KPMultiply
#define PR_KEY_KP_SUBTRACT     ::PR::Key::KPSubtract
#define PR_KEY_KP_ADD          ::PR::Key::KPAdd
#define PR_KEY_KP_ENTER        ::PR::Key::KPEnter
#define PR_KEY_KP_EQUAL        ::PR::Key::KPEqual

#define PR_KEY_LEFT_SHIFT      ::PR::Key::LeftShift
#define PR_KEY_LEFT_CONTROL    ::PR::Key::LeftControl
#define PR_KEY_LEFT_ALT        ::PR::Key::LeftAlt
#define PR_KEY_LEFT_SUPER      ::PR::Key::LeftSuper
#define PR_KEY_RIGHT_SHIFT     ::PR::Key::RightShift
#define PR_KEY_RIGHT_CONTROL   ::PR::Key::RightControl
#define PR_KEY_RIGHT_ALT       ::PR::Key::RightAlt
#define PR_KEY_RIGHT_SUPER     ::PR::Key::RightSuper
#define PR_KEY_MENU            ::PR::Key::Menu
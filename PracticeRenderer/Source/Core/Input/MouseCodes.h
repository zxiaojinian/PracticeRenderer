#pragma once

#include <iostream>

namespace PR
{
	typedef enum class MouseCode : uint16_t
	{
		// From glfw3.h
		Button0 = 0,
		Button1 = 1,
		Button2 = 2,
		Button3 = 3,
		Button4 = 4,
		Button5 = 5,
		Button6 = 6,
		Button7 = 7,

		ButtonLast = Button7,
		ButtonLeft = Button0,
		ButtonRight = Button1,
		ButtonMiddle = Button2
	} Mouse;

	inline std::ostream& operator<<(std::ostream& os, MouseCode mouseCode)
	{
		os << static_cast<int32_t>(mouseCode);
		return os;
	}
}

#define PR_MOUSE_BUTTON_0      ::PR::Mouse::Button0
#define PR_MOUSE_BUTTON_1      ::PR::Mouse::Button1
#define PR_MOUSE_BUTTON_2      ::PR::Mouse::Button2
#define PR_MOUSE_BUTTON_3      ::PR::Mouse::Button3
#define PR_MOUSE_BUTTON_4      ::PR::Mouse::Button4
#define PR_MOUSE_BUTTON_5      ::PR::Mouse::Button5
#define PR_MOUSE_BUTTON_6      ::PR::Mouse::Button6
#define PR_MOUSE_BUTTON_7      ::PR::Mouse::Button7
#define PR_MOUSE_BUTTON_LAST   ::PR::Mouse::ButtonLast
#define PR_MOUSE_BUTTON_LEFT   ::PR::Mouse::ButtonLeft
#define PR_MOUSE_BUTTON_RIGHT  ::PR::Mouse::ButtonRight
#define PR_MOUSE_BUTTON_MIDDLE ::PR::Mouse::ButtonMiddle
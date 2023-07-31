#pragma once

#include "Core/Input/KeyCodes.h"
#include "Core/Input/MouseCodes.h"

#include <utility>

namespace PR 
{
	class Input
	{
	public:
		static bool IsKeyPressed(KeyCode key);

		static bool IsMouseButtonPressed(MouseCode button);
		static std::pair<float, float> GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};

}
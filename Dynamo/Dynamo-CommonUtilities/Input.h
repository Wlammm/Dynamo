#pragma once

#include "Point.h"
#include <Windows.h>
#include <vector>

namespace CommonUtilities
{
	struct KeyState
	{
		bool myIsDown = false;
		bool myIsPressed = false;
		bool myIsUp = false;
	};

	class Input
	{
	public:
		Input();
		bool IsKeyDown(const int& aKeyCode) const;
		bool IsKeyPressed(const int& aKeyCode) const;
		bool IsKeyUp(const int& aKeyCode) const;

		Point GetMousePosition() const;

		bool UpdateEvents(UINT message, WPARAM wParam, LPARAM lParam);

	private:
		KeyState myKeyState[254];

		bool myLeftMousePressed = false;
		bool myRightMousePressed = false;
		bool myMiddleMousePressed = false;

		Point myMousePoint;
	};
}

namespace CU = CommonUtilities;
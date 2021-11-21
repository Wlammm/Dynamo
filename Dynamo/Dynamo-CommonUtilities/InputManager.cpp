#include "InputManager.h"
#include <WindowsX.h>

bool CommonUtilities::InputManager::IsKeyDown(const int aKeyCode)
{
	return myKeyState[aKeyCode].myIsDown;
}

bool CommonUtilities::InputManager::IsKeyDown(const KeyCode aKeyCode)
{
	return IsKeyDown(static_cast<int>(aKeyCode));
}

bool CommonUtilities::InputManager::IsKeyDown(const MouseButton aMouseButton)
{
	return IsKeyDown(static_cast<int>(aMouseButton));
}

bool CommonUtilities::InputManager::IsKeyUp(const int aKeyCode)
{
	return myKeyState[aKeyCode].myIsUp;
}

bool CommonUtilities::InputManager::IsKeyUp(const KeyCode aKeyCode)
{
	return IsKeyUp(static_cast<int>(aKeyCode));
}

bool CommonUtilities::InputManager::IsKeyUp(const MouseButton aMouseButton)
{
	return IsKeyUp(static_cast<int>(aMouseButton));
}

bool CommonUtilities::InputManager::IsKeyPressed(const int aKeyCode)
{
	return myKeyState[aKeyCode].myIsPressed;
}

bool CommonUtilities::InputManager::IsKeyPressed(const KeyCode aKeyCode)
{
	return IsKeyPressed(static_cast<int>(aKeyCode));
}

bool CommonUtilities::InputManager::IsKeyPressed(const MouseButton aMouseButton)
{
	return IsKeyPressed(static_cast<int>(aMouseButton));
}

POINT CommonUtilities::InputManager::GetMousePosition() const
{
	return myMousePos;
}

POINT CommonUtilities::InputManager::GetMousePositionClientRelative() const
{
	return myMousePosClientRelative;
}

POINT CommonUtilities::InputManager::GetMouseDelta() const
{
	return { myMousePos.x - myMousePosLastFrame.x, myMousePos.y - myMousePosLastFrame.y };
}

POINT CommonUtilities::InputManager::GetMouseDeltaClientRelative() const
{
	return { myMousePosClientRelative.x - myMousePosClientRelativeLastFrame.x, myMousePosClientRelative.y - myMousePosClientRelativeLastFrame.y };
}

bool CommonUtilities::InputManager::UpdateEvents(UINT message, WPARAM wParam, LPARAM lParam)
{
	bool detectMessage = false;

	switch (message)
	{
	case WM_KEYDOWN:
	{
		int keyCode = static_cast<int>(wParam);

		switch (keyCode)
		{
		case VK_SHIFT:
		case VK_CONTROL:
		case VK_MENU:
			keyCode = GetRightOrLeftKeyCode(keyCode, lParam);
			break;
		default:
			break;
		}

		if (myKeyState[keyCode].myIsPressed == false)
		{
			myKeyState[keyCode].myIsDown = true;
		}
		myKeyState[keyCode].myIsPressed = true;
		detectMessage = true;
	} break;

	case WM_KEYUP:
	{
		int keyCode = static_cast<int>(wParam);

		switch (keyCode)
		{
		case VK_SHIFT:
		case VK_CONTROL:
		case VK_MENU:
			keyCode = GetRightOrLeftKeyCode(keyCode, lParam);
			break;
		default:
			break;
		}

		myKeyState[keyCode].myIsUp = true;
		myKeyState[keyCode].myIsDown = false;
		myKeyState[keyCode].myIsPressed = false;
		detectMessage = true;
	} break;

	case WM_MOUSEMOVE:
		POINT p;
		if (GetCursorPos(&p))
		{
			myMousePos.x = p.x;
			myMousePos.y = p.y;
		}
		myMousePosClientRelative.x = GET_X_LPARAM(lParam);
		myMousePosClientRelative.y = GET_Y_LPARAM(lParam);
		detectMessage = true;
		break;

	case WM_LBUTTONDOWN:
		if (myKeyState[VK_LBUTTON].myIsPressed == false)
		{
			myKeyState[VK_LBUTTON].myIsDown = true;
		}
		myKeyState[VK_LBUTTON].myIsPressed = true;
		detectMessage = true;
		break;

	case WM_LBUTTONUP:
		myKeyState[VK_LBUTTON].myIsUp = true;
		myKeyState[VK_LBUTTON].myIsDown = false;
		myKeyState[VK_LBUTTON].myIsPressed = false;
		detectMessage = true;
		break;

	case WM_RBUTTONDOWN:
		if (myKeyState[VK_RBUTTON].myIsPressed == false)
		{
			myKeyState[VK_RBUTTON].myIsDown = true;
		}
		myKeyState[VK_RBUTTON].myIsPressed = true;
		detectMessage = true;
		break;

	case WM_RBUTTONUP:
		myKeyState[VK_RBUTTON].myIsUp = true;
		myKeyState[VK_RBUTTON].myIsDown = false;
		myKeyState[VK_RBUTTON].myIsPressed = false;
		detectMessage = true;
		break;

	case WM_MBUTTONDOWN:
		if (myKeyState[VK_MBUTTON].myIsPressed == false)
		{
			myKeyState[VK_MBUTTON].myIsDown = true;
		}
		myKeyState[VK_MBUTTON].myIsPressed = true;
		detectMessage = true;
		break;

	case WM_MBUTTONUP:
		myKeyState[VK_MBUTTON].myIsUp = true;
		myKeyState[VK_MBUTTON].myIsDown = false;
		myKeyState[VK_MBUTTON].myIsPressed = false;
		detectMessage = true;
		break;

	default:
		break;
	}

	return detectMessage;
}

void CommonUtilities::InputManager::EndFrame()
{
	for (int keyCode = 0; keyCode < 254; ++keyCode)
	{
		myKeyState[keyCode].myIsUp = false;
		myKeyState[keyCode].myIsDown = false;
	}

	myMousePosLastFrame = myMousePos;
	myMousePosClientRelativeLastFrame = myMousePosClientRelative;
}

int CommonUtilities::InputManager::GetRightOrLeftKeyCode(const int aVirtualKeyCode, const LPARAM lParam)
{
	int resultKeyCode = aVirtualKeyCode;

	const auto scanCode = static_cast<UINT>((lParam & 0x00ff0000) >> 16);
	const bool isExtended = (lParam & 0x01000000);

	switch (aVirtualKeyCode)
	{
	case VK_SHIFT:
		// Convert the scan code to a virtual key code
		resultKeyCode = MapVirtualKey(scanCode, MAPVK_VSC_TO_VK_EX);
		break;
	case VK_CONTROL:
		resultKeyCode = isExtended ? VK_RCONTROL : VK_LCONTROL;
		break;
	case VK_MENU:
		resultKeyCode = isExtended ? VK_RMENU : VK_LMENU;
		break;
	default:
		break;
	}

	return resultKeyCode;
}


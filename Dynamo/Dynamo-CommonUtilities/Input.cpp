#include "pch.h"
#include "Input.h"
#include <windowsx.h>


CommonUtilities::Input::Input()
    : myMousePoint(Point(0, 0))
{
}

bool CommonUtilities::Input::IsKeyDown(const int& aKeyCode) const
{
    return myKeyState[aKeyCode].myIsDown;
}

bool CommonUtilities::Input::IsKeyPressed(const int& aKeyCode) const
{
    return myKeyState[aKeyCode].myIsPressed;
}

bool CommonUtilities::Input::IsKeyUp(const int& aKeyCode) const
{
    return myKeyState[aKeyCode].myIsUp;
}

CU::Point CommonUtilities::Input::GetMousePosition() const
{
    return myMousePoint;
}

bool CommonUtilities::Input::UpdateEvents(UINT message, WPARAM wParam, LPARAM lParam)
{
    for (int i = 0; i < 254; ++i)
    {
        myKeyState[i].myIsUp = false;
        myKeyState[i].myIsDown = false;
    }

    switch (message)
    {
    case WM_KEYDOWN:
        if (myKeyState[wParam].myIsPressed == false)
        {
            myKeyState[wParam].myIsDown = true;
        }
        myKeyState[wParam].myIsPressed = true;
        return true;

    case WM_KEYUP:
        myKeyState[wParam].myIsUp = true;
        myKeyState[wParam].myIsDown = false;
        myKeyState[wParam].myIsPressed = false;
        return true;

    case WM_MOUSEMOVE:
        myMousePoint.SetX(GET_X_LPARAM(lParam));
        myMousePoint.SetY(GET_Y_LPARAM(lParam));
        return true;

    case WM_LBUTTONDOWN:
        if (myKeyState[VK_LBUTTON].myIsPressed == false)
        {
            myKeyState[VK_LBUTTON].myIsDown = true;
        }
        myKeyState[VK_LBUTTON].myIsPressed = true;
        return true;

    case WM_RBUTTONDOWN:
        myKeyState[VK_LBUTTON].myIsUp = true;
        myKeyState[VK_LBUTTON].myIsDown = false;
        myKeyState[VK_LBUTTON].myIsPressed = false;
        return true;

    case WM_MBUTTONDOWN:
        if (myKeyState[VK_MBUTTON].myIsPressed == false)
        {
            myKeyState[VK_MBUTTON].myIsDown = true;
        }
        myKeyState[VK_MBUTTON].myIsPressed = true;
        return true;

    case WM_LBUTTONUP:
        myKeyState[VK_MBUTTON].myIsUp = true;
        myKeyState[VK_MBUTTON].myIsDown = false;
        myKeyState[VK_MBUTTON].myIsPressed = false;
        return true;

    case WM_RBUTTONUP:
        if (myKeyState[VK_RBUTTON].myIsPressed == false)
        {
            myKeyState[VK_RBUTTON].myIsDown = true;
        }
        myKeyState[VK_RBUTTON].myIsPressed = true;
        return true;

    case WM_MBUTTONUP:
        myKeyState[VK_RBUTTON].myIsUp = true;
        myKeyState[VK_RBUTTON].myIsDown = false;
        myKeyState[VK_RBUTTON].myIsPressed = false;
        return true;

    default:
        return false;
    }
}
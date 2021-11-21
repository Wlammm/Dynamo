#include "pch.h"
#include "WindowHandler.h"
#include <Windows.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace Dynamo
{
	WindowHandler::WindowHandler()
	{
		WNDCLASS windowClass = {};
		windowClass.style = CS_VREDRAW | CS_HREDRAW | CS_OWNDC;
		windowClass.lpfnWndProc = WindowHandler::WndProc;
		windowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
		windowClass.lpszClassName = L"DynamoMainWnd";
		RegisterClass(&windowClass);

		assert(Main::ourInstance != nullptr && "OurInstance in MainSingleton is not initialized.");

		Main::ourInstance->myWindowHandle = CreateWindow(L"DynamoMainWnd", Main::GetWindowTitle().c_str(), WS_OVERLAPPEDWINDOW | WS_POPUP | WS_VISIBLE, Main::GetWindowOffset().x, Main::GetWindowOffset().y,
			Main::GetWindowSize().x, Main::GetWindowSize().y, nullptr, nullptr, nullptr, this);
	}


	LRESULT WindowHandler::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		{
			return true;
		}

		if (Input::GetManager().UpdateEvents(uMsg, wParam, lParam))
		{
			return 0;
		}

		if (uMsg == WM_DESTROY || uMsg == WM_CLOSE)
		{
			PostQuitMessage(0);
			return 0;
		}
		else if (uMsg == WM_CREATE)
		{
			// The code inside here doesnt seem to do anything? 
			return 0;
			
			//CREATESTRUCT* createStructs = reinterpret_cast<CREATESTRUCT*>(lParam);
		}
		
		if (uMsg == WM_SIZE)
		{
			Main::ourInstance->myWindowResolution = { LOWORD(lParam), HIWORD(lParam) };
			return 0;
		}

		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
}

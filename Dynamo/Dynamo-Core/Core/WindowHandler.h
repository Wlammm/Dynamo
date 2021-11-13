#pragma once

namespace Dynamo
{
	class WindowHandler
	{
	public:
		WindowHandler();
		~WindowHandler() = default;

		static LRESULT CALLBACK WndProc(_In_ HWND hWnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam);

	private:
		std::wstring myWindowName;

	};
}
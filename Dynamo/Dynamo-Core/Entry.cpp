#include "pch.h"
#include "Entry.h"
#include "Core/DirectXFramework.h"

namespace Dynamo
{
	void Entry::Run()
	{
		Main::Init();

		Loop();
	}

	void Entry::Loop()
	{
		MSG windowsMessage = { 0 };
		while (Main::GetEngineRunning())
		{
			while (PeekMessage(&windowsMessage, 0, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&windowsMessage);
				DispatchMessage(&windowsMessage);

				if (windowsMessage.message == WM_QUIT)
				{
					Main::SetEngineRunning(false);
				}
			}

			Main::GetFramework().BeginFrame();



			Main::GetFramework().RenderFrame();
			Main::GetFramework().EndFrame();
		}
	}
}
#include "pch.h"
#include "Entry.h"
#include "Core/DirectXFramework.h"
#include "Scene/Scene.h"
#include "Utils/CrashHandler.h"

namespace Dynamo
{
	void Entry::Init()
	{
		Main::Init();
	}

	void Entry::Run()
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

			Main::Update();

			Main::GetFramework().RenderFrame();
			Main::GetFramework().EndFrame();
		}

		Main::Destroy();
	}
}
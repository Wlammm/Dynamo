#pragma once

namespace Dynamo
{
	class ImGuiManager
	{
	public:
		static void Start();
		static void BeginFrame();
		static void Render();
		static void Destroy();
	};
}
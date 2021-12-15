#include "EditorPch.h"
#include "RenderDebugger.h"

namespace Editor
{
	RenderDebugger::RenderDebugger()
		: EditorWindow("Render Debugger")
	{
	}

	void RenderDebugger::Update()
	{
		ImGui::Text("Depth Texture:##renderdebugger");
		ImGui::Image(Dyn::Main::GetRenderManager().GetDepthTexture().GetSRV(), ImVec2(256, 256));
	}
}

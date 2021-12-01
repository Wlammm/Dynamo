#include "EditorPch.h"
#include "Viewport.h"
#include "Systems/Guizmos.h"

namespace Editor
{
	Viewport::Viewport()
		: EditorWindow("Viewport")
	{ }

	void Viewport::Init()
	{
		myGuizmos = Main::GetEditorManager()->GetSystem<Guizmos>();
	}

	void Viewport::Update()
	{
		Dyn::Texture& renderTexture = Dyn::Main::GetRenderManager().GetMainRenderTarget();

		Vec2ui viewportOffset = { 8, 25 };
		const ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		ImVec2 imageSize = ClampToAspectRatio(viewportPanelSize, { 16, 9 });

		ImVec2 pos = { (viewportPanelSize.x - imageSize.x) * 0.5f, (viewportPanelSize.y - imageSize.y) * 0.5f };

		pos.x += viewportOffset.x;
		pos.y += viewportOffset.y;

		ImGui::SetCursorPos(pos);
		ImGui::Image(renderTexture.GetSRV(), imageSize);

		ImVec2 windowPos = ImGui::GetWindowPos();
		pos.x += windowPos.x;
		pos.y += windowPos.y;

		myGuizmos->DrawGuizmos(pos, imageSize);
	}

	const ImVec2 Viewport::ClampToAspectRatio(const ImVec2& aSize, const ImVec2& anAspectRatio) const
	{
		float x = aSize.x, y = aSize.y;

		if (aSize.x < aSize.y * (anAspectRatio.x / anAspectRatio.y))
		{
			y = aSize.x * (anAspectRatio.y / anAspectRatio.x);
			x = aSize.x;
		}
		else if (aSize.y < aSize.x * (anAspectRatio.y / anAspectRatio.x))
		{
			y = aSize.y;
			x = aSize.y * (anAspectRatio.x / anAspectRatio.y);
		}
		else
		{
			y = aSize.y;
			x = aSize.x;
		}
		return ImVec2(x, y);
	}
}

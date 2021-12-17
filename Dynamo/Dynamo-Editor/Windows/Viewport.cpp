#include "EditorPch.h"
#include "Viewport.h"
#include "Systems/Guizmos.h"

#include "Components/MeshRenderer.h"
#include "Components/Camera.h"

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
		myImageSize = ClampToAspectRatio(viewportPanelSize, { 16, 9 });

		ImVec2 pos = { (viewportPanelSize.x - myImageSize.x) * 0.5f, (viewportPanelSize.y - myImageSize.y) * 0.5f };

		pos.x += viewportOffset.x;
		pos.y += viewportOffset.y;

		ImGui::SetCursorPos(pos);
		ImGui::Image(renderTexture.GetSRV(), myImageSize);
		myImagePos = ImGui::GetItemRectMin();
		HandleDragDrop();

		ImVec2 windowPos = ImGui::GetWindowPos();
		pos.x += windowPos.x;
		pos.y += windowPos.y;

		myGuizmos->DrawGuizmos(pos, myImageSize);
	}

	Vec2f Viewport::GetMousePosWindowNormalized() const
	{
		const Vec2ui& mousePos = Input::GetMousePosition();
		Vec2f mousePosRelative(mousePos.x - myImagePos.x, mousePos.y - myImagePos.y);
		Vec2f imageSizeCU(myImageSize.x, myImageSize.y);
		mousePosRelative = mousePosRelative / imageSizeCU;

		if (mousePosRelative.x > 1 || mousePosRelative.x < 0 ||
			mousePosRelative.y > 1 || mousePosRelative.y < 0)
		{
			mousePosRelative = { -1, -1 };
		}

		return mousePosRelative;
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

	void Viewport::HandleDragDrop()
	{
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(".fbx"))
			{
				std::string path = *(std::string*)payload->Data;
				Dyn::Scene* scene = Dyn::Main::GetScene();
				Dyn::Camera* cam = Dyn::Main::GetMainCamera();

				if (scene && cam)
				{
					std::filesystem::path fsPath = path;
					GameObject* ob = Dyn::Main::GetScene()->CreateGameObject();
					ob->GetTransform().SetPosition(cam->GetTransform()->GetPosition() + cam->GetTransform()->GetForward() * 1000.0f);
					ob->SetName(fsPath.stem().string());
					Dyn::MeshRenderer* mesh = ob->AddComponent<Dyn::MeshRenderer>();
					mesh->SetModel(path);
				}
			}

			ImGui::EndDragDropTarget();
		}
	}
}

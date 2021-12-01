#include "EditorPch.h"
#include "Guizmos.h"
#include "Components/Camera.h"

namespace Editor
{
	Guizmos::Guizmos()
	{

	}

	void Guizmos::Update()
	{
		if (Input::IsKeyPressed(MouseButton::Right))
			return;

		if (Input::IsKeyDown(KeyCode::Q))
		{
			myOperation = (ImGuizmo::OPERATION)0;
		}

		if (Input::IsKeyDown(KeyCode::W))
		{
			myOperation = ImGuizmo::OPERATION::TRANSLATE;
		}

		if (Input::IsKeyDown(KeyCode::E))
		{
			myOperation = ImGuizmo::OPERATION::ROTATE;
		}

		if (Input::IsKeyDown(KeyCode::R))
		{
			myOperation = ImGuizmo::OPERATION::SCALE;
		}
	}

	void Guizmos::DrawGuizmos(const ImVec2& aPos, const ImVec2& aSize)
	{
		GameObject* selectedObject = Main::GetSelectedGameObject();
		if (!selectedObject)
			return;

		Dyn::Camera* camera = Dyn::Main::GetMainCamera();
		if (!camera)
			return;

		ImGuizmo::SetDrawlist(ImGui::GetWindowDrawList());
		ImGuizmo::SetRect(aPos.x, aPos.y, aSize.x, aSize.y);

		const Mat4f& viewMatrix = camera->GetTransform()->GetMatrix().FastInverse();
		const Mat4f& projection = camera->GetProjectionMatrix();
		Mat4f& objectMatrixRef = selectedObject->GetTransform().GetMatrixRef();
		ImGuizmo::Manipulate(&viewMatrix(1, 1), &projection(1, 1), myOperation, myOperation == ImGuizmo::OPERATION::SCALE ? ImGuizmo::MODE::LOCAL : mySpace, &objectMatrixRef(1, 1));
	}
}

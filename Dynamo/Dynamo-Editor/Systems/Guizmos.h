#pragma once

namespace Editor
{
	class Guizmos : public EditorSystem
	{
	public:
		Guizmos();

		void Update() override;

		void DrawGuizmos(const ImVec2& aPos, const ImVec2& aSize);

	private:
		ImGuizmo::OPERATION myOperation = ImGuizmo::OPERATION::TRANSLATE;
		ImGuizmo::MODE mySpace = ImGuizmo::MODE::WORLD;
	};
}
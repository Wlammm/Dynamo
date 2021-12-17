#pragma once

namespace Editor
{
	class Guizmos;

	class Viewport : public EditorWindow
	{
	public:
		Viewport();

		void Init() override;

		void Update() override;

		Vec2f GetMousePosWindowNormalized() const;

		bool IsHovered() const;

	private:
		const ImVec2 ClampToAspectRatio(const ImVec2& aSize, const ImVec2& anAspectRatio) const;

		void HandleDragDrop();

	private:
		Guizmos* myGuizmos = nullptr;

		ImVec2 myImagePos;
		ImVec2 myImageSize;

		bool myIsHovered = false;
	};
}
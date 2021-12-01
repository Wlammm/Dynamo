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

	private:
		const ImVec2 ClampToAspectRatio(const ImVec2& aSize, const ImVec2& anAspectRatio) const;

	private:
		Guizmos* myGuizmos = nullptr;
	};
}
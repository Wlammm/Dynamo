#pragma once

namespace Editor
{
	class Viewport : public EditorWindow
	{
	public:
		Viewport();
		void Update() override;

	private:
		const ImVec2 ClampToAspectRatio(const ImVec2& aSize, const ImVec2& anAspectRatio) const;
	};
}
#pragma once

namespace Editor
{
	class GraphNodeEditor : public EditorWindow
	{
	public:
		GraphNodeEditor();

		void Init() override;

		void Update() override;
	};
}

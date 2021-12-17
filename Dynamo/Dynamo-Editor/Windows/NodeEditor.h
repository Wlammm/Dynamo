#pragma once

namespace Editor
{
	class GraphManager;

	class NodeEditor : public EditorWindow
	{
	public:
		NodeEditor();
		~NodeEditor();

		void Init() override;
		void Update() override;

	private:
		GraphManager* myGraphManager = nullptr;

	};
}
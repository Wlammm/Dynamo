#pragma once

namespace Editor
{
	class NodeEditor : public EditorWindow
	{
	public:
		NodeEditor();
		~NodeEditor();

		void Init() override;
		void Update() override;

	private:

	};
}
#pragma once

namespace Editor
{
	class NodeRegistry;
	class NodeInstance;
	class NodePin;

	class NodeEditorManager
	{
	public:
		NodeEditorManager() = default;
		~NodeEditorManager() = default;

		static void Create();
		static void Destroy();

		static void Load();

		static void BeginFrame();
		static void Update();
		static void EndFrame();

		static void AddNode(const int aNodeID);
		static int GetPinUID();

		static NodeRegistry& GetNodeRegistry();

	private:
		void BeginFrameInternal();
		void UpdateInternal();
		void EndFrameInternal();

		void AddNodeInternal(const int aNodeID);

		void DrawNodeHeaderInternal(NodeInstance* aNodeInstance);
		void DrawNodeBodyInternal(NodeInstance* aNodeInstance);

		void DrawPinIcon(NodePin* aPin, const bool aIsConnected);

	private:
		inline static NodeEditorManager* ourInstance;

		ImGui::NodeEditor::EditorContext* myEditorContext = nullptr;

	private:
		NodeRegistry* myNodeRegistry = nullptr;
		std::vector<NodeInstance*> myNodeInstances;
		unsigned int myNodeInstanceUID_Counter = 0;
		unsigned int myNodePinUID_Counter = 0;

	};
}


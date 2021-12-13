#include "EditorPch.h"
#include "NodeEditorManager.h"

#include "NodeRegistry.h"

#include "Drawing.h"
#include "Widgets.h"
#include "Interop.h"

#include "NodeEditor/BaseTypes/NodeInstance.h"
#include "NodeEditor/BaseTypes/NodePin.h"

void Editor::NodeEditorManager::Create()
{
	ourInstance = new NodeEditorManager();
	ourInstance->myNodeRegistry = new NodeRegistry();
	ourInstance->myNodeInstanceUID_Counter = 0;
	ourInstance->myNodePinUID_Counter = 0;
}

void Editor::NodeEditorManager::Destroy()
{
	for (auto& node : ourInstance->myNodeInstances)
	{
		delete node;
		node = nullptr;
	}

	delete ourInstance->myNodeRegistry;
	ourInstance->myNodeRegistry = nullptr;

	delete ourInstance;
	ourInstance = nullptr;
}

void Editor::NodeEditorManager::Load()
{
	ImGui::NodeEditor::Config config;
	config.SettingsFile = "Assets/Editor/NodeEditor/NodeEditorSettings.json";
	ourInstance->myEditorContext = ImGui::NodeEditor::CreateEditor(&config);
	ourInstance->myNodeRegistry->PopulateTypes();

	ourInstance->AddNode(0);
}

void Editor::NodeEditorManager::BeginFrame()
{
	ourInstance->BeginFrameInternal();
}

void Editor::NodeEditorManager::Update()
{
	ourInstance->UpdateInternal();
}

void Editor::NodeEditorManager::EndFrame()
{
	ourInstance->EndFrameInternal();
}

void Editor::NodeEditorManager::AddNode(const int aNodeID)
{
	ourInstance->AddNodeInternal(aNodeID);
}

int Editor::NodeEditorManager::GetPinUID()
{
	return ourInstance->myNodePinUID_Counter++;
}

Editor::NodeRegistry& Editor::NodeEditorManager::GetNodeRegistry()
{
	return *ourInstance->myNodeRegistry;
}

void Editor::NodeEditorManager::BeginFrameInternal()
{
	ImGui::NodeEditor::SetCurrentEditor(myEditorContext);
	ImGui::NodeEditor::Begin("NodeEditor");
}

void Editor::NodeEditorManager::UpdateInternal()
{
	for (auto& node : myNodeInstances)
	{
		namespace NE = ImGui::NodeEditor;
		const int uid = node->GetUID();
		const auto& position = node->GetPosition();
		NE::SetNodePosition(uid, position);

		NE::PushStyleVar(NE::StyleVar_NodeRounding, 3.0f);
		NE::PushStyleVar(NE::StyleVar_NodePadding, ImVec4(8, 4, 8, 8));
		NE::BeginNode(uid);
		ImGui::PushID(uid);

		DrawNodeHeaderInternal(node);
		DrawNodeBodyInternal(node);

		ImGui::PopID();
		NE::EndNode();
	}
}

void Editor::NodeEditorManager::EndFrameInternal()
{
	ImGui::NodeEditor::End();
}

void Editor::NodeEditorManager::AddNodeInternal(const int aNodeID)
{
	NodeInstance* node = new NodeInstance(aNodeID, myNodeInstanceUID_Counter);
	myNodeInstances.push_back(node);
	myNodeInstanceUID_Counter++;
}

void Editor::NodeEditorManager::DrawNodeHeaderInternal(NodeInstance* aNodeInstance)
{
	ImGui::BeginVertical("node");
	{
		ImGui::BeginHorizontal("header");
		{
			ImGui::BeginVertical("headerLabel", ImVec2(0, 28));
			{
				ImGui::TextUnformatted(aNodeInstance->GetNodeName().c_str());
			}
			ImGui::EndVertical();
			ImGui::Spring(1);
			ImGui::Dummy(ImVec2(0, 28));
			ImGui::Spring(0);
		}
		ImGui::EndHorizontal();
		ImGui::Spring(0, ImGui::GetStyle().ItemSpacing.y * 2.0f);
	}
	ImGui::EndVertical();
}

void Editor::NodeEditorManager::DrawNodeBodyInternal(NodeInstance* aNodeInstance)
{
	// Sort NodePins
	std::vector<NodePin*> inputPins;
	std::vector<NodePin*> outputPins;
	auto& nodePins = aNodeInstance->GetPins();
	int firstInput = -1;
	int firstOutput = -1;
	
	for (int i = 0; i < nodePins.size(); i++)
	{
		if (nodePins[i].myPinType == PinType::In)
		{
			inputPins.push_back(&nodePins[i]);
			if (nodePins[i].myPinData == PinData::None && firstInput == -1)
			{
				firstInput = static_cast<int>(inputPins.size() - 1);
			}
		}
		else
		{
			outputPins.push_back(&nodePins[i]);
			if (nodePins[i].myPinData == PinData::None && firstOutput == -1)
			{
				firstOutput = static_cast<int>(outputPins.size() - 1);
			}
		}
	}

	namespace NE = ImGui::NodeEditor;
	
	ImGui::BeginVertical("Body");
	{
		const bool drawInputRow = firstInput + firstOutput >= -1;
		if (drawInputRow)
		{
			ImGui::BeginHorizontal("PinRow_None");
			{
				if (firstInput > -1)
				{
					NodePin* pin = inputPins[firstInput];
					NE::BeginPin(pin->myUID, NE::PinKind::Input);
					DrawPinIcon(pin, false);
					NE::EndPin();
				}
				if (firstOutput > -1)
				{
					ImGui::Spring(1.0f);
					ImGui::Dummy({ 75, 0 });

					NodePin* pin = outputPins[firstOutput];
					ImGui::Text(pin->myLabel.c_str());
					NE::BeginPin(pin->myUID, NE::PinKind::Output);
					DrawPinIcon(pin, false);
					NE::EndPin();
				}
			}
			ImGui::EndHorizontal();
		}
	}
	ImGui::EndVertical();
}

void Editor::NodeEditorManager::DrawPinIcon(NodePin* aPin, const bool aIsConnected)
{
	namespace Draw = ax::Drawing;
	namespace Widget = ax::Widgets;

	Draw::IconType iconType;
	PinData pinData = aPin->myPinData;
	ImColor color = PinDataColor::GetColor(pinData);
	color.Value.w = 1.0f;

	switch (pinData)
	{
	case PinData::None:     iconType = Draw::IconType::Flow;   break;
	case PinData::Bool:     iconType = Draw::IconType::Circle; break;
	case PinData::Int:      iconType = Draw::IconType::Circle; break;
	case PinData::Float:    iconType = Draw::IconType::Circle; break;
	case PinData::String:   iconType = Draw::IconType::Circle; break;
	case PinData::Unknown:  iconType = Draw::IconType::Circle; break;
	default:
		iconType = Draw::IconType::Circle; break;
		return;
	}

	const int pinIconSize = 24;
	Widget::Icon(ImVec2(pinIconSize, pinIconSize), iconType, aIsConnected, color, ImColor(32, 32, 32));
}

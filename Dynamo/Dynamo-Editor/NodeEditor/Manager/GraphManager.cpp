#include "EditorPch.h"
#include "GraphManager.h"

#include "Drawing.h"
#include "Widgets.h"
#include "Interop.h"

#include "GraphNodeRegistry.h"
#include "NodeEditor/BaseTypes/GraphNodeInstance.h"
#include "NodeEditor/Nodes/Variable/VariableNode.h"

namespace ed = ax::NodeEditor;

using namespace ax::Drawing;
static ed::EditorContext* g_Context = nullptr;

GraphManager::GraphManager()
{
	myGraphVariables.reserve(64);
}

GraphManager::~GraphManager()
{
	for (auto& nodeInstance : myNodeInstancesInGraph)
	{
		delete nodeInstance;
	}

	myNodeInstancesInGraph.clear();

	GraphNodeRegistry::Destroy();

	delete[] myNewVarName;
	delete[] myMenuSeachField;

	ed::DestroyEditor(g_Context);
}
void GraphManager::Load()
{
	ed::Config config;
	config.SettingsFile = "Simple.json";
	g_Context = ed::CreateEditor(&config);


	GraphNodeRegistry::PopulateTypes();
	myMenuSeachField = DBG_NEW char[127];
	memset(&myMenuSeachField[0], 0, sizeof(myMenuSeachField));

}

void GraphManager::ReTriggerUpdateringTrees()
{
	//Locate start nodes, we support N start nodes, we might want to remove this, as we dont "support" different trees with different starrtnodes to be connected. It might work, might not
	for (auto& nodeInstance : myNodeInstancesInGraph)
	{
		if (nodeInstance->myNodeType->IsStartNode() && nodeInstance->myShouldTriggerAgain)
		{
			nodeInstance->Enter();
		}
	}
}

void GraphManager::ReTriggerTree(std::string aSpecificNodeName)
{
	// Reset all variables
	for (auto& var : myGraphVariables)
	{
		var.Initialize();
	}

	//Locate start nodes, we support N start nodes, we might want to remove this, as we dont "support" different trees with different starrtnodes to be connected. It might work, might not
	for (auto& nodeInstance : myNodeInstancesInGraph)
	{
		if (nodeInstance->myNodeType->IsStartNode() && !nodeInstance->myShouldTriggerAgain)
		{
			if (aSpecificNodeName.empty() && nodeInstance->GetNodeName() == "Start" || nodeInstance->GetNodeName() == aSpecificNodeName)
			{
				nodeInstance->Enter();
			}
		}
	}
}

void GraphManager::ExecuteTreeWithPayload(std::string aStartNodeName, Payload aPayload)
{
	//Locate start nodes, we support N start nodes, we might want to remove this, as we dont "support" different trees with different starrtnodes to be connected. It might work, might not
	for (auto& nodeInstance : myNodeInstancesInGraph)
	{
		if (nodeInstance->myNodeType->IsStartNode() && !nodeInstance->myShouldTriggerAgain)
		{
			if (nodeInstance->GetNodeName() == aStartNodeName)
			{
				nodeInstance->EnterStartNode(aPayload);
			}
		}
	}
}

void GraphManager::ExecuteTreeWithPayload(std::tuple<std::string, Payload> aTuple)
{
	// Forward on ahead to the other function
	ExecuteTreeWithPayload(std::get<0>(aTuple), std::get<1>(aTuple));
}

void GraphManager::SaveTreeToFile()
{
	/*
	{
		rapidjson::StringBuffer s;
		rapidjson::PrettyWriter<rapidjson::StringBuffer> writer1(s);

		writer1.StartObject();
		writer1.Key("UID_MAX");

		writer1.StartObject();
		writer1.Key("Num");
		writer1.Int(UID::myGlobalUID);
		writer1.EndObject();

		writer1.Key("NodeInstances");
		writer1.StartArray();
		for (auto& nodeInstance : myNodeInstancesInGraph)
		{
			nodeInstance->Serialize(writer1);
		}
		writer1.EndArray();
		writer1.EndObject();



		std::ofstream of("nodeinstances.json");
		of << s.GetString();
	}
	//Links
	{
		rapidjson::StringBuffer s;
		rapidjson::PrettyWriter<rapidjson::StringBuffer> writer1(s);

		writer1.StartObject();
		writer1.Key("Links");
		writer1.StartArray();
		for (auto& link : myLinks)
		{
			writer1.StartObject();
			writer1.Key("ID");
			writer1.Int(static_cast<int>(link.Id.Get()));
			writer1.Key("Input");
			writer1.Int(static_cast<int>(link.InputId.Get()));
			writer1.Key("Output");
			writer1.Int(static_cast<int>(link.OutputId.Get()));
			writer1.EndObject();

		}
		writer1.EndArray();
		writer1.EndObject();


		std::ofstream of("links.json");
		of << s.GetString();
	}
	// Variables
	{
		rapidjson::StringBuffer s;
		rapidjson::PrettyWriter<rapidjson::StringBuffer> writer1(s);
		writer1.StartObject();
		writer1.Key("Variables");
		writer1.StartArray();
		for (auto& variable : myGraphVariables)
		{
			variable.Serialize(writer1);
		}
		writer1.EndArray();
		writer1.EndObject();

		std::ofstream of("variables.json");
		of << s.GetString();
	}
	*/
}

void GraphManager::LoadTreeFromFile()
{
	/*
	for (auto& nodeInstance : myNodeInstancesInGraph)
	{
		delete nodeInstance;
	}

	if (!myNodeInstancesInGraph.empty())
	{
		for (size_t i = 0; i < myNodeInstancesInGraph.size(); i++)
		{
			delete myNodeInstancesInGraph[i];
		}
	}

	myLinks.clear();
	myNodeInstancesInGraph.clear();
	myGraphVariables.clear();
	UID::myAllUIDs.clear();
	UID::myGlobalUID = 0;

	{
		std::ifstream inputFile("variables.json");
		std::stringstream jsonDocumentBuffer;
		std::string inputLine;
		while (std::getline(inputFile, inputLine))
		{
			jsonDocumentBuffer << inputLine << "\n";
		}

		rapidjson::Document document;
		document.Parse(jsonDocumentBuffer.str().c_str());

		rapidjson::Value& results = document["Variables"];

		for (rapidjson::SizeType i = 0; i < results.Size(); i++)
		{
			rapidjson::Value& variable = results[i];
			int UID = variable["UID"].GetInt();
			std::string Name = variable["Name"].GetString();

			DataType dataType = DataType::Unknown;
			DataPtr dataPtr;

			rapidjson::Value& variableData = variable["DATA"];
			dataType = (DataType)variable["DATA_TYPE"].GetUint();

			const SizeType dataSize = variableData.Size();
			if (dataSize > 0)
			{
				dataPtr = DataPtr::Create(dataType, dataSize);
				std::vector<char> data(dataSize);
				for (SizeType d = 0; d < dataSize; d++)
				{
					data[d] = (char)variableData[d].GetInt();
				}

				memcpy(dataPtr.Ptr, &data.front(), data.size());
			}

			myGraphVariables.push_back(GraphVariable(Name, dataType, dataPtr, false));
			myGraphVariables[myGraphVariables.size() - 1].myUID = UID;
		}
	}

	{
		std::ifstream inputFile("nodeinstances.json");
		std::stringstream jsonDocumentBuffer;
		std::string inputLine;

		while (std::getline(inputFile, inputLine))
		{
			jsonDocumentBuffer << inputLine << "\n";
		}
		rapidjson::Document document;
		document.Parse(jsonDocumentBuffer.str().c_str());

		rapidjson::Value& uidmax = document["UID_MAX"];

		int test = uidmax["Num"].GetInt();
		UID::myGlobalUID = test;

		rapidjson::Value& results = document["NodeInstances"];

		for (rapidjson::SizeType i = 0; i < results.Size(); i++)
		{
			rapidjson::Value& nodeInstance = results[i];
			GraphNodeInstance* object = DBG_NEW GraphNodeInstance(this, false);
			int nodeType = nodeInstance["NodeType"].GetInt();
			int UID = nodeInstance["UID"].GetInt();

			object->myUID = UID;
			object->myNodeType = GraphNodeRegistry::GetNodeTypeFromID(nodeType);
			object->myEditorPos[0] = static_cast<float>(nodeInstance["Position"]["X"].GetInt());
			object->myEditorPos[1] = static_cast<float>(nodeInstance["Position"]["Y"].GetInt());

			object->ConstructUniquePins();

			object->myVariable = nullptr;
			int VUID = nodeInstance["VUID"].GetInt();
			if (VUID >= 0)
			{
				for (int v = 0; v < myGraphVariables.size(); v++)
				{
					if (myGraphVariables[v].myUID.ToInt() == VUID)
					{
						object->myVariable = &myGraphVariables[v];
						object->ChangePinTypes(object->myVariable->GetType());
						break;
					}
				}
			}

			for (unsigned int z = 0; z < nodeInstance["Pins"].Size(); z++)
			{
				int index = nodeInstance["Pins"][z]["Index"].GetInt();
				object->myPins[index].UID.SetUID(nodeInstance["Pins"][z]["UID"].GetInt());
				DataType pinDataType = (DataType)nodeInstance["Pins"][z]["DATA_TYPE"].GetUint();

				rapidjson::Value& someData = nodeInstance["Pins"][z]["DATA"];
				const SizeType dataSize = someData.Size();
				if (dataSize > 0)
				{
					object->myPins[index].Data = DataPtr::Create(pinDataType, dataSize);
					std::vector<char> data(dataSize);
					for (SizeType d = 0; d < dataSize; d++)
					{
						data[d] = (char)someData[d].GetInt();
					}

					memcpy(object->myPins[index].Data.Ptr, &data.front(), data.size());
				}

				if (object->myPins[index].DataType == DataType::Unknown)
				{
					object->ChangePinTypes(pinDataType);
				}
			}

			myNodeInstancesInGraph.push_back(object);
		}
	}
	{
		std::ifstream inputFile("links.json");
		std::stringstream jsonDocumentBuffer;
		std::string inputLine;

		while (std::getline(inputFile, inputLine))
		{
			jsonDocumentBuffer << inputLine << "\n";
		}
		rapidjson::Document document;
		document.Parse(jsonDocumentBuffer.str().c_str());

		myNextLinkIdCounter = 0;
		for (rapidjson::SizeType i = 0; i < document["Links"].Size(); i++)
		{
			int id = document["Links"][i]["ID"].GetInt();
			int inputID = document["Links"][i]["Input"].GetInt();
			int Output = document["Links"][i]["Output"].GetInt();

			GraphNodeInstance* firstNode = GetNodeFromPinID(inputID);
			GraphNodeInstance* secondNode = GetNodeFromPinID(Output);

			firstNode->AddLinkToVia(secondNode, inputID, Output, id);
			secondNode->AddLinkToVia(firstNode, Output, inputID, id);

			myLinks.push_back({ ed::LinkId(id), ed::PinId(inputID), ed::PinId(Output) });
			if (myNextLinkIdCounter < id + 1)
			{
				myNextLinkIdCounter = id + 1;
			}

		}
	}
	*/
}

std::vector<int> myFlowsToBeShown;
void GraphManager::ShowFlow(int aLinkID)
{
	myFlowsToBeShown.push_back(aLinkID);
}

void DrawPinIcon(const GraphNodePin& pin, bool connected, int alpha)
{
	IconType iconType;
	DataType actualType = pin.DataType;
	if (actualType != DataType::Exec && pin.Parent->GetVariable())
	{
		actualType = pin.Parent->GetVariable()->GetType();
	}

	ImColor color = DataTypeColor::Get(actualType);
	color.Value.w = alpha / 255.0f;

	switch (actualType)
	{
	case DataType::Exec:     iconType = IconType::Flow;   break;
	case DataType::Bool:     iconType = IconType::Circle; break;
	case DataType::Int:      iconType = IconType::Circle; break;
	case DataType::Float:    iconType = IconType::Circle; break;
	case DataType::String:   iconType = IconType::Circle; break;
	case DataType::Unknown:  iconType = IconType::Circle; break;
	default:
		iconType = IconType::Circle; break;
		return;
	}
	const int s_PinIconSize = 20;
	ax::Widgets::Icon(ImVec2(s_PinIconSize, s_PinIconSize), iconType, connected, color, ImColor(32, 32, 32, alpha));
};

GraphNodeInstance* GraphManager::GetNodeFromNodeID(unsigned int aID)
{
	auto it = myNodeInstancesInGraph.begin();
	while (it != myNodeInstancesInGraph.end())
	{
		if ((*it)->myUID.ToUInt() == aID)
		{
			return *it;
		}
		else
		{
			++it;
		}
	}

	return nullptr;

}


GraphNodeInstance* GraphManager::GetNodeFromPinID(unsigned int aID)
{
	for (auto& nodeInstance : myNodeInstancesInGraph)
	{
		std::vector<GraphNodePin>& pins = nodeInstance->GetPins();

		for (auto& pin : pins)
		{
			if (pin.UID.ToUInt() == aID)
			{
				return nodeInstance;
			}
		}

	}
	return nullptr;
}


ImVec2 GraphManager::DrawTypeSpecificPin(GraphNodePin& aPin, GraphNodeInstance* aNodeInstance)
{
	ImVec2 width = ImVec2(0, 0);

	if (!aPin.AllowConstructInPlace)
	{
		ImGui::PushID(aPin.UID.ToUInt());
		ImGui::PushItemWidth(width.x = 100.0f);
		DrawPinIcon(aPin, aNodeInstance->IsPinConnected(aPin), 255);
		ImGui::PopItemWidth();
		ImGui::PopID();
	}
	else
	{
		switch (aPin.DataType)
		{
		case DataType::String:
		{
			if (!aPin.Data)
			{
				aPin.Data = DataPtr::Create(aPin.DataType, 128);
			}

			ImGui::PushID(aPin.UID.ToUInt());
			ImGui::PushItemWidth(width.x = 100.0f);
			DrawPinIcon(aPin, aNodeInstance->IsPinConnected(aPin), 255);
			ImGui::PopItemWidth();

			ImGui::PopID();
			break;
		}
		case DataType::Int:
		{
			if (!aPin.Data)
			{
				aPin.Data = DataPtr::Create(aPin.DataType);
			}
			int* c = ((int*)aPin.Data.Ptr);
			ImGui::PushID(aPin.UID.ToUInt());
			ImGui::PushItemWidth(width.x = 100.0f);
			DrawPinIcon(aPin, aNodeInstance->IsPinConnected(aPin), 255);
			ImGui::PopID();
			break;
		}
		case DataType::Bool:
		{
			if (!aPin.Data)
			{
				aPin.Data = DataPtr::Create(aPin.DataType);
			}
			bool* c = ((bool*)aPin.Data.Ptr);
			ImGui::PushID(aPin.UID.ToUInt());
			ImGui::PushItemWidth(width.x = 100.0f);
			DrawPinIcon(aPin, aNodeInstance->IsPinConnected(aPin), 255);
			ImGui::PopItemWidth();

			ImGui::PopID();
			break;
		}
		case DataType::Float:
		{
			if (!aPin.Data)
			{
				aPin.Data = DataPtr::Create(aPin.DataType);
				float* c = ((float*)aPin.Data.Ptr);
				*c = 1.0f;
			}
			float* c = ((float*)aPin.Data.Ptr);
			ImGui::PushID(aPin.UID.ToUInt());
			ImGui::PushItemWidth(width.x = 70.0f);
			DrawPinIcon(aPin, aNodeInstance->IsPinConnected(aPin), 255);
			ImGui::PopItemWidth();

			ImGui::PopID();
			break;
		}
		case DataType::Unknown:
		{
			ImGui::PushID(aPin.UID.ToInt());
			ImGui::PushItemWidth(width.x = 100.0f);

			int selectedIndex = -1;
			if (ImGui::RadioButton("Bool", false))
			{
				selectedIndex = (int)DataType::Bool;
			}
			if (ImGui::RadioButton("Int", false))
			{
				selectedIndex = (int)DataType::Int;
			}
			if (ImGui::RadioButton("Float", false))
			{
				selectedIndex = (int)DataType::Float;
			}
			if (ImGui::RadioButton("String", false))
			{
				selectedIndex = (int)DataType::String;
			}

			if (selectedIndex != -1)
			{
				GraphNodeInstance* instance = GetNodeFromPinID(aPin.UID.ToUInt());
				instance->ChangePinTypes((DataType)selectedIndex);
			}


			ImGui::PopItemWidth();
			ImGui::PopID();
			break;
		}
		case DataType::Variable:
		{
			break;
		}
		default:
			assert(false && "Don't know how to draw this particular type! Please nodify GraphManager::DrawTypeSpecificPin");
		}
	}
	return width;
}

ImVec2 GraphManager::DrawTypeSpecificInput(GraphNodePin& aPin, GraphNodeInstance* aNodeInstance)
{
	ImVec2 width = ImVec2(0, 0);

	if (!aPin.AllowConstructInPlace)
	{
		ImGui::PushID(aPin.UID.ToUInt());
		ImGui::PushItemWidth(width.x = 100.0f);
		DrawPinIcon(aPin, aNodeInstance->IsPinConnected(aPin), 255);
		ImGui::PopItemWidth();
		ImGui::PopID();
	}
	else
	{
		switch (aPin.DataType)
		{
		case DataType::String:
		{
			if (!aPin.Data)
			{
				aPin.Data = DataPtr::Create(aPin.DataType, 128);
			}

			ImGui::PushID(aPin.UID.ToUInt());
			ImGui::PushItemWidth(width.x = 100.0f);
			ImGui::InputText("##edit", (char*)aPin.Data.Ptr, 127);
			ImGui::PopItemWidth();

			ImGui::PopID();
			break;
		}
		case DataType::Int:
		{
			if (!aPin.Data)
			{
				aPin.Data = DataPtr::Create(aPin.DataType);
			}
			int* c = ((int*)aPin.Data.Ptr);
			ImGui::PushID(aPin.UID.ToUInt());
			ImGui::PushItemWidth(width.x = 100.0f);
			ImGui::InputInt("##edit", c);
			ImGui::PopItemWidth();
			ImGui::PopID();
			break;
		}
		case DataType::Bool:
		{
			if (!aPin.Data)
			{
				aPin.Data = DataPtr::Create(aPin.DataType);
			}
			bool* c = ((bool*)aPin.Data.Ptr);
			ImGui::PushID(aPin.UID.ToUInt());
			ImGui::PushItemWidth(width.x = 100.0f);
			ImGui::Checkbox("##edit", c);
			ImGui::PopItemWidth();

			ImGui::PopID();
			break;
		}
		case DataType::Float:
		{
			if (!aPin.Data)
			{
				aPin.Data = DataPtr::Create(aPin.DataType);
				float* c = ((float*)aPin.Data.Ptr);
				*c = 1.0f;
			}
			float* c = ((float*)aPin.Data.Ptr);
			ImGui::PushID(aPin.UID.ToUInt());
			ImGui::PushItemWidth(width.x = 70.0f);
			ImGui::InputFloat("##edit", c);
			ImGui::PopItemWidth();

			ImGui::PopID();
			break;
		}
		case DataType::Unknown:
		{
			ImGui::PushID(aPin.UID.ToInt());
			ImGui::PushItemWidth(width.x = 100.0f);

			int selectedIndex = -1;
			if (ImGui::RadioButton("Bool", false))
			{
				selectedIndex = (int)DataType::Bool;
			}
			if (ImGui::RadioButton("Int", false))
			{
				selectedIndex = (int)DataType::Int;
			}
			if (ImGui::RadioButton("Float", false))
			{
				selectedIndex = (int)DataType::Float;
			}
			if (ImGui::RadioButton("String", false))
			{
				selectedIndex = (int)DataType::String;
			}

			if (selectedIndex != -1)
			{
				GraphNodeInstance* instance = GetNodeFromPinID(aPin.UID.ToUInt());
				instance->ChangePinTypes((DataType)selectedIndex);
			}


			ImGui::PopItemWidth();
			ImGui::PopID();
			break;
		}
		case DataType::Variable:
		{
			break;
		}
		default:
			assert(false && "Don't know how to draw this particular type! Please nodify GraphManager::DrawTypeSpecificPin");
		}
	}
	return width;
}

void GraphManager::WillBeCyclic(GraphNodeInstance* aFirst, bool& aIsCyclic, GraphNodeInstance* aBase)
{
	if (aIsCyclic)
	{
		return;
	}

	std::vector<GraphNodePin>& pins = aFirst->GetPins();
	for (auto& pin : pins)
	{
		if (pin.Direction == PinDirection::PinDirection_OUT)
		{
			std::vector< GraphNodeEdge*> links = aFirst->GetLinksFromPin(pin.UID.ToUInt());
			if (links.size() == 0)
			{
				return;
			}
			for (auto& link : links)
			{
				GraphNodeInstance* connectedNodeToOutPut = GetNodeFromPinID(link->ToPinID);

				if (connectedNodeToOutPut == aBase)
				{
					aIsCyclic |= true;
					return;
				}
				WillBeCyclic(connectedNodeToOutPut, aIsCyclic, aBase);
			}
		}
	}
}

void GraphManager::PreFrame(float aTimeDelta)
{
	static float timer = 0;
	timer += aTimeDelta;
	auto& io = ImGui::GetIO();

	switch (myNewVarType)
	{
	case DataType::Bool:
		myNewVarTypeLabel = "Boolean";
		break;
	case DataType::Int:
		myNewVarTypeLabel = "Integer";
		break;
	case DataType::Float:
		myNewVarTypeLabel = "Float";
		break;
	case DataType::String:
		myNewVarTypeLabel = "String";
		break;
	default:
		myNewVarTypeLabel = "Select Type...";
		break;
	}

	// Modal Dialog for handling variables
	{
		ImGui::SetNextWindowSize(ImVec2(400, 275), ImGuiCond_Always);
		
		if (ImGui::BeginPopupModal("Manage Variables", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar/*, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings*/))
		{
			// Adding a new variable
			{
				ImGui::Text("Name");
				ImGui::SameLine();
				ImGui::Dummy({ 5, 0 });
				ImGui::SameLine();
				ImGui::PushItemWidth(180.0f);
				ImGui::InputText("##varName", myNewVarName, 255);
				ImGui::PopItemWidth();

				ImGui::Text("Type");
				ImGui::SameLine();
				ImGui::Dummy({ 10, 0 });
				ImGui::SameLine();
				if (ImGui::Button("Boolean", ImVec2(60, 0)))
				{
					myNewVarType = DataType::Bool;
				}
				ImGui::SameLine();
				if (ImGui::Button("Int", ImVec2(60, 0)))
				{
					myNewVarType = DataType::Int;
				}
				ImGui::SameLine();
				if (ImGui::Button("Float", ImVec2(60, 0)))
				{
					myNewVarType = DataType::Float;
				}
				ImGui::SameLine();
				if (ImGui::Button("String", ImVec2(60, 0)))
				{
					myNewVarType = DataType::String;
				}

				//Selectable doesn't work?
				//I want to use this instead of buttons, bror

				/*
				ImGui::Text("Type");
				ImGui::SameLine();
				ImGui::Dummy({ 10, 0 });
				ImGui::SameLine();
				if (ImGui::BeginCombo("##variableCombo", myNewVarTypeLabel.c_str()))
				{
					ax::Widgets::Icon(ImVec2(18, 18), IconType::Circle, true, DataTypeColor::Get(DataType::Bool), ImColor(32, 32, 32, 128));
					ImGui::SameLine();
					if (ImGui::Selectable("Boolean##variable", myNewVarType == DataType::Bool))
					{
						myNewVarType = DataType::Bool;
					}
					ax::Widgets::Icon(ImVec2(18, 18), IconType::Circle, true, DataTypeColor::Get(DataType::Int), ImColor(32, 32, 32, 128));
					ImGui::SameLine();
					if (ImGui::Selectable("Int##variable", myNewVarType == DataType::Int))
					{
						myNewVarType = DataType::Int;
					}
					ax::Widgets::Icon(ImVec2(18, 18), IconType::Circle, true, DataTypeColor::Get(DataType::Float), ImColor(32, 32, 32, 128));
					ImGui::SameLine();
					if (ImGui::Selectable("Float##variable", myNewVarType == DataType::Float))
					{
						myNewVarType = DataType::Float;
					}
					ax::Widgets::Icon(ImVec2(18, 18), IconType::Circle, true, DataTypeColor::Get(DataType::String), ImColor(32, 32, 32, 128));
					ImGui::SameLine();
					if (ImGui::Selectable("String##variable", myNewVarType == DataType::String))
					{
						myNewVarType = DataType::String;
					}
					ImGui::EndCombo();
				}
				*/

				ImGui::Text("Type");
				ImGui::SameLine();
				ImGui::Dummy({ 15, 0 });
				ImGui::SameLine();
				if (myNewVarType != DataType::Unknown)
				{
					ImGui::Dummy({ -10, 0 });
					ImGui::SameLine();
					ax::Widgets::Icon(ImVec2(18, 18), IconType::Circle, true, DataTypeColor::Get(myNewVarType), ImColor(32, 32, 32, 128));
					ImGui::SameLine();
				}
				ImGui::Text(myNewVarTypeLabel.c_str());

				ImGui::SameLine();
				ImGui::Dummy({ 15, 0 });
				ImGui::SameLine();

				const float itemSpacing = ImGui::GetStyle().ItemSpacing.x;

				float buttonWidth = 100.0f;
				ImVec2 cursor = ImGui::GetCursorPos();
				cursor.x = ImGui::GetWindowWidth() - (buttonWidth + itemSpacing);
				ImGui::SetCursorPos(cursor);
				ImGui::PushItemWidth(buttonWidth);

				bool varNameExists = false;
				if (strlen(myNewVarName) > 0)
				{
					const std::string currentNewVarName(myNewVarName);
					for (auto& var : myGraphVariables)
					{
						if (std::equal(var.myName.begin(), var.myName.end(), currentNewVarName.begin(), currentNewVarName.end(), [](char a, char b) { return toupper(a) == toupper(b); }))
						{
							varNameExists = true;
							break;
						}
					}
				}

				const bool isAddEnabled = strlen(myNewVarName) > 0 && myNewVarType != DataType::Unknown && !varNameExists;

				if (!isAddEnabled)
				{
					ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.6f);
					ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
				}

				if (ImGui::Button("Add", { buttonWidth, 0 }))
				{
					if (strlen(myNewVarName) != 0)
					{
						std::string newVariableName(myNewVarName);
						myGraphVariables.push_back(GraphVariable(newVariableName, myNewVarType));
						memset(myNewVarName, 0, 255);
						myNewVarType = DataType::Unknown;
					}
				}

				if (!isAddEnabled)
				{
					ImGui::PopItemFlag();
					ImGui::PopStyleVar();
				}

				ImGui::PopItemWidth();
			}

			// Variable list
			{
				ImGui::Text("Variables");
				if (ImGui::ListBoxHeader("##lbVariables", { -FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing() }))
				{
					for (int v = 0; v < myGraphVariables.size(); v++)
					{
						auto& var = myGraphVariables[v];

						ax::Widgets::Icon(ImVec2(18, 18), IconType::Circle, true, DataTypeColor::Get(var.GetType()), ImColor(32, 32, 32, 128));
						ImGui::SameLine();
						if (ImGui::Selectable(var.GetName().c_str(), v == mySelectedVariableIndex))
						{
							mySelectedVariableIndex = v;
							myVariableErrorMessage = "";
						}
					}
					ImGui::ListBoxFooter();
				}

				const float itemSpacing = ImGui::GetStyle().ItemSpacing.x;

				float buttonWidth = 100.0f;
				ImVec2 cursor = ImGui::GetCursorPos();
				cursor.x = ImGui::GetWindowWidth() - (buttonWidth + itemSpacing) * 2;
				ImGui::SetCursorPos(cursor);
				ImGui::PushItemWidth(buttonWidth);

				const bool isRemoveEnabled = mySelectedVariableIndex > -1;

				if (!isRemoveEnabled)
				{
					ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.6f);
					ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
				}

				if (ImGui::Button("Remove", { buttonWidth, 0 }))
				{
					// We need to check if any nodes use this variable!
					auto& currentVar = myGraphVariables[mySelectedVariableIndex];
					bool canDelete = true;
					for (auto& node : myNodeInstancesInGraph)
					{
						if (node->GetVariable() && node->GetVariable()->GetUID().ToInt() == currentVar.GetUID().ToInt()) // What xP
						{
							myVariableErrorMessage = "Cannot delete a variable that is used in the graph!";
							canDelete = false;
							break;
						}
					}

					if (canDelete)
					{
						myGraphVariables.erase(myGraphVariables.begin() + mySelectedVariableIndex);

						mySelectedVariableIndex = -1;
						myVariableErrorMessage = "";
					}
				}

				if (!isRemoveEnabled)
				{
					ImGui::PopItemFlag();
					ImGui::PopStyleVar();
				}

				ImGui::SameLine();
				if (ImGui::Button("Close", { buttonWidth, 0 }))
				{
					myVariableErrorMessage = "";
					memset(myNewVarName, 0, 255);
					myNewVarType = DataType::Unknown;
					mySelectedVariableIndex = -1;
					ImGui::CloseCurrentPopup();
				}

				ImGui::TextColored({ 255, 0, 0, 255 }, myVariableErrorMessage.c_str());
			}

			ImGui::EndPopup();
		}
	}

	if (ImGui::Button("Retrigger"))
	{
		ReTriggerTree("Start");
	}
	ImGui::SameLine();
	if (ImGui::Button("Save"))
	{
		myLikeToSave = true;

	}
	ImGui::SameLine();
	if (ImGui::Button("Load"))
	{
		LoadTreeFromFile();
	}
	static bool showFlow = false;
	if (ImGui::Checkbox("Show Flow", &showFlow))
	{
		myLikeToShowFlow = showFlow;

	}
	ImGui::SameLine();

	if (ImGui::Button("Variables"))
	{
		ImGui::OpenPopup("Manage Variables");
	}

	for (auto& nodeInstance : myNodeInstancesInGraph)
	{
		nodeInstance->DebugUpdate();

		nodeInstance->VisualUpdate(aTimeDelta);
	}

	static int outRate = 30;
	ImGui::SliderInt("Max framerate", &outRate, 0, 100);
	if (timer > (1.0f / (float)outRate))
	{
		ReTriggerUpdateringTrees();
		timer = 0;
	}

	ed::SetCurrentEditor(g_Context);
	ed::Begin("NodeEditor##GraphManager");
}

bool ArePinTypesCompatible(GraphNodePin& aFirst, GraphNodePin& aSecond)
{
	if ((aFirst.DataType == DataType::Exec && aSecond.DataType != DataType::Exec))
	{
		return false;
	}
	if ((aSecond.DataType == DataType::Exec && aFirst.DataType != DataType::Exec))
	{
		return false;
	}
	if (aFirst.IsReadOnly == true || aSecond.IsReadOnly == true)
		return false;
	return true;
}


struct SDistBestResult
{
	size_t myScore;
	GraphNodeBase* myInstance;
};

struct less_than_key
{
	inline bool operator() (const SDistBestResult& struct1, const SDistBestResult& struct2)
	{
		return (struct1.myScore < struct2.myScore);
	}
};

template<typename T>
size_t uiLevenshteinDistance(const T& source, const T& target)
{
	if (source.size() > target.size())
	{
		return uiLevenshteinDistance(target, source);
	}

	using TSizeType = typename T::size_type;
	const TSizeType min_size = source.size(), max_size = target.size();
	std::vector<TSizeType> lev_dist(min_size + 1);

	for (TSizeType i = 0; i <= min_size; ++i)
	{
		lev_dist[i] = i;
	}

	for (TSizeType j = 1; j <= max_size; ++j)
	{
		TSizeType previous_diagonal = lev_dist[0], previous_diagonal_save;
		++lev_dist[0];

		for (TSizeType i = 1; i <= min_size; ++i)
		{
			previous_diagonal_save = lev_dist[i];
			if (source[i - 1] == target[j - 1])
			{
				lev_dist[i] = previous_diagonal;
			}
			else
			{
				lev_dist[i] = (std::min)((std::min)(lev_dist[i - 1], lev_dist[i]), previous_diagonal) + 1;
			}
			previous_diagonal = previous_diagonal_save;
		}
	}

	return lev_dist[min_size];
}

void GraphManager::ConstructEditorTreeAndConnectLinks()
{
	for (auto& nodeInstance : myNodeInstancesInGraph)
	{
		if (!nodeInstance->myHasSetEditorPos)
		{
			ed::SetNodePosition(nodeInstance->myUID.ToUInt(), ImVec2(nodeInstance->myEditorPos[0], nodeInstance->myEditorPos[1]));
			nodeInstance->myHasSetEditorPos = true;
		}

		// Start drawing nodes.
		ed::PushStyleVar(ed::StyleVar_NodeRounding, 3.0f);
		ed::PushStyleVar(ed::StyleVar_NodePadding, ImVec4(8, 4, 8, 8));
		ed::BeginNode(nodeInstance->myUID.ToUInt());
		ImGui::PushID(nodeInstance->myUID.ToUInt());
		ImGui::TextUnformatted(nodeInstance->GetNodeName().c_str());
		ax::rect HeaderRect = ImGui_GetItemRect();

		std::vector<GraphNodePin*> InputPins;
		std::vector<GraphNodePin*> OutputPins;
		std::vector<GraphNodePin>& nodePins = nodeInstance->GetPins();
		int firstInputExec = -1;
		int firstOutputExec = -1;
		for (size_t p = 0; p < nodePins.size(); p++)
		{
			if (nodePins[p].Direction == PinDirection::PinDirection_IN)
			{
				InputPins.push_back(&nodePins[p]);
				if (nodePins[p].DataType == DataType::Exec && firstInputExec == -1)
					firstInputExec = static_cast<int>(InputPins.size() - 1);
			}
			else
			{
				OutputPins.push_back(&nodePins[p]);
				if (nodePins[p].DataType == DataType::Exec && firstOutputExec == -1)
					firstOutputExec = static_cast<int>(OutputPins.size() - 1);
			}
		}

		const bool bDrawExecRow = firstInputExec + firstOutputExec >= -1;

		if (bDrawExecRow)
		{
			ImVec2 inputNameSize = ImVec2(0, 0);

			if (firstInputExec > -1)
			{
				GraphNodePin* pin = InputPins[firstInputExec];
				inputNameSize = ImGui::CalcTextSize(pin->Text.c_str());

				ed::BeginPin(pin->UID.ToUInt(), ed::PinKind::Input);
				DrawPinIcon(*pin, nodeInstance->IsPinConnected(*pin), 255);
				ed::EndPin();
				ImGui::SameLine();
				ImGui::Text(pin->Text.c_str());
			}
			if (firstOutputExec > -1)
			{
				if (firstInputExec > -1)
				{
					ImGui::SameLine();
				}
				GraphNodePin* pin = OutputPins[firstOutputExec];

				const ImVec2 outputNameSize = ImGui::CalcTextSize(pin->Text.c_str());
				ImGui::Dummy({ 117.5f - outputNameSize.x - inputNameSize.x, 0 });
				ImGui::SameLine();

				ImGui::Text(pin->Text.c_str());
				ImGui::SameLine();
				ed::BeginPin(pin->UID.ToUInt(), ed::PinKind::Output);
				DrawPinIcon(*pin, nodeInstance->IsPinConnected(*pin), 255);
				ed::EndPin();
			}
		}

		const size_t numRows = InputPins.size() > OutputPins.size() ? InputPins.size() : OutputPins.size();
		
		for (size_t row = 0; row < numRows; row++)
		{
			if (row < OutputPins.size() && row != firstOutputExec)
			{
				GraphNodePin* pin = OutputPins[row];

				const ImVec2 pinNameSize = ImGui::CalcTextSize(pin->Text.c_str());
				ImGui::Dummy({ 150.0f - pinNameSize.x, 0 });
				ImGui::SameLine();

				ImGui::Text(pin->Text.c_str());
				ImGui::SameLine();
				ed::BeginPin(pin->UID.ToUInt(), ed::PinKind::Output);
				DrawPinIcon(*pin, nodeInstance->IsPinConnected(*pin), 255);
				ed::EndPin();
			}
		}
		
		for (size_t row = 0; row < numRows; row++)
		{
			if (row < InputPins.size() && row != firstInputExec) // Should we draw input?
			{
				GraphNodePin* pin = InputPins[row];

				if (pin->DataType == DataType::Exec)
				{
					ed::BeginPin(pin->UID.ToUInt(), ed::PinKind::Input);
					DrawPinIcon(*pin, nodeInstance->IsPinConnected(*pin), 255);
					ed::EndPin();
				}
				else
				{
					if (nodeInstance->IsPinConnected(*pin) || !pin->AllowConstructInPlace)
					{
						ed::BeginPin(pin->UID.ToUInt(), ed::PinKind::Input);
						/*ImVec2 size = */DrawTypeSpecificPin(*pin, nodeInstance);
						ed::EndPin();
						ImGui::SameLine();
						ImGui::Text(pin->Text.c_str());
						//ImGui::SameLine();
						//ImGui::Dummy(size);
					}
					else
					{
						ed::BeginPin(pin->UID.ToUInt(), ed::PinKind::Input);
						DrawTypeSpecificPin(*pin, nodeInstance);
						ed::EndPin();
						ImGui::SameLine();
						ImGui::Text(pin->Text.c_str());
						ImGui::SameLine();
						DrawTypeSpecificInput(*pin, nodeInstance);
					}
				}
			}
		}
		

		ImVec2 size = ed::GetNodeSize(nodeInstance->myUID.myID);

		HeaderRect = ax::rect(HeaderRect.x, HeaderRect.y, static_cast<int>(size.x - 8), HeaderRect.h);
		ed::EndNode();

		if (ImGui::IsItemVisible())
		{
			auto drawList = ed::GetNodeBackgroundDrawList(nodeInstance->myUID.ToUInt());
			
			const auto halfBorderWidth = ed::GetStyle().NodeBorderWidth * 0.5f;
			auto headerColor = nodeInstance->GetColor();

			static Dyn::SRV* image = Dyn::ResourceFactory::GetSRV("Assets/Editor/NodeEditor/Sprites/BlueprintBackground.dds");

			drawList->AddImage(image->Get(),
				to_imvec(HeaderRect.top_left()) - ImVec2(8 - halfBorderWidth, 4 - halfBorderWidth),
				to_imvec(HeaderRect.bottom_right()) + ImVec2(-(2 - halfBorderWidth), 0),
				ImVec2(0.0f, 0.0f), ImVec2(0.8f, 0.1f), headerColor);
			
			// Fixed UVs here
			//drawList->AddImageRounded(image,
			//	to_imvec(HeaderRect.top_left()) - ImVec2(8 - halfBorderWidth, 4 - halfBorderWidth),
			//	to_imvec(HeaderRect.bottom_right()) + ImVec2(8 - halfBorderWidth, 0),
			//	ImVec2(0.0f, 0.0f), ImVec2(0.99f, 0.1f),
			//	headerColor, ed::GetStyle().NodeRounding, 
			//	ImDrawFlags_Closed | ImDrawListFlags_AntiAliasedLines);
			

			// Fixed this to align line at bottom of header
			
			auto headerSeparatorRect = ax::rect(HeaderRect.bottom_left(), HeaderRect.bottom_right());
			drawList->AddLine(
				to_imvec(headerSeparatorRect.top_left()) + ImVec2(-(8 - halfBorderWidth), -0.5f),
				to_imvec(headerSeparatorRect.top_right()) + ImVec2((-2 - halfBorderWidth), -0.5f),
				ImColor(255, 255, 255, 255), 1.0f);
		}
		ImGui::PopID();
		ed::PopStyleVar();
	}


	for (auto& linkInfo : myLinks)
		ed::Link(linkInfo.Id, linkInfo.InputId, linkInfo.OutputId);

	// Handle creation action, returns true if editor want to create new object (node or link)
	if (ed::BeginCreate())
	{
		ed::PinId inputPinId, outputPinId;
		if (ed::QueryNewLink(&inputPinId, &outputPinId))
		{
			if (inputPinId && outputPinId)
			{
				if (ed::AcceptNewItem())
				{
					GraphNodeInstance* firstNode = GetNodeFromPinID(static_cast<int>(inputPinId.Get()));
					GraphNodeInstance* secondNode = GetNodeFromPinID(static_cast<int>(outputPinId.Get()));
					assert(firstNode);
					assert(secondNode);

					if (firstNode == secondNode)
					{
						// User trying connect input and output on the same node :/, who does this!?!
						// SetBlueScreenOnUserComputer(true)
					}
					else
					{
						{
							GraphNodePin* firstPin = firstNode->GetPinFromID(static_cast<int>(inputPinId.Get()));
							GraphNodePin* secondPin = secondNode->GetPinFromID(static_cast<int>(outputPinId.Get()));

							bool canAddlink = true;
							if (firstPin && secondPin)
							{
								if (firstPin->Direction == PinDirection::PinDirection_IN && secondPin->Direction == PinDirection::PinDirection_IN)
								{
									canAddlink = false;
								}
							}

							if (!ArePinTypesCompatible(*firstPin, *secondPin))
							{
								canAddlink = false;
							}

							if (!firstNode->CanAddLink(static_cast<int>(inputPinId.Get())))
							{
								canAddlink = false;
							}
							if (!secondNode->CanAddLink(static_cast<int>(outputPinId.Get())))
							{
								canAddlink = false;
							}

							if (firstNode->HasLinkBetween(static_cast<int>(inputPinId.Get()), static_cast<int>(outputPinId.Get())))
							{
								canAddlink = false;
							}


							if (canAddlink)
							{
								if (secondPin->DataType == DataType::Unknown)
								{
									secondNode->ChangePinTypes(firstPin->DataType);
								}

								if (firstNode->IsPinConnected(*firstPin) && firstPin->DataType == DataType::Exec)
								{
									std::vector<GraphNodeEdge*> firstNodeLinks = firstNode->GetLinksFromPin(firstPin->UID.ToUInt());
									for (auto firstNodeLink : firstNodeLinks)
									{
										if (firstNodeLink->Target != firstNode)
										{
											firstNodeLink->Target->RemoveLinkToVia(firstNode, firstNodeLink->ToPinID);
											firstNode->RemoveLinkToVia(firstNodeLink->Target, firstNodeLink->FromPinID);
											for (auto& link : myLinks)
											{
												if (link.Id.Get() == firstNodeLink->LinkID)
												{
													myLinks.erase(&link);
												}
											}
										}
									}
								}

								if (secondNode->IsPinConnected(*secondPin) && secondPin->DataType == DataType::Exec)
								{
									std::vector<GraphNodeEdge*> secondNodeLinks = secondNode->GetLinksFromPin(secondPin->UID.ToUInt());
									for (auto secondNodeLink : secondNodeLinks)
									{
										if (secondNodeLink->Target != secondNode)
										{
											secondNodeLink->Target->RemoveLinkToVia(secondNode, secondNodeLink->ToPinID);
											secondNode->RemoveLinkToVia(secondNodeLink->Target, secondNodeLink->FromPinID);
											for (auto& link : myLinks)
											{
												if (link.Id.Get() == secondNodeLink->LinkID)
												{
													myLinks.erase(&link);
												}
											}
										}
									}
								}


								int linkId = myNextLinkIdCounter++;
								firstNode->AddLinkToVia(secondNode, static_cast<int>(inputPinId.Get()), static_cast<int>(outputPinId.Get()), linkId);
								secondNode->AddLinkToVia(firstNode, static_cast<int>(outputPinId.Get()), static_cast<int>(inputPinId.Get()), linkId);

								bool aIsCyclic = false;
								WillBeCyclic(firstNode, aIsCyclic, firstNode);
								if (aIsCyclic || !canAddlink)
								{
									firstNode->RemoveLinkToVia(secondNode, static_cast<int>(inputPinId.Get()));
									secondNode->RemoveLinkToVia(firstNode, static_cast<int>(outputPinId.Get()));
								}
								else
								{
									// Depending on if you drew the new link from the output to the input we need to create the link as the flow FROM->TO to visualize the correct flow
									if (firstPin->Direction == PinDirection::PinDirection_IN)
									{
										myLinks.push_back({ ed::LinkId(linkId), outputPinId, inputPinId });
									}
									else
									{
										myLinks.push_back({ ed::LinkId(linkId), inputPinId, outputPinId });
									}

									//ReTriggerTree();
								}
							}
						}
					}
				}
			}
		}
	}
	ed::EndCreate();

	// Handle deletion action
	if (ed::BeginDelete())
	{
		// There may be many links marked for deletion, let's loop over them.
		ed::LinkId deletedLinkId;
		while (ed::QueryDeletedLink(&deletedLinkId))
		{
			// If you agree that link can be deleted, accept deletion.
			if (ed::AcceptDeletedItem())
			{
				// Then remove link from your data.
				for (auto& link : myLinks)
				{
					if (link.Id == deletedLinkId)
					{
						GraphNodeInstance* firstNode = GetNodeFromPinID(static_cast<int>(link.InputId.Get()));
						GraphNodeInstance* secondNode = GetNodeFromPinID(static_cast<int>(link.OutputId.Get()));
						assert(firstNode);
						assert(secondNode);

						firstNode->RemoveLinkToVia(secondNode, static_cast<int>(link.InputId.Get()));
						secondNode->RemoveLinkToVia(firstNode, static_cast<int>(link.OutputId.Get()));

						myLinks.erase(&link);

						break;
					}
				}
			}
		}
		ed::NodeId nodeId = 0;
		while (ed::QueryDeletedNode(&nodeId))
		{
			if (ed::AcceptDeletedItem())
			{
				auto it = myNodeInstancesInGraph.begin();
				while (it != myNodeInstancesInGraph.end())
				{
					if ((*it)->myUID.ToUInt() == nodeId.Get())
					{
						it = myNodeInstancesInGraph.erase(it);
					}
					else
					{
						++it;
					}
				}

			}
		}
	}
	ed::EndDelete();

	auto openPopupPosition = ImGui::GetMousePos();
	ed::Suspend();

	if (ed::ShowBackgroundContextMenu())
	{
		ImGui::OpenPopup("Create New Node");
	}
	ed::Resume();

	ed::Suspend();
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 8));

	if (ImGui::BeginPopup("Create New Node"))
	{
		auto newNodePostion = openPopupPosition;
		GraphNodeBase** types = GraphNodeRegistry::GetAllNodeTypes();
		unsigned short noOfTypes = GraphNodeRegistry::GetNodeTypeCount();

		std::map< std::string, std::vector<GraphNodeBase*>> cats;

		for (int i = 0; i < noOfTypes; i++)
		{
			cats[types[i]->GetNodeTypeCategory()].push_back(types[i]);
		}

		ImGui::PushItemWidth(100.0f);
		ImGui::InputText("##edit", (char*)myMenuSeachField, 127);
		if (mySetSearchFokus)
		{
			ImGui::SetKeyboardFocusHere(0);
		}
		mySetSearchFokus = false;
		ImGui::PopItemWidth();

		if (myMenuSeachField[0] != '\0')
		{
			std::vector<SDistBestResult> distanceResults;
			for (int i = 0; i < noOfTypes; i++)
			{
				distanceResults.push_back(SDistBestResult());
				SDistBestResult& dist = distanceResults.back();
				dist.myInstance = types[i];
				dist.myScore = uiLevenshteinDistance<std::string>(types[i]->GetNodeName(), myMenuSeachField);
			}

			std::sort(distanceResults.begin(), distanceResults.end(), less_than_key());

			int firstCost = static_cast<int>(distanceResults[0].myScore);
			for (int i = 0; i < distanceResults.size(); i++)
			{
				GraphNodeInstance* node = nullptr;
				if (ImGui::MenuItem(distanceResults[i].myInstance->GetNodeName().c_str()))
				{
					node = DBG_NEW GraphNodeInstance(this);

					node->myNodeType = distanceResults[i].myInstance;
					node->ConstructUniquePins();
					ed::SetNodePosition(node->myUID.ToUInt(), newNodePostion);
					node->myHasSetEditorPos = true;

					myNodeInstancesInGraph.push_back(node);
				}
				int distance = static_cast<int>(distanceResults[i].myScore) - firstCost;
				if (distance > 3)
				{
					break;
				}
			}
		}
		else
		{
			for (auto& category : cats)
			{
				std::string theCatName = category.first;
				if (theCatName.empty())
				{
					theCatName = "General";
				}

				if (ImGui::BeginMenu(theCatName.c_str()))
				{
					GraphNodeInstance* node = nullptr;
					for (int i = 0; i < category.second.size(); i++)
					{
						GraphNodeBase* type = category.second[i];

						if (type->IsHiddenFromMenu())
							continue;

						if (ImGui::MenuItem(type->GetNodeName().c_str()))
						{
							node = DBG_NEW GraphNodeInstance(this);

							node->myNodeType = type;
							node->ConstructUniquePins();
							ed::SetNodePosition(node->myUID.ToUInt(), newNodePostion);
							node->myHasSetEditorPos = true;

							myNodeInstancesInGraph.push_back(node);
						}
					}
					ImGui::EndMenu();
				}
			}

			if (ImGui::BeginMenu("Variables"))
			{
				GraphNodeInstance* node = nullptr;
				for (size_t i = 0; i < myGraphVariables.size(); i++)
				{
					if (ImGui::BeginMenu(myGraphVariables[i].GetName().c_str()))
					{
						if (ImGui::MenuItem("Get"))
						{
							node = DBG_NEW GraphNodeInstance(this);
							node->myNodeType = GraphNodeRegistry::GetNodeTypePtrFromType<GetNode>();
							node->ConstructUniquePins();
							ed::SetNodePosition(node->myUID.ToUInt(), newNodePostion);
							node->myHasSetEditorPos = true;
							node->myVariable = &myGraphVariables[i];
							node->ChangePinTypes(myGraphVariables[i].GetType());
							myNodeInstancesInGraph.push_back(node);
						}
						if (ImGui::MenuItem("Set"))
						{
							node = DBG_NEW GraphNodeInstance(this);
							node->myNodeType = GraphNodeRegistry::GetNodeTypePtrFromType<SetNode>();
							node->ConstructUniquePins();
							ed::SetNodePosition(node->myUID.ToUInt(), newNodePostion);
							node->myHasSetEditorPos = true;
							node->myVariable = &myGraphVariables[i];
							node->ChangePinTypes(myGraphVariables[i].GetType());
							myNodeInstancesInGraph.push_back(node);
						}
						ImGui::EndMenu();
					}
				}
				node = nullptr;
				ImGui::EndMenu();
			}
		}
		ImGui::EndPopup();
	}
	else
	{
		mySetSearchFokus = true;
		memset(&myMenuSeachField[0], 0, sizeof(myMenuSeachField));
	}

	ImGui::PopStyleVar();
	ed::Resume();
}

void GraphManager::PostFram()
{
	if (myLikeToSave)
	{
		myLikeToSave = false;
		SaveTreeToFile();
	}
	if (myLikeToShowFlow)
	{
		for (int i = 0; i < myLinks.size(); i++)
		{
			ed::Flow(myLinks[i].Id);
		}
	}
	for (auto i : myFlowsToBeShown)
	{
		ed::Flow(i);
	}
	myFlowsToBeShown.clear();
	ed::End();
	ed::SetCurrentEditor(nullptr);
}

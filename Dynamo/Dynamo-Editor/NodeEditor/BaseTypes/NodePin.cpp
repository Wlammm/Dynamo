#include "EditorPch.h"
#include "NodePin.h"

#include "NodeEditor/Manager/NodeEditorManager.h"

Editor::NodePin::NodePin(const std::string& aLabel, const PinType aType, const PinData aDataType, bool isReadOnly)
{
	myLabel = aLabel;
	myPinType = aType;
	myPinData = aDataType;
	myIsReadOnly = isReadOnly;
	myUID = NodeEditorManager::GetPinUID();
}

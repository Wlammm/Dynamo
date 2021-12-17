#include "EditorPch.h"
#include "GraphNodeRegistry.h"
#include "NodeEditor/BaseTypes/TemplateHelper.h"

// Base Nodes
#include "NodeEditor/Nodes/Base/StartNode.h"
#include "NodeEditor/Nodes/Variable/VariableNode.h"

// Debug Nodes
#include "NodeEditor/Nodes/Debug/PrintNode.h"

// Math Nodes
#include "NodeEditor/Nodes/MathNodes/AddNode.h"


void GraphNodeRegistry::PopulateTypes()
{
	// Base Nodes
	TemplateHelper::RegisterType<StartNode>();
	TemplateHelper::RegisterType<SetNode>();
	TemplateHelper::RegisterType<GetNode>();

	// Debug Nodes
	TemplateHelper::RegisterType<PrintNode>();

	// Math
	TemplateHelper::RegisterType<AddNode>();
}


GraphNodeBase* GraphNodeRegistry::myTypes[128];
unsigned short GraphNodeRegistry::myTypeCounter = 0;
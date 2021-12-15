#include "EditorPch.h"
#include "GraphNodeRegistry.h"

#include "NodeEditor/Nodes/Variable/VariableNode.h"

#include "NodeEditor/Nodes/MathNodes/AddNode.h"

#include "NodeEditor/BaseTypes/TemplateHelper.h"

void GraphNodeRegistry::PopulateTypes()
{
	// Variable Nodes
	TemplateHelper::RegisterType<SetNode>();
	TemplateHelper::RegisterType<GetNode>();

	// Math
	TemplateHelper::RegisterType<AddNode>();
}


GraphNodeBase* GraphNodeRegistry::myTypes[128];
unsigned short GraphNodeRegistry::myTypeCounter = 0;
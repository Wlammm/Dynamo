#include "EditorPch.h"
#include "GraphNodeRegistry.h"

#include "NodeEditor/Nodes/Variable/VariableNode.h"

#include "NodeEditor/Nodes/MathNodes/AddNode.h"

void GraphNodeRegistry::PopulateTypes()
{
	// Variable Nodes
	RegisterType<SetNode>();
	RegisterType<GetNode>();

	// Math
	RegisterType<AddNode>();
}

GraphNodeBase* GraphNodeRegistry::myTypes[128];
unsigned short GraphNodeRegistry::myTypeCounter = 0;
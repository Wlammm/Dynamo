#include "EditorPch.h"
#include "NodeRegistry.h"

// Math Nodes
#include "NodeEditor/Nodes/MathNodes/AddNode.h"
#include "NodeEditor/Nodes/MathNodes/SubNode.h"

Editor::NodeRegistry::~NodeRegistry()
{
	for (int i = 0; i < myTypeCounter; i++)
	{
		delete myNodeTypes[i];
		myNodeTypes[i] = nullptr;
	}
}

void Editor::NodeRegistry::PopulateTypes()
{
	// Math Nodes
	RegisterType<AddNode>();
	RegisterType<SubNode>();

}

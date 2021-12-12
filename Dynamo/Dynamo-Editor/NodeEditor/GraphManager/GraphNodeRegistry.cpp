#include "EditorPch.h"
#include "GraphNodeRegistry.h"


	void GraphNodeRegistry::PopulateTypes()
	{
		//RegisterType<GraphNode_Print>();
		//RegisterType<GraphNode_Start>();
		//RegisterType<GraphNode_MathAdd>();
		//RegisterType<GraphNode_Set>();
		//RegisterType<GraphNode_Get>();
	}

	GraphNodeBase* GraphNodeRegistry::myTypes[128];
	unsigned short GraphNodeRegistry::myTypeCounter = 0;

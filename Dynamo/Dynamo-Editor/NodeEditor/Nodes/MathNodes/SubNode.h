#pragma once
#include "NodeEditor/Nodes/BaseNode.h"

namespace Editor
{
	class SubNode : public BaseNode
	{
	public:
		SubNode();

		int Enter(NodeInstance* aNodeInstance) override;
	};
}
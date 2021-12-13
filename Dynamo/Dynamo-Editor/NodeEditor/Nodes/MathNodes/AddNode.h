#pragma once
#include "../BaseNode.h"

namespace Editor
{
	class AddNode : public BaseNode
	{
	public:
		AddNode();

		int Enter(NodeInstance* aNodeInstance) override;

	};
}


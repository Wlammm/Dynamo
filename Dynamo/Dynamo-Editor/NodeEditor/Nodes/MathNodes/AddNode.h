#pragma once
#include "../GraphNodeBase.h"

namespace Editor
{
	class AddNode : public GraphNodeBase
	{
	public:
		AddNode();

		int OnExec(class GraphNodeInstance* aNodeInstance) override;

		virtual std::string GetNodeName() const override { return "Add"; }
		virtual std::string GetNodeTypeCategory() const override { return "Math"; }
	};
}

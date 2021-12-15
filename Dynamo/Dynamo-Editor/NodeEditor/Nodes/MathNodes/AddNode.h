#pragma once
#include "../GraphNodeBase.h"
#include "NodeEditor/BaseTypes/NodeTypes.h"

class AddNode : GraphNodeBase
{
public:
	AddNode();

	int OnExec(class GraphNodeInstance* aNodeInstance) override;

	virtual std::string GetNodeName() const override { return "Add"; }
	virtual std::string GetNodeTypeCategory() const override { return "Math"; }
};


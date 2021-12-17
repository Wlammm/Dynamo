#pragma once
#include "../GraphNodeBase.h"

class PrintNode : public GraphNodeBase
{
public:
	PrintNode();

	int OnExec(class GraphNodeInstance* aNodeInstance) override;

	virtual std::string GetNodeName() const override { return "Print"; }
};


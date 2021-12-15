#pragma once
#include "../GraphNodeBase.h"

class SetNode : public GraphNodeBase
{

public:
	SetNode();

	virtual int OnExec(GraphNodeInstance* aNodeInstance) override;
	virtual std::string GetInstanceName(const GraphNodeInstance* aNodeInstance) const override;
};

class GetNode : public GraphNodeBase
{
public:

	GetNode();

	virtual int OnExec(GraphNodeInstance* aNodeInstance) override;
	virtual std::string GetInstanceName(const GraphNodeInstance* aNodeInstance) const override;
};


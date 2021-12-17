#pragma once
#include "../GraphNodeBase.h"

namespace Editor
{
	class StartNode : public GraphNodeBase
	{
	public:
		StartNode();

		int OnExec(class GraphNodeInstance* aNodeInstance) override;

		bool IsStartNode() const override { return true; }

		virtual std::string GetNodeName() const override { return "Start"; }
	};
}

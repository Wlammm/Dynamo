#pragma once
#include "NodeEditor/Nodes/BaseNode.h"

namespace Editor
{
	class NodeRegistry
	{
	public:
		NodeRegistry() = default;
		~NodeRegistry();

		void PopulateTypes();

		template <class T>
		void RegisterType()
		{
			myNodeTypes[myTypeCounter] = new T;
			myNodeTypes[myTypeCounter]->myID = myTypeCounter;
			myTypeCounter++;
		}

		BaseNode* GetNodeFromID(const int anID)
		{
			return myNodeTypes[anID];
		}

		template <class T>
		int GetIDFromNodeType()
		{
			for (auto& type : myNodeTypes)
			{
				if (!type)
				{
					continue;
				}

				if (typeid(*type) == typeid(T))
				{
					return type->myID;
				}
			}
			return -1;
		}

	private:
		std::array<BaseNode*, 128> myNodeTypes;
		unsigned short myTypeCounter = 0;

	};
}

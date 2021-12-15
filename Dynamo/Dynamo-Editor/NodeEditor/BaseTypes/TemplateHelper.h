#pragma once
#include "NodeEditor/BaseTypes/NodeTypes.h"
#include "NodeEditor/BaseTypes/GraphNodeInstance.h"
#include "NodeEditor/Nodes/GraphNodeBase.h"
#include "NodeEditor/BaseTypes/GraphNodePin.h"
#include "NodeEditor/Manager/GraphNodeRegistry.h"

//class GraphNodeBase;
//class GraphNodeInstance;
//class GraphNodePin;

namespace TemplateHelper
{
	template<typename T>
	static T GetPinData(GraphNodeInstance* aNode, unsigned int aPinIndex)
	{
		DataPtr ptr;
		aNode->FetchData(aPinIndex, ptr);
		return *((T*)ptr.Ptr);
	}

	/**
	 * Overload to handle strings since ImGui doesn't :P
	 */
	template<typename T>
	static std::string GetPinData(GraphNodeBase& aGraphNodeBase, GraphNodeInstance* aNode, unsigned int aPinIndex)
	{
		DataType outType;
		DataPtr outPtr;
		size_t outSize;

		aGraphNodeBase.GetPinData(aNode, aPinIndex, outType, outPtr, outSize);

		std::string str = std::string((char*)outPtr.Ptr);
		return str;
	}


	/**
	 * Sets the provided data on the specified pin.
	 * Creates the data object on the pin if it's not already present.
	 * @param aNodeInstance The Node Instance to set the data on
	 * @param aPinIndex The index of the pin on the Node Instance to set the data.
	 * @param someData The data itself.
	 */
	template<typename T>
	static void SetPinData(GraphNodeInstance* aNodeInstance, unsigned int aPinIndex, T someData)
	{
		GraphNodePin& pin = aNodeInstance->myPins[aPinIndex];
		if (!pin.Data)
		{
			pin.Data = DataPtr::Create(pin.DataType);
		}
		memcpy(pin.Data.Ptr, &someData, sizeof(T));
	}

	/**
	 * Overload to handle strings since ImGui doesn't without an addon :P
	 */
	template<typename T>
	static void SetPinData(GraphNodeBase& aGraphNodeBase, GraphNodeInstance* aNodeInstance, unsigned int aPinIndex, std::string someData)
	{
		char* terminatedData = DBG_NEW char[someData.length() + 1]();
		memcpy(terminatedData, someData.c_str(), someData.length());

		aGraphNodeBase.SetPinDataRaw(aNodeInstance, aPinIndex, terminatedData, someData.length() + 1);
	}

	template<typename T>
	static void SetVariable(GraphNodeInstance& aGraphNodeInstance, T someData)
	{
		aGraphNodeInstance->myVariable->Set(someData);
	}

	template <typename T>
	static void RegisterType()
	{
		T* t = new T();

		GraphNodeRegistry::myTypes[GraphNodeRegistry::myTypeCounter] = reinterpret_cast<GraphNodeBase*>(t);
		GraphNodeRegistry::myTypes[GraphNodeRegistry::myTypeCounter]->myID = GraphNodeRegistry::myTypeCounter;
		GraphNodeRegistry::myTypeCounter++;
	}
};

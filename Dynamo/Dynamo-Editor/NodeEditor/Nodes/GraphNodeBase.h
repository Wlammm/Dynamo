#pragma once
#include "NodeEditor/BaseTypes/NodeTypes.h"
#include "NodeEditor/Manager/UID.h"
#include "NodeEditor/BaseTypes/GraphNodePin.h"

/**
 * Base class for all graph nodes regardless of type.
 */

namespace Editor
{
	class GraphNodeRegistry;
	class GraphNodeInstance;

	class GraphNodeBase
	{
		friend GraphNodeRegistry;
		friend GraphNodeInstance;

		std::vector<GraphNodePin> myPins;

	public:
		int myID = -1;

		virtual ~GraphNodeBase();

		FORCEINLINE virtual std::string GetNodeName() const { return "N/A"; }
		FORCEINLINE virtual std::string GetInstanceName(const GraphNodeInstance* anInstance) const { anInstance;  return GetNodeName(); }
		FORCEINLINE virtual bool IsStartNode() const { return false; }
		FORCEINLINE virtual std::string GetNodeTypeCategory() const { return ""; }

		std::vector<GraphNodePin> GetPins() { return myPins; }

		/**
		 * Node Entry point. This is when we get activated.
		 * @param prevNode The Node that called this function.
		 * @returns Pin ID to continue execution flow on, or -1 if we have no such pin to use.
		 */
		int Exec(GraphNodeInstance* prevNode);

		int ExecWithPayload(GraphNodeInstance* prevNode, Payload& aPayload);

		bool IsFlowNode();

		// Draw debug? Draw text? Use it for something handy.
		virtual void DebugUpdate(GraphNodeInstance*) {}

		__forceinline int GetId() const { return myID; }
		__forceinline bool IsHiddenFromMenu() const { return myHiddenFromMenu; }

		void GetPinData(GraphNodeInstance* aNode, unsigned int aPinIndex, DataType& outType, DataPtr& someData, size_t& outSize) const;

		/**
		 * Allows setting of raw data on nodes, useful for i.e. std::string can so on (called by specific SetPinData overload)
		 * @param aNodeInstance The Node Instance to set the data on
		 * @param aPinIndex The index of the pin on the Node Instance to set the data.
		 * @param someRawData The data itself.
		 * @param someDataSize The length of the data ptr.
		 */
		void SetPinDataRaw(GraphNodeInstance* aNodeInstance, unsigned int aPinIndex, const char* someRawData, size_t someDataSize);

	protected:
		bool myHiddenFromMenu = false;

		/**
		 * Event triggered when the node is entered. This is what should be overriden to do Node work.
		 */
		virtual int OnExec(GraphNodeInstance* aNodeInstance);

		/**
		 * Specific event used by Start nodes that can have input data.
		 */
		virtual int OnExecWithPayload(GraphNodeInstance* aNodeInstance, Payload& aPayload);

		/**
		 * Creates a new pin of the specified type.
		 * @param aLabel The label of this pin, if applicable.
		 * @param aDirection If this pin is an In or Out pin.
		 * @param aPinType Type of this pin, also controls its data type.
		 * @param isReadOnly If True this pin can only be used to construct a type, not be connected to anything.
		 * @param allowConstructInPlace If True this pin will show input controls if it's not connected.
		 */
		void CreatePin(std::string aLabel, PinDirection aDirection, DataType aPinType = DataType::Exec,
			bool isReadOnly = false, bool allowConstructInPlace = true);

	};
}
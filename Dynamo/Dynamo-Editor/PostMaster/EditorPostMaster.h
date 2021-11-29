#pragma once

#include "EditorPostObserver.h"
#include "EditorCommandTypes.hpp"

namespace Editor
{
	struct PostCommand
	{
		PostCommand() = delete;
		PostCommand(CommandType aType, void* aPointer)
		{
			myType = aType;
			myPtr = aPointer;
		}

		void* myPtr;
		CommandType myType;
	};

	class PostMaster
	{
	public:
		PostMaster() = default;
		~PostMaster() = default;

		void AddObserver(PostObserver* anObserver, CommandType aCommand);
		void RemoveObserver(PostObserver* anObserver, CommandType aCommand);
		void PostCommand(const PostCommand& aCommand);

	private:
		std::map<CommandType, CU::DArray<PostObserver*>> myObservers;
	};
}
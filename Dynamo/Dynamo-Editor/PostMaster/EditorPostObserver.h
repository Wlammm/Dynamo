#pragma once

namespace Editor
{
	struct PostCommand;
	class PostObserver
	{
	public:
		virtual void OnCommand(const PostCommand& aCommand) = 0;
	};
}
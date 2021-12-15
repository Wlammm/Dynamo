#pragma once

namespace Dynamo
{
	struct Animation;

	class AnimationFactory
	{
	public:
		static Animation* GetAnimation(const std::string& aPath);

	private:
		static void LoadAnimation(const std::string& aPath);

	private:
		inline static std::unordered_map<std::string, Animation*> myAnimations;
	};
}
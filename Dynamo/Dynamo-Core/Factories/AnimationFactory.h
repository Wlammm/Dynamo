#pragma once

namespace fbxsdk
{
	class FbxNode;
}

namespace Dynamo
{
	struct Animation;

	class AnimationFactory
	{
	public:
		static Animation* GetAnimation(const std::string& aPath);

	private:
		static void LoadAnimation(const std::string& aPath);

		static void GatherNodes(fbxsdk::FbxNode* aNode, std::vector<fbxsdk::FbxNode*>& outVector);

	private:
		inline static std::unordered_map<std::string, Animation*> myAnimations;
	};
}
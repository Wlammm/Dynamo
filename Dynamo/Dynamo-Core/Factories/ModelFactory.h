#pragma once

namespace fbxsdk
{
	class FbxNode;
}

namespace Dynamo
{
	class Model;

	class ModelFactory
	{
	public:
		static Model* GetModel(const std::string& aPath);

	private:
		static void LoadModel(const std::string& aPath);
		static void GatherMeshNodes(fbxsdk::FbxNode* aNode, std::vector<fbxsdk::FbxNode*>& outMeshes);

	private:
		static std::unordered_map<std::string, std::unique_ptr<Model>> myModels;
	};
}
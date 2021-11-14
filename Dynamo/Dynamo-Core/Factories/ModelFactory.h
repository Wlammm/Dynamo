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
		static void GatherMeshNodes(FbxNode* aNode, std::vector<FbxNode*>& outMeshes);

	private:
		static std::unordered_map<std::string, std::unique_ptr<Model>> myModels;
	};
}
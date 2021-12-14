#pragma once

namespace fbxsdk
{
	class FbxNode;
	class FbxAMatrix;
}


namespace Dynamo
{
	struct Skeleton
	{
		struct Joint
		{
			uint myParentIndex;
			std::string myName;

			Mat4f myBindPoseInverse;
		};

		std::vector<Joint> myJoints;
		std::unordered_map<std::string, uint> myJointNameToIndex;
	};

	class Model;

	class ModelFactory
	{
	public:
		static Model* GetModel(const std::string& aPath);

	private:
		static void LoadModel(const std::string& aPath);
		static void GatherMeshNodes(fbxsdk::FbxNode* aNode, std::vector<fbxsdk::FbxNode*>& outMeshes);
		static bool GatherSkeletonData(fbxsdk::FbxNode* aNode, Skeleton& outSkeleton, uint anIndex, uint someParentIndex);
		static Mat4f GetMatrix(fbxsdk::FbxAMatrix& aMat);

	private:
		static std::unordered_map<std::string, std::unique_ptr<Model>> myModels;
	};
}
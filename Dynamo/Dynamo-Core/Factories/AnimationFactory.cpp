#include "pch.h"
#include "AnimationFactory.h"
#include "Rendering/Animation.h"
#include "fbxsdk.h"
#include <filesystem>

namespace Dynamo
{
    Animation* AnimationFactory::GetAnimation(const std::string& aPath)
    {
        if (!std::filesystem::exists(aPath))
            return nullptr;


        if (myAnimations.find(aPath) != myAnimations.end())
        {
            return myAnimations[aPath];
        }

        LoadAnimation(aPath);
        return myAnimations[aPath];
    }

    void AnimationFactory::LoadAnimation(const std::string& aPath)
    {
        Animation* anim = new Animation();
        FbxManager* manager = FbxManager::Create();

        FbxImporter* importer = FbxImporter::Create(manager, "AnimScene");
        bool result = importer->Initialize(aPath.c_str(), -1, manager->GetIOSettings());
        assert(result);
        result = importer->IsFBX();
        assert(result);

        FbxScene* scene = FbxScene::Create(manager, "AnimScene");
        FbxAxisSystem axisSystem(FbxAxisSystem::eDirectX);

        if (!importer->Import(scene))
            assert(false && "Failed to import animation");

        axisSystem.DeepConvertScene(scene);

        bool hasBones = false;
        Skeleton skeleton;

        std::vector<FbxNode*> meshNodes;
        ModelFactory::GatherMeshNodes(scene->GetRootNode(), meshNodes);

        hasBones = ModelFactory::GatherSkeletonData(scene->GetRootNode(), skeleton, 0, 0);

        for (FbxNode* node : meshNodes)
        {
            FbxMesh* mesh = node->GetMesh();

            const FbxVector4 translation = node->GetGeometricTranslation(FbxNode::eSourcePivot);
            const FbxVector4 rotation = node->GetGeometricRotation(FbxNode::eSourcePivot);
            const FbxVector4 scale = node->GetGeometricScaling(FbxNode::eSourcePivot);
            const FbxAMatrix rootTransform = FbxAMatrix(translation, rotation, scale);

            for (int deformIdx = 0; deformIdx < mesh->GetDeformerCount(); ++deformIdx)
            {
                FbxSkin* skin = (FbxSkin*)mesh->GetDeformer(deformIdx, FbxDeformer::eSkin);

                if (!skin)
                    continue;

                for (int clusterIdx = 0; clusterIdx < skin->GetClusterCount(); ++clusterIdx)
                {
                    FbxCluster* cluster = skin->GetCluster(clusterIdx);

                    uint jointIndex = skeleton.myJointNameToIndex[cluster->GetLink()->GetName()];

                    FbxAnimStack* animStack = scene->GetSrcObject<FbxAnimStack>(0);
                    FbxString animStackName = animStack->GetName();
                    std::string animationName = animStackName.Buffer();

                    FbxTakeInfo* takeInfo = scene->GetTakeInfo(animStackName);
                    if (takeInfo)
                    {
                        FbxTime start = takeInfo->mLocalTimeSpan.GetStart();
                        FbxTime end = takeInfo->mLocalTimeSpan.GetStop();

                        FbxLongLong animTime = end.GetFrameCount(FbxTime::eFrames24) - start.GetFrameCount(FbxTime::eFrames24) + 1;

                        anim->myFrameCount = (uint)animTime;
                        anim->myBoneTransforms[jointIndex].resize(animTime + 1);

                        for (FbxLongLong t = start.GetFrameCount(FbxTime::eFrames24); t <= end.GetFrameCount(FbxTime::eFrames24); ++t)
                        {
                            FbxTime time;
                            time.SetFrame(t, FbxTime::eFrames24);

                            FbxAMatrix nodeTransform = node->EvaluateGlobalTransform(time) * rootTransform;
                            FbxAMatrix frameTransform = nodeTransform.Inverse() * cluster->GetLink()->EvaluateGlobalTransform(time);

                            frameTransform = frameTransform.Transpose();

                            anim->myBoneTransforms[jointIndex][t] = ModelFactory::GetMatrix(frameTransform);
                        }
                    }
                }
            }
        }

        anim->myPath = aPath;
        myAnimations[aPath] = anim;
    }
}
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
        {
            Console::ErrorLog("Missing animation: %s", aPath.c_str());
            return nullptr;
        }

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
        DYN_ASSERT(result);
        result = importer->IsFBX();
        DYN_ASSERT(result);

        FbxScene* scene = FbxScene::Create(manager, "AnimScene");
        FbxAxisSystem axisSystem(FbxAxisSystem::eDirectX);

        if (!importer->Import(scene))
            DYN_ASSERT(false && "Failed to import animation");

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

                    FbxAMatrix meshBindTransform;
                    cluster->GetTransformMatrix(meshBindTransform);

                    FbxAMatrix linkTransform;
                    cluster->GetTransformLinkMatrix(linkTransform);

                    FbxAMatrix bindPoseInverse = linkTransform.Inverse() * meshBindTransform * rootTransform;
                    bindPoseInverse = bindPoseInverse.Transpose();

                    uint jointIndex = skeleton.myJointNameToIndex[cluster->GetLink()->GetName()];

                    FbxAnimStack* animStack = scene->GetSrcObject<FbxAnimStack>(0);
                    FbxString animStackName = animStack->GetName();
                    std::string animationName = animStackName.Buffer();

                    FbxTakeInfo* takeInfo = scene->GetTakeInfo(animStackName);
                    if (takeInfo)
                    {
                        FbxTime start = takeInfo->mLocalTimeSpan.GetStart();
                        FbxTime end = takeInfo->mLocalTimeSpan.GetStop();

                        FbxLongLong animTime = end.GetFrameCount(FbxTime::eFrames30) - start.GetFrameCount(FbxTime::eFrames30) + 1;

                        anim->myFrameCount = (uint)animTime;
                        anim->myBoneTransforms[jointIndex].resize(animTime + 1);

                        for (FbxLongLong t = start.GetFrameCount(FbxTime::eFrames30); t <= end.GetFrameCount(FbxTime::eFrames30); ++t)
                        {
                            FbxTime time;
                            time.SetFrame(t, FbxTime::eFrames30);

                            FbxAMatrix nodeTransform = node->EvaluateGlobalTransform(time) * rootTransform;
                            FbxAMatrix frameTransform = nodeTransform.Inverse() * cluster->GetLink()->EvaluateGlobalTransform(time);

                            frameTransform = frameTransform.Transpose();

                            frameTransform = frameTransform;

                            Mat4f engFrameTransform = ModelFactory::GetMatrix(frameTransform);
                            Mat4f engInverseBind = ModelFactory::GetMatrix(bindPoseInverse);

                            anim->myBoneTransforms[jointIndex][t] = engFrameTransform * engInverseBind;
                        }
                    }
                }
            }
        }

        anim->myPath = aPath;
        myAnimations[aPath] = anim;
    }

    void AnimationFactory::GatherNodes(FbxNode* aNode, std::vector<FbxNode*>& outVector)
    {
        for (int i = 0; i < aNode->GetChildCount(); ++i)
        {
            FbxNode* childNode = aNode->GetChild(i);
            FbxNodeAttribute* attribute = childNode->GetNodeAttribute();

            if (!attribute)
                GatherNodes(childNode, outVector);
            else
            {
                FbxNodeAttribute::EType childType = attribute->GetAttributeType();
                if (childType != FbxNodeAttribute::eSkeleton)
                {
                    GatherNodes(childNode, outVector);
                }
                else
                {
                    outVector.push_back(childNode);
                    GatherNodes(childNode, outVector);
                }
            }

        }
    }
}
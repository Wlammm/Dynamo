#include "pch.h"
#include "ModelFactory.h"
#include "Rendering/Model.h"
#include "fbxsdk.h"

/*
* Autodesk FBX SDK Guide.
* http://docs.autodesk.com/FBX/2014/ENU/FBX-SDK-Documentation/
*/

namespace Dynamo
{
    Model* ModelFactory::GetModel(const std::string& aPath)
    {
        if (myModels.find(aPath) != myModels.end())
        {
            return myModels[aPath].get();
        }

        LoadModel(aPath);
        return myModels[aPath].get();
    }

    void ModelFactory::LoadModel(const std::string& aPath)
    {
        struct Vertex
        {
            Vec4f myPosition;
            Color myVertexColor1;
            Color myVertexColor2;
            Color myVertexColor3;
            Color myVertexColor4;
            Vec2f myVertexUV1;
            Vec2f myVertexUV2;
            Vec2f myVertexUV3;
            Vec2f myVertexUV4;
            Vec4f myNormals;
            Vec4f myTangents;
            Vec4f myBinormals;

            bool operator==(const Vertex& lhs)
            {
                return memcmp(this, &lhs, sizeof(Vertex));
            }
        };

        std::vector<Vertex> vertices;
        std::vector<uint> indicies;

        FbxManager* manager = FbxManager::Create();
        FbxImporter* importer = FbxImporter::Create(manager, "");

        FbxScene* scene = FbxScene::Create(manager, "Scene");

        FbxAxisSystem axisSystem(FbxAxisSystem::eDirectX);
        axisSystem.ConvertScene(scene);

        if (importer->Import(scene))
        {
            FbxGeometryConverter converter(manager);
            converter.Triangulate(scene, true);

            std::vector<FbxNode*> meshNodes;
            GatherMeshNodes(scene->GetRootNode(), meshNodes);

            for (FbxNode* node : meshNodes)
            {
                FbxMesh* mesh = node->GetMesh();

                if (mesh->GetElementBinormalCount() == 0 || mesh->GetElementTangentCount() == 0)
                {
                    bool result = mesh->GenerateTangentsData(0, true, false);
                    assert(result);
                }

                const uint polyCount = mesh->GetPolygonCount();
                for (int p = 0; p < polyCount; ++p)
                {
                    for (int v = 0; v < 3; ++v)
                    {
                        const int controlPointIndex = mesh->GetPolygonVertex(p, v);
                        const FbxVector4 vertexPos = mesh->GetControlPointAt(controlPointIndex);

                        FbxVector2 vertexUVs[4];
                        const int numUVs = mesh->GetElementUVCount();
                        const int textureUVIndex = mesh->GetTextureUVIndex(p, v);
                        for (int uv = 0; uv < numUVs && uv < 4; ++uv)
                        {
                            FbxGeometryElementUV* uvElement = mesh->GetElementUV(uv);
                            vertexUVs[uv] = uvElement->GetDirectArray().GetAt(textureUVIndex);
                        }

                        const int polygonIndex = p * 3 + v;
                        FbxVector4 normal;
                        FbxGeometryElementNormal* normalElement = mesh->GetElementNormal(0);
                        auto mapNode = normalElement->GetMappingMode();
                        assert(mapNode == 2);
                        auto refMode = normalElement->GetReferenceMode();
                        assert(refMode == 0);
                        normal = normalElement->GetDirectArray().GetAt(polygonIndex);

                        FbxVector4 tangent;
                        FbxGeometryElementTangent* tangentElement = mesh->GetElementTangent(0);
                        tangent = tangentElement->GetDirectArray().GetAt(polygonIndex);

                        FbxVector4 binormal;
                        FbxGeometryElementBinormal* binormalElement = mesh->GetElementBinormal(0);
                        binormal = binormalElement->GetDirectArray().GetAt(polygonIndex);

                        FbxColor fbxColors[4];
                        const int numVertexColors = mesh->GetElementVertexColorCount();
                        for (int col = 0; col < numVertexColors && col < 4; ++col)
                        {
                            FbxGeometryElementVertexColor* colorElement = mesh->GetElementVertexColor(col);
                            fbxColors[col] = colorElement->GetDirectArray().GetAt(polygonIndex);
                        }

                        Vertex vertex;
                        vertex.myPosition = Vec4d{ vertexPos[0], vertexPos[1], vertexPos[2], 0 }.Cast<float>();
                        vertex.myVertexColor1 = Vec4d{ fbxColors[0][0], fbxColors[0][1], fbxColors[0][2], fbxColors[0][3] }.Cast<float>();
                        vertex.myVertexColor2 = Vec4d{ fbxColors[1][0], fbxColors[1][1], fbxColors[1][2], fbxColors[1][3] }.Cast<float>();
                        vertex.myVertexColor3 = Vec4d{ fbxColors[2][0], fbxColors[2][1], fbxColors[2][2], fbxColors[2][3] }.Cast<float>();
                        vertex.myVertexColor4 = Vec4d{ fbxColors[3][0], fbxColors[3][1], fbxColors[3][2], fbxColors[3][3] }.Cast<float>();
                        vertex.myVertexUV1 = Vec2i{ vertexUVs[0][0], vertexUVs[0][1] }.Cast<float>();
                        vertex.myVertexUV2 = Vec2i{ vertexUVs[1][0], vertexUVs[1][1] }.Cast<float>();
                        vertex.myVertexUV3 = Vec2i{ vertexUVs[2][0], vertexUVs[2][1] }.Cast<float>();
                        vertex.myVertexUV4 = Vec2i{ vertexUVs[3][0], vertexUVs[3][1] }.Cast<float>();
                        vertex.myNormals = Vec4d{ normal[0], normal[1], normal[2], normal[3] }.Cast<float>();
                        vertex.myTangents = Vec4d{ tangent[0], tangent[1], tangent[2], tangent[3] }.Cast<float>();
                        vertex.myBinormals = Vec4d{ binormal[0], binormal[1], binormal[2], binormal[3] }.Cast<float>();

                        bool alreadyExists = false;
                        for (int i = 0; i < vertices.size(); ++i)
                        {
                            if (vertices[i] == vertex)
                            {
                                indicies.push_back((uint)i);
                                alreadyExists = true;
                                break;
                            }
                        }

                        if (!alreadyExists)
                        {
                            vertices.push_back(vertex);
                            indicies.push_back((uint)vertices.size() - 1);
                        }
                    }
                }
            }
        }
    }

    void ModelFactory::GatherMeshNodes(FbxNode* aNode, std::vector<FbxNode*>& outMeshes)
    {
        for (int i = 0; i < aNode->GetChildCount(); ++i)
        {
            FbxNode* childNode = aNode->GetChild(i);
            FbxNodeAttribute* childAttribute = childNode->GetNodeAttribute();

            if (!childAttribute)
            {
                GatherMeshNodes(childNode, outMeshes);
            }
            else
            {
                FbxNodeAttribute::EType childType = childAttribute->GetAttributeType();
                if (childType != FbxNodeAttribute::eMesh)
                {
                    GatherMeshNodes(childNode, outMeshes);
                }
                else
                {
                    outMeshes.push_back(childNode);
                    GatherMeshNodes(childNode, outMeshes);
                }
            }
        }
    }
}

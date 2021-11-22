#include "pch.h"
#include "ModelFactory.h"
#include "Rendering/Model.h"
#include "fbxsdk.h"
#include <fstream>
#include "Rendering/Material.h"

/*
* Autodesk FBX SDK Guide.
* http://docs.autodesk.com/FBX/2014/ENU/FBX-SDK-Documentation/
*/

namespace Dynamo
{
    std::unordered_map<std::string, std::unique_ptr<Model>> ModelFactory::myModels;

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
        std::unique_ptr<Model> model = std::make_unique<Model>();

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
                return !memcmp(this, &lhs, sizeof(Vertex));
            }
        };

        FbxManager* manager = FbxManager::Create();
        FbxImporter* importer = FbxImporter::Create(manager, "Scene");
        bool result = importer->Initialize(aPath.c_str(), -1, manager->GetIOSettings());
        assert(result);
        result = importer->IsFBX();
        assert(result);

        FbxScene* scene = FbxScene::Create(manager, "Scene");

        FbxAxisSystem axisSystem(FbxAxisSystem::eDirectX);
        axisSystem.DeepConvertScene(scene);

        if (importer->Import(scene))
        {
            FbxGeometryConverter converter(manager);
            converter.Triangulate(scene, true);

            std::vector<FbxNode*> meshNodes;
            GatherMeshNodes(scene->GetRootNode(), meshNodes);

            for (FbxNode* node : meshNodes)
            {
                std::vector<Vertex> vertices;
                std::vector<uint> indicies;

                FbxMesh* fbxMesh = node->GetMesh();

                if (fbxMesh->GetElementBinormalCount() == 0 || fbxMesh->GetElementTangentCount() == 0)
                {
                    bool result = fbxMesh->GenerateTangentsData(0, true, false);
                    assert(result);
                }

                const uint polyCount = fbxMesh->GetPolygonCount();
                for (uint p = 0; p < polyCount; ++p)
                {
                    for (int v = 0; v < 3; ++v)
                    {
                        const int controlPointIndex = fbxMesh->GetPolygonVertex(p, v);
                        const fbxsdk::FbxVector4 vertexPos = fbxMesh->GetControlPointAt(controlPointIndex);

                        FbxVector2 vertexUVs[4];
                        const int numUVs = fbxMesh->GetElementUVCount();
                        const int textureUVIndex = fbxMesh->GetTextureUVIndex(p, v);
                        for (int uv = 0; uv < numUVs && uv < 4; ++uv)
                        {
                            FbxGeometryElementUV* uvElement = fbxMesh->GetElementUV(uv);
                            vertexUVs[uv] = uvElement->GetDirectArray().GetAt(textureUVIndex);
                        }

                        const int polygonIndex = p * 3 + v;
                        fbxsdk::FbxVector4 normal;
                        FbxGeometryElementNormal* normalElement = fbxMesh->GetElementNormal(0);
                        auto mapNode = normalElement->GetMappingMode();
                        assert(mapNode == 2);
                        auto refMode = normalElement->GetReferenceMode();
                        assert(refMode == 0);
                        normal = normalElement->GetDirectArray().GetAt(polygonIndex);

                        fbxsdk::FbxVector4 tangent;
                        FbxGeometryElementTangent* tangentElement = fbxMesh->GetElementTangent(0);
                        tangent = tangentElement->GetDirectArray().GetAt(polygonIndex);

                        fbxsdk::FbxVector4 binormal;
                        FbxGeometryElementBinormal* binormalElement = fbxMesh->GetElementBinormal(0);
                        binormal = binormalElement->GetDirectArray().GetAt(polygonIndex);

                        FbxColor fbxColors[4];
                        const int numVertexColors = fbxMesh->GetElementVertexColorCount();
                        for (int col = 0; col < numVertexColors && col < 4; ++col)
                        {
                            FbxGeometryElementVertexColor* colorElement = fbxMesh->GetElementVertexColor(col);
                            fbxColors[col] = colorElement->GetDirectArray().GetAt(polygonIndex);
                        }

                        Vertex vertex;
                        vertex.myPosition = Vec4d{ vertexPos[0], vertexPos[1], vertexPos[2], 1 }.Cast<float>();
                        vertex.myVertexColor1 = Vec4d{ fbxColors[0][0], fbxColors[0][1], fbxColors[0][2], fbxColors[0][3] }.Cast<float>();
                        vertex.myVertexColor2 = Vec4d{ fbxColors[1][0], fbxColors[1][1], fbxColors[1][2], fbxColors[1][3] }.Cast<float>();
                        vertex.myVertexColor3 = Vec4d{ fbxColors[2][0], fbxColors[2][1], fbxColors[2][2], fbxColors[2][3] }.Cast<float>();
                        vertex.myVertexColor4 = Vec4d{ fbxColors[3][0], fbxColors[3][1], fbxColors[3][2], fbxColors[3][3] }.Cast<float>();
                        vertex.myVertexUV1 = Vec2d{ vertexUVs[0][0], 1 - vertexUVs[0][1] }.Cast<float>();
                        vertex.myVertexUV2 = Vec2d{ vertexUVs[1][0], 1 - vertexUVs[1][1] }.Cast<float>();
                        vertex.myVertexUV3 = Vec2d{ vertexUVs[2][0], 1 - vertexUVs[2][1] }.Cast<float>();
                        vertex.myVertexUV4 = Vec2d{ vertexUVs[3][0], 1 - vertexUVs[3][1] }.Cast<float>();
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

                HRESULT result;
                
                D3D11_BUFFER_DESC vertexBufferDesc = { 0 };
                vertexBufferDesc.ByteWidth = (uint)vertices.size() * sizeof(vertices[0]);
                vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
                vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

                D3D11_SUBRESOURCE_DATA vertexSubresource = { 0 };
                vertexSubresource.pSysMem = vertices.data();

                ID3D11Buffer* vertexBuffer;
                result = Main::GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexSubresource, &vertexBuffer);
                assert(SUCCEEDED(result));

                D3D11_BUFFER_DESC indexBufferDesc = { 0 };
                indexBufferDesc.ByteWidth = (uint)indicies.size() * sizeof(indicies[0]);
                indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
                indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

                D3D11_SUBRESOURCE_DATA indexSubresource = { 0 };
                indexSubresource.pSysMem = indicies.data();

                ID3D11Buffer* indexBuffer;
                result = Main::GetDevice()->CreateBuffer(&indexBufferDesc, &indexSubresource, &indexBuffer);
                assert(SUCCEEDED(result));

                D3D11_INPUT_ELEMENT_DESC layout[] =
                {
                    { "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
                    { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
                    { "COLOR", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
                    { "COLOR", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
                    { "COLOR", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
                    { "UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
                    { "UV", 1, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
                    { "UV", 2, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
                    { "UV", 3, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
                    { "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
                    { "TANGENT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
                    { "BINORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
                };

                ID3D11InputLayout* inputLayout;
                const std::string defaultVSData = MaterialFactory::GetDefaultMaterial()->myVertexShader->GetData();
                result = Main::GetDevice()->CreateInputLayout(layout, sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC), defaultVSData.data(), defaultVSData.size(), &inputLayout);
                assert(SUCCEEDED(result));

                Mesh mesh;
                mesh.myVertexBuffer = vertexBuffer;
                mesh.myIndexBuffer = indexBuffer;
                mesh.myNumVertices = (uint)vertices.size();
                mesh.myNumIndicies = (uint)indicies.size();
                mesh.myInputLayout = inputLayout;
                mesh.myPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
                mesh.myOffset = 0;
                mesh.myStride = sizeof(Vertex);

                model->AddMesh(mesh);
            }
        }

        model->myPath = aPath;
        myModels[aPath] = std::move(model);
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

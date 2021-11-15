#include "pch.h"
#include "MaterialFactory.h"
#include "Rendering/Material.h"

namespace Dynamo
{
    Material* Dynamo::MaterialFactory::GetMaterial(const std::string& aPath)
    {
        if (myMaterials.find(aPath) != myMaterials.end())
        {
            return &myMaterials[aPath];
        }

        LoadMaterial(aPath);
        return &myMaterials[aPath];
    }

    Material* MaterialFactory::GetDefaultMaterial()
    {
        return GetMaterial("Assets/Materials/DefaultMaterial.json");
    }

    void MaterialFactory::LoadMaterial(const std::string& aPath)
    {
        nlohmann::json json = JsonFactory::GetJson(aPath);

        Material mat;
        mat.myAlbedo = ResourceFactory::GetSRV(json["AlbedoTexture"].get<std::string>());
        mat.myNormal = ResourceFactory::GetSRV(json["NormalTexture"].get<std::string>());
        mat.myMaterial = ResourceFactory::GetSRV(json["MaterialTexture"].get<std::string>());
        mat.myPixelShader = ShaderFactory::GetShader(json["PixelShader"].get<std::string>(), ShaderType::PixelShader);
        mat.myVertexShader = ShaderFactory::GetShader(json["VertexShader"].get<std::string>(), ShaderType::VertexShader);

        mat.myCustomTextures[0] = ResourceFactory::GetSRV(json["CustomTexture1"].get<std::string>());
        mat.myCustomTextures[1] = ResourceFactory::GetSRV(json["CustomTexture2"].get<std::string>());
        mat.myCustomTextures[2] = ResourceFactory::GetSRV(json["CustomTexture3"].get<std::string>());
        mat.myCustomTextures[3] = ResourceFactory::GetSRV(json["CustomTexture4"].get<std::string>());

        myMaterials[aPath] = mat;
    }
}
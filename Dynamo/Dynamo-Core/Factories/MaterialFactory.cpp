#include "pch.h"
#include "MaterialFactory.h"
#include "Rendering/Material.h"
#include "Utils/FileUtils.h"
#include <filesystem>

namespace Dynamo
{
    std::unordered_map<std::string, Material> MaterialFactory::myMaterials;
    std::unordered_map<std::string, Material> MaterialFactory::myModelMaterials;

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
        return GetMaterial("Assets/Materials/DefaultMaterial.dynmaterial");
    }

    Material* MaterialFactory::GetMaterialForModel(const std::string& aModelPath)
    {
        if (myModelMaterials.find(aModelPath) != myModelMaterials.end())
        {
            return &myModelMaterials[aModelPath];
        }

        LoadModelMaterial(aModelPath);
        return &myModelMaterials[aModelPath];
    }

    void MaterialFactory::SaveMaterial(const Material* aMaterial)
    {

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

        mat.myReceiveShadows = json["ReceiveShadows"].get<bool>();
        mat.mySurfaceType = json["SurfaceType"].get<SurfaceType>();

        mat.myMaterialPath = aPath;

        myMaterials[aPath] = mat;
    }

    void MaterialFactory::LoadModelMaterial(const std::string& aPath)
    {
        std::string noExt = FileUtils::RemoveExtension(aPath);
        
        std::string albedoPath = noExt + "_C.dds";
        std::string normalPath = noExt + "_N.dds";
        std::string materialPath = noExt + "_M.dds";

        Material mat;
        Material* defaultMat = GetDefaultMaterial();

        if (!FileUtils::FileExists(albedoPath) || !FileUtils::FileExists(normalPath) || !FileUtils::FileExists(materialPath))
        {
            Console::LogOnce("Texture paths not found for mesh: %s", aPath.c_str());
            myModelMaterials[aPath] = *defaultMat;
            return;
        }

        mat.myAlbedo = ResourceFactory::GetSRV(albedoPath);
        mat.myNormal = ResourceFactory::GetSRV(normalPath);
        mat.myMaterial = ResourceFactory::GetSRV(materialPath);
        mat.myPixelShader = defaultMat->myPixelShader;
        mat.myVertexShader = defaultMat->myVertexShader;
        myModelMaterials[aPath] = mat;
    }
}
#include "pch.h"
#include "MaterialFactory.h"
#include "Rendering/Material.h"
#include "Utils/FileUtils.h"
#include <filesystem>
#include <fstream>

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

        if (!std::filesystem::exists(aPath))
        {
            return GetMissingMaterial();
        }

        LoadMaterial(aPath);
        return &myMaterials[aPath];
    }

    Material* MaterialFactory::GetDefaultMaterial()
    {
        return GetMaterial("Assets/Materials/DefaultMaterials/DefaultMaterial.dynmaterial");
    }

    Material* MaterialFactory::GetMissingMaterial()
    {
        return GetMaterial("Assets/Materials/DefaultMaterials/MissingMaterial.dynmaterial");
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
        nlohmann::json json;
        json["AlbedoTexture"] = aMaterial->myAlbedo->GetPath().string();
        json["NormalTexture"] = aMaterial->myNormal->GetPath().string();
        json["MaterialTexture"] = aMaterial->myMaterial->GetPath().string();
        json["PixelShader"] = aMaterial->myPixelShader->GetPath().string();
        json["VertexShader"] = aMaterial->myVertexShader->GetPath().string();

        std::string customTex1 = "";
        if (aMaterial->myCustomTextures[0])
            customTex1 = aMaterial->myCustomTextures[0]->GetPath().string();
        json["CustomTexture1"] = customTex1;

        std::string customTex2 = "";
        if (aMaterial->myCustomTextures[1])
            customTex2 = aMaterial->myCustomTextures[1]->GetPath().string();
        json["CustomTexture2"] = customTex2;

        std::string customTex3 = "";
        if (aMaterial->myCustomTextures[2])
            customTex3 = aMaterial->myCustomTextures[2]->GetPath().string();
        json["CustomTexture3"] = customTex3;

        std::string customTex4 = "";
        if (aMaterial->myCustomTextures[3])
            customTex4 = aMaterial->myCustomTextures[3]->GetPath().string();
        json["CustomTexture4"] = customTex4;

        json["ReceiveShadows"] = aMaterial->myReceiveShadows;
        json["Renderer"] = aMaterial->myRenderer;

        json["CustomValue1"] = aMaterial->myCustomValues[0];
        json["CustomValue2"] = aMaterial->myCustomValues[1];
        json["CustomValue3"] = aMaterial->myCustomValues[2];
        json["CustomValue4"] = aMaterial->myCustomValues[3];

        json["MetalnessInterp"] = aMaterial->myMetalnessInterpolation;
        json["Metalness"] = aMaterial->myMetalnessConstant;
        json["RoughnessInterp"] = aMaterial->myRoughnessInterpolation;
        json["Roughness"] = aMaterial->myRoughnessConstant;

        std::ofstream stream(aMaterial->myMaterialPath);
        stream << json.dump(4);
        stream.close();
    }

    Material* MaterialFactory::CreateMaterial(const std::filesystem::path& aPath)
    {
        Material mat = *GetDefaultMaterial();
        mat.myMaterialPath = aPath;
        myMaterials[aPath.string()] = mat;
        SaveMaterial(&mat);
        return &myMaterials[aPath.string()];
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
        mat.myRenderer = json["Renderer"].get<Renderer>();

        mat.myCustomValues[0] = json["CustomValue1"];
        mat.myCustomValues[1] = json["CustomValue2"];
        mat.myCustomValues[2] = json["CustomValue3"];
        mat.myCustomValues[3] = json["CustomValue4"];

        mat.myMetalnessInterpolation = json["MetalnessInterp"];
        mat.myMetalnessConstant = json["Metalness"];
        mat.myRoughnessInterpolation = json["RoughnessInterp"];
        mat.myRoughnessConstant = json["Roughness"];

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
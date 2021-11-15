#include "pch.h"
#include "ShaderFactory.h"
#include "Rendering/Shader.h"

namespace Dynamo
{
    Shader* Dynamo::ShaderFactory::GetShader(const std::string& aPath, const ShaderType aShaderType)
    {
        if (aPath == "")
            return nullptr;

        if (myShaders.find(aPath) != myShaders.end())
        {
            return myShaders[aPath].get();
        }

        LoadShader(aPath, aShaderType);
        return myShaders[aPath].get();
    }

    void ShaderFactory::LoadShader(const std::string& aPath, const ShaderType aShaderType)
    {
        std::unique_ptr<Shader> shader = std::make_unique<Shader>(aPath, aShaderType);
        myShaders[aPath] = std::move(shader);
    }
}
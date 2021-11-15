#pragma once

#include "Rendering/Shader.h"

namespace Dynamo
{
	class ShaderFactory
	{
	public:
		static Shader* GetShader(const std::string& aPath, const ShaderType aShaderType);

	private:
		static void LoadShader(const std::string& aPath, const ShaderType aShaderType);

	private:
		static std::unordered_map<std::string, std::unique_ptr<Shader>> myShaders;
	};
}
#pragma once

namespace Dynamo
{
	class Shader;

	class ShaderFactory
	{
	public:
		static Shader* GetShader(const std::string& aPath, const uint aShaderType);

	private:
		static void LoadShader(const std::string& aPath, const uint aShaderType);

	private:
		static std::unordered_map<std::string, std::unique_ptr<Shader>> myShaders;
	};
}
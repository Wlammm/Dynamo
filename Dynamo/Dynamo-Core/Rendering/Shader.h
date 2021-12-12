#pragma once

namespace Dynamo
{
	enum class ShaderType
	{
		PixelShader, 
		VertexShader,
		GeometryShader,
		Count
	};

	class Shader
	{
	public:
		Shader(const std::string& aPath, const ShaderType aShaderType);
		~Shader();

		// Binds shader to stage.
		void Bind();

		// Unbinds shader from stage. 
		void ResetStage();

		const std::string& GetData() const;

		const std::filesystem::path& GetPath() const;

	private:
		void LoadPixelShader(const std::string& aPath);
		void LoadVertexShader(const std::string& aPath);
		void LoadGeometryShader(const std::string& aPath);

	private:
		std::string myData;
		std::filesystem::path myPath = "";
		ShaderType myShaderType;
		ID3D11PixelShader* myPixelShader = nullptr;
		ID3D11VertexShader* myVertexShader = nullptr;
		ID3D11GeometryShader* myGeometryShader = nullptr;

	};
}
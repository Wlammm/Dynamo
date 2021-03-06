#include "pch.h"
#include "Shader.h"
#include <fstream>

namespace Dynamo
{
	Shader::Shader(const std::string& aPath, const ShaderType aShaderType)
	{
		myPath = aPath;

		myShaderType = aShaderType;

		if (aShaderType == ShaderType::PixelShader)
			LoadPixelShader(aPath);

		if (aShaderType == ShaderType::VertexShader)
			LoadVertexShader(aPath);

		if (aShaderType == ShaderType::GeometryShader)
			LoadGeometryShader(aPath);
	}

	Shader::~Shader()
	{
		if (myPixelShader)
			myPixelShader->Release();

		if (myVertexShader)
			myVertexShader->Release();

		if (myGeometryShader)
			myGeometryShader->Release();
	}

	void Shader::Bind()
	{
		if (myShaderType == ShaderType::PixelShader)
		{
			Main::GetContext()->PSSetShader(myPixelShader, nullptr, 0);
			return;
		}

		if (myShaderType == ShaderType::VertexShader)
		{
			Main::GetContext()->VSSetShader(myVertexShader, nullptr, 0);
			return;
		}

		if (myShaderType == ShaderType::GeometryShader)
		{
			Main::GetContext()->GSSetShader(myGeometryShader, nullptr, 0);
			return;
		}
	}

	void Shader::ResetStage()
	{
		if (myShaderType == ShaderType::PixelShader)
		{
			Main::GetContext()->PSSetShader(nullptr, nullptr, 0);
			return;
		}

		if (myShaderType == ShaderType::VertexShader)
		{
			Main::GetContext()->VSSetShader(nullptr, nullptr, 0);
			return;
		}

		if (myShaderType == ShaderType::GeometryShader)
		{
			Main::GetContext()->GSSetShader(nullptr, nullptr, 0);
			return;
		}
	}

	const std::string& Shader::GetData() const
	{
		return myData;
	}

	const std::filesystem::path& Shader::GetPath() const
	{
		return myPath;
	}

	void Shader::LoadPixelShader(const std::string& aPath)
	{
		std::ifstream psFile(aPath, std::ios::binary);
		myData = { std::istreambuf_iterator<char>(psFile), std::istreambuf_iterator<char>() };
		psFile.close();
		
		HRESULT result = Main::GetDevice()->CreatePixelShader(myData.data(), myData.size(), nullptr, &myPixelShader);
		DYN_ASSERT(SUCCEEDED(result));
	}

	void Shader::LoadVertexShader(const std::string& aPath)
	{
		std::ifstream vsFile(aPath, std::ios::binary);
		myData = { std::istreambuf_iterator<char>(vsFile), std::istreambuf_iterator<char>() };
		vsFile.close();

		HRESULT result = Main::GetDevice()->CreateVertexShader(myData.data(), myData.size(), nullptr, &myVertexShader);
		DYN_ASSERT(SUCCEEDED(result));
	}

	void Shader::LoadGeometryShader(const std::string& aPath)
	{
		std::ifstream gsFile(aPath, std::ios::binary);
		myData = { std::istreambuf_iterator<char>(gsFile), std::istreambuf_iterator<char>() };
		gsFile.close();

		HRESULT result = Main::GetDevice()->CreateGeometryShader(myData.data(), myData.size(), nullptr, &myGeometryShader);
		DYN_ASSERT(SUCCEEDED(result));
	}
}
#pragma once

namespace Dynamo
{
	class ResourceFactory
	{
	public:
		static ID3D11ShaderResourceView* GetSRV(const std::string& aPath);

		static void ReleaseAll();

	private:
		static void LoadSRV(const std::string& aPath);

	private:
		static std::unordered_map<std::string, ID3D11ShaderResourceView*> mySRVs;
	};
}
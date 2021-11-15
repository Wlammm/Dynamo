#include "pch.h"
#include "ResourceFactory.h"
#include "External/DirectXTK/DDSTextureLoader.h"

namespace Dynamo
{
	ID3D11ShaderResourceView* Dynamo::ResourceFactory::GetSRV(const std::string& aPath)
	{
		if (aPath == "")
			return nullptr;

		if (mySRVs.find(aPath) != mySRVs.end())
		{
			return mySRVs[aPath];
		}

		LoadSRV(aPath);
		return mySRVs[aPath];
	}

	void ResourceFactory::ReleaseAll()
	{
		for (auto val : mySRVs)
		{
			val.second->Release();
		}
		mySRVs.clear();
	}

	void Dynamo::ResourceFactory::LoadSRV(const std::string& aPath)
	{
		ID3D11ShaderResourceView* srv;
		std::wstring wPath = { aPath.begin(), aPath.end() };

		DirectX::CreateDDSTextureFromFile(Main::GetDevice(), wPath.c_str(), nullptr, &srv);
		mySRVs[aPath] = srv;
	}
}
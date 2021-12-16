#include "pch.h"
#include "ResourceFactory.h"
#include "External/DirectXTK/DDSTextureLoader.h"

namespace Dynamo
{
	std::unordered_map<std::string, SRV*> ResourceFactory::mySRVs;

	SRV* ResourceFactory::GetSRV(const std::string& aPath)
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
			delete val.second;
		}
		mySRVs.clear();
	}

	void ResourceFactory::LoadSRV(const std::string& aPath)
	{
		ID3D11ShaderResourceView* resource;
		std::wstring wPath = { aPath.begin(), aPath.end() };

		HRESULT result = DirectX::CreateDDSTextureFromFile(Main::GetDevice(), wPath.c_str(), nullptr, &resource);
		DYN_ASSERT(SUCCEEDED(result));

		SRV* srv = new SRV();
		srv->mySRV = resource;
		srv->myPath = aPath;

		mySRVs[aPath] = srv;
	}
}
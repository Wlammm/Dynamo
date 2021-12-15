#pragma once

#include "Rendering/SRV.h"

namespace Dynamo
{
	class ResourceFactory
	{
	public:
		static SRV* GetSRV(const std::string& aPath);

		static void ReleaseAll();

	private:
		static void LoadSRV(const std::string& aPath);

	private:
		static std::unordered_map<std::string, SRV*> mySRVs;
	};
}
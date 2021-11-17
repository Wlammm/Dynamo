#include "pch.h"
#include "JsonFactory.h"
#include <fstream>

namespace Dynamo
{
	std::unordered_map<std::string, nlohmann::json> JsonFactory::myJsons;

	nlohmann::json& JsonFactory::GetJson(const std::string& aPath)
	{
		if (myJsons.find(aPath) != myJsons.end())
		{
			return myJsons[aPath];
		}

		LoadJson(aPath);
		return myJsons[aPath];
	}

	void JsonFactory::LoadJson(const std::string& aPath)
	{
		nlohmann::json json;
		std::ifstream file;
		file.open(aPath);
		file >> json;
		file.close();
		myJsons[aPath] = json;
	}
}
#pragma once

namespace Dynamo
{
	class JsonFactory
	{
	public:
		static nlohmann::json& GetJson(const std::string& aPath);

	private:
		static void LoadJson(const std::string& aPath);

	private:
		static std::unordered_map<std::string, nlohmann::json> myJsons;

	};
}
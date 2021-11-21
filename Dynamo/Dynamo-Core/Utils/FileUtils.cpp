#include "pch.h"
#include "FileUtils.h"
#include <filesystem>

namespace Dynamo
{
	std::string FileUtils::RemoveExtension(const std::string& aPath)
	{
		size_t index = aPath.find_last_of('.');
		std::string newPath = aPath.substr(0, index);
		return newPath;
	}

	bool FileUtils::FileExists(const std::string& aPath)
	{
		return std::filesystem::exists(aPath);
	}
}

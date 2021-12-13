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

	std::filesystem::path FileUtils::GetFreePath(const std::filesystem::path& aPath)
	{
		std::filesystem::path possibleName{ aPath };
		auto stem = possibleName.stem().string();
		auto ext = possibleName.extension().string();
		for (int i = 1; std::filesystem::exists(possibleName); ++i)
		{
			std::ostringstream fn;
			fn << stem << "(" << i << ")" << ext;
			possibleName.replace_filename(fn.str());
		}
		return possibleName;
	}
}

#pragma once

namespace Dynamo
{
	class FileUtils
	{
	public:
		static std::string RemoveExtension(const std::string& aPath);
		static bool FileExists(const std::string& aPath);
		static std::filesystem::path GetFreePath(const std::filesystem::path& aPath);
	};
}
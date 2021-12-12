#pragma once

#include <filesystem>

namespace Editor
{
	enum eFileType
	{
		FILETYPE_FOLDER,
		FILETYPE_JSON,
		FILETYPE_DDS,
		FILETYPE_FBX,
		FILETYPE_PNG,
		FILETYPE_CPP,
		FILETYPE_H,
		FILETYPE_HLSL,
		FILETYPE_WAV,
		FILETYPE_UNKNOWN,
		FILETYPE_COUNT
	};

	struct ContentBrowserItem
	{
		ContentBrowserItem(const std::filesystem::path& aPath, Dyn::SRV* aSRV)
		{
			mySRV = aSRV;
			myPath = aPath;
		}

		std::filesystem::path myPath;
		Dyn::SRV* mySRV;
		bool myIsSelected = false;
	};

	struct ContentBrowserDirectory
	{
		ContentBrowserDirectory() = default;
		ContentBrowserDirectory(const std::filesystem::path& aPath)
		{
			myPath = aPath;
		}

		std::filesystem::path myPath = "";
		CU::DArray<ContentBrowserDirectory> myChildren;
	};

	class ContentBrowser : public EditorWindow
	{
	public:
		ContentBrowser();
		
		void Update() override;

	private:
		void LoadDirectory(const std::filesystem::path& aPath);
		void LoadAllDirectories(const std::filesystem::path& aPath);
		void DrawDirectoriesView(const std::filesystem::path& aPath, const int aID);
		void UpdateRightClickMenu();
		void BeginRenaming();
		void UpdateRenaming();
		void UpdateShortcuts();
		void HandleDoubleClick(const ContentBrowserItem& aClickedItem);

		std::string GetPayloadType(const std::filesystem::path& aPath);

		Dyn::SRV* GetSRVFromPath(const std::filesystem::path& aPath);
		bool IsExcludedPath(const std::filesystem::path& aPath);

	private:
		float myTextureSize = 96;

		std::filesystem::path myRoot = "Assets";
		std::filesystem::path myCurrentPath = "";
		std::filesystem::path mySelectedTreePath = "";

		std::array<Dyn::SRV*, eFileType::FILETYPE_COUNT> myFolderIcons;

		CU::DArray<std::string> myExcludedExtensions = { ".dll", ".exe", ".pdb", ".ini" };
		CU::DArray<ContentBrowserItem> myItems;

		std::map<std::filesystem::path, CU::DArray<std::filesystem::path>> myDirectories;

		int mySelectedItem = -1;
		int myRenamingItem = -1;
		bool myRenamingInProgress = false;
		std::string myNewFileName = "";

		std::string myDragDropPath = "";
	};
}
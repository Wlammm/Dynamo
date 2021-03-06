#include "EditorPch.h"
#include "ContentBrowser.h"
#include "StringUtils.hpp"
#include "MaterialEditor.h"
#include <fstream>
#include "Utils/FileUtils.h"

namespace fs = std::filesystem;

namespace Editor
{
	ContentBrowser::ContentBrowser()
		: EditorWindow("Content Browser")
	{
		myCurrentPath = myRoot;

		myFolderIcons[FILETYPE_FOLDER] = Dyn::ResourceFactory::GetSRV("Assets/Editor/ContentBrowser/Icon_Folder.dds");
		myFolderIcons[FILETYPE_JSON] = Dyn::ResourceFactory::GetSRV("Assets/Editor/ContentBrowser/Icon_Json.dds");
		myFolderIcons[FILETYPE_DDS] = Dyn::ResourceFactory::GetSRV("Assets/Editor/ContentBrowser/Icon_DDS.dds");
		myFolderIcons[FILETYPE_FBX] = Dyn::ResourceFactory::GetSRV("Assets/Editor/ContentBrowser/Icon_FBX.dds");
		myFolderIcons[FILETYPE_PNG] = Dyn::ResourceFactory::GetSRV("Assets/Editor/ContentBrowser/Icon_PNG.dds");
		myFolderIcons[FILETYPE_CPP] = Dyn::ResourceFactory::GetSRV("Assets/Editor/ContentBrowser/Icon_CPP.dds");
		myFolderIcons[FILETYPE_H] = Dyn::ResourceFactory::GetSRV("Assets/Editor/ContentBrowser/Icon_HPP.dds");
		myFolderIcons[FILETYPE_HLSL] = Dyn::ResourceFactory::GetSRV("Assets/Editor/ContentBrowser/Icon_HLSL.dds");
		myFolderIcons[FILETYPE_WAV] = Dyn::ResourceFactory::GetSRV("Assets/Editor/ContentBrowser/Icon_WAV.dds");
		myFolderIcons[FILETYPE_UNKNOWN] = Dyn::ResourceFactory::GetSRV("Assets/Editor/ContentBrowser/Icon_Unknown.dds");

		LoadAllDirectories(myRoot);
		LoadDirectory(myCurrentPath);
	}

	void ContentBrowser::Update()
	{
		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, 200);

		DrawDirectoriesView(myRoot, 1);

		ImGui::NextColumn();
		ImGui::BeginChild("FileWindow");

		UpdateRightClickMenu();
		float padding = 20;
		float cellWidth = myTextureSize + padding;

		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = static_cast<int>(panelWidth / cellWidth);

		if (columnCount)
		{
			ImGui::Columns(columnCount, 0, false);
			for (int i = 0; i < myItems.sizeI(); ++i)
			{
				bool wasSelected = false;
				if (i == mySelectedItem)
				{
					wasSelected = true;
					ImGui::PushStyleColor(ImGuiCol_Border, { 0, 0, 1, 1 });
				}

				ImGui::PushID(i);
				if (ImGui::ImageButton(myItems[i].mySRV->Get(), {myTextureSize, myTextureSize}))
				{
					mySelectedItem = i;
				}
				
				if (wasSelected)
				{
					ImGui::PopStyleColor();
				}

				if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
				{
					if (fs::is_directory(myItems[i].myPath))
					{
						LoadDirectory(myItems[i].myPath.string());
						ImGui::SetScrollHereY(0);
						ImGui::PopID();
						ImGui::EndChild();
						return;
					}

					HandleDoubleClick(myItems[i]);
				}

				if (!std::filesystem::is_directory(myItems[i].myPath))
				{
					if (ImGui::BeginDragDropSource())
					{
						ImGui::Text(myItems[i].myPath.string().c_str());
						myDragDropPath = myItems[i].myPath.string();
						ImGui::SetDragDropPayload(GetPayloadType(myItems[i].myPath).c_str(), &myDragDropPath, sizeof(myDragDropPath));
						ImGui::EndDragDropSource();
					}
				}

				ImGui::PopID();

				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
				if (i != myRenamingItem)
				{
					ImGui::Text(myItems[i].myPath.filename().string().c_str());
				}
				else
				{
					ImGui::InputText("##renamingfilecontentbrowser", &myNewFileName);
					ImGui::SetKeyboardFocusHere(0);
				}

				ImGui::NextColumn();
			}
		}

		UpdateRenaming();
		UpdateShortcuts();
		ImGui::EndChild();
	}

	void ContentBrowser::LoadDirectory(const std::filesystem::path& aPath)
	{
		mySelectedItem = -1;
		myCurrentPath = aPath;
		myItems.clear();

		for (auto entry : fs::directory_iterator(aPath))
		{
			if (entry.is_directory())
			{
				myItems.push_back({ entry.path(), myFolderIcons[FILETYPE_FOLDER] });
			}
		}

		for (auto entry : fs::directory_iterator(aPath))
		{
			if (entry.is_directory())
				continue;

			if (IsExcludedPath(entry.path()))
				continue;

			myItems.push_back({ entry.path(), GetSRVFromPath(entry.path()) });
		}
	}

	void ContentBrowser::LoadAllDirectories(const std::filesystem::path& aPath)
	{
		for (auto entry : fs::recursive_directory_iterator(aPath))
		{
			if(entry.is_directory())
				myDirectories[entry.path().parent_path()].Add(entry.path());
		}
	}

	void ContentBrowser::DrawDirectoriesView(const std::filesystem::path& aPath, const int aID)
	{
		ImGui::PushID(aID);

		ImGuiTreeNodeFlags flags = ((mySelectedTreePath == aPath) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanFullWidth;

		if (myDirectories[aPath].empty())
		{
			flags |= ImGuiTreeNodeFlags_Leaf;
		}

		bool isOpen = ImGui::TreeNodeEx(aPath.filename().string().c_str(), flags);

		if (ImGui::IsItemClicked())
		{
			mySelectedTreePath = aPath;
			LoadDirectory(aPath);
		}

		if (isOpen)
		{
			for (int i = 0; i < myDirectories[aPath].sizeI(); ++i)
			{
				DrawDirectoriesView(myDirectories[aPath][i], aID + i + 1);
			}

			ImGui::TreePop();
		}

		ImGui::PopID();
	}

	void ContentBrowser::UpdateRightClickMenu()
	{
		if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) && ImGui::IsWindowHovered())
		{
			ImGui::OpenPopup("ContentBrowserPopup");
		}

		if (ImGui::BeginPopup("ContentBrowserPopup"))
		{
			if (ImGui::BeginMenu("New"))
			{
				if (ImGui::MenuItem("Material"))
				{
					fs::path matPath = myCurrentPath;
					matPath.append("unnamedmaterial.dynmaterial");
					matPath = Dyn::FileUtils::GetFreePath(matPath);

					Dyn::MaterialFactory::CreateMaterial(matPath);
					LoadDirectory(myCurrentPath);
					for (int i = 0; i < myItems.sizeI(); ++i)
					{
						if (myItems[i].myPath == matPath)
						{
							mySelectedItem = i;
							BeginRenaming();
							break;
						}
					}
				}

				ImGui::EndMenu();
			}

			ImGui::Separator();

			if (ImGui::MenuItem("Open In File Explorer"))
			{
				ShellExecuteA(NULL, "open", std::filesystem::absolute(myCurrentPath).string().c_str(), NULL, NULL, SW_SHOWDEFAULT);
			}

			if (ImGui::MenuItem("Refresh", "F5"))
			{
				LoadDirectory(myCurrentPath);
			}

			ImGui::Separator();

			if (ImGui::MenuItem("New Folder"))
			{
				fs::path dirPath = myCurrentPath;
				dirPath.append("unnamed.dynmaterial");
				fs::create_directory(dirPath);
				LoadDirectory(myCurrentPath);

				for (int i = 0; i < myItems.sizeI(); ++i)
				{
					if (myItems[i].myPath == dirPath)
					{
						mySelectedItem = i;
						BeginRenaming();
						break;
					}
				}
			}

			if (ImGui::MenuItem("Import... (PH)"))
			{

			}

			if (ImGui::MenuItem("Rename", "F2", false, mySelectedItem != -1))
			{
				BeginRenaming();
			}

			ImGui::EndPopup();
		}
	}

	void ContentBrowser::BeginRenaming()
	{
		if (mySelectedItem != -1)
		{
			myRenamingItem = mySelectedItem;
			myNewFileName = myItems[mySelectedItem].myPath.filename().string();
			myRenamingInProgress = true;
		}
	}

	void ContentBrowser::UpdateRenaming()
	{
		if (myRenamingInProgress)
		{
			if (Input::IsKeyDown(KeyCode::Enter) || Input::IsKeyDown(KeyCode::Escape))
			{
				myRenamingInProgress = false;
				fs::path newPath = myItems[myRenamingItem].myPath;

				newPath.remove_filename();
				newPath.append(myNewFileName);

				fs::rename(myItems[myRenamingItem].myPath, newPath);
				myItems[myRenamingItem].myPath = newPath;
				myRenamingItem = -1;
				LoadDirectory(myCurrentPath);
			}
		}
	}

	void ContentBrowser::UpdateShortcuts()
	{
		if (Input::IsKeyDown(MouseButton::ScrollForward) && Input::IsKeyPressed(KeyCode::LeftControl))
		{
			myTextureSize += 4;
		}

		if (Input::IsKeyDown(MouseButton::ScrollBackwards) && Input::IsKeyPressed(KeyCode::LeftControl))
		{
			myTextureSize -= 4;

			if (myTextureSize < 1)
				myTextureSize = 1;
		}

		if (Input::IsKeyDown(KeyCode::F5))
		{
			LoadDirectory(myCurrentPath);
		}

		if (Input::IsKeyDown(KeyCode::F2))
		{
			if (mySelectedItem != -1)
			{
				BeginRenaming();
			}
		}
	}

	void ContentBrowser::HandleDoubleClick(const ContentBrowserItem& aClickedItem)
	{
		std::string extension = CU::StringUtils::ToLower(aClickedItem.myPath.extension().string());
		if (extension == ".dynmaterial")
		{
			MaterialEditor* matEditor = nullptr;
			if (!(matEditor = Main::GetEditorManager()->GetWindow<MaterialEditor>()))
			{
				matEditor = Main::GetEditorManager()->AddWindow(new MaterialEditor());
			}

			matEditor->SetSelectedMaterial(Dyn::MaterialFactory::GetMaterial(aClickedItem.myPath.string()));
		}
	}

	std::string ContentBrowser::GetPayloadType(const std::filesystem::path& aPath)
	{
		return CU::StringUtils::ToLower(aPath.extension().string());
	}

	Dyn::SRV* ContentBrowser::GetSRVFromPath(const std::filesystem::path& aPath)
	{
		if (std::filesystem::is_directory(aPath))
			return myFolderIcons[FILETYPE_FOLDER];

		std::string lowExtension = CU::StringUtils::ToLower(aPath.extension().string());
		if (lowExtension == ".json")
		{
			return myFolderIcons[FILETYPE_JSON];
		}

		if (lowExtension == ".dds")
		{
			Dyn::SRV* srv = Dyn::ResourceFactory::GetSRV(aPath.string());
			return srv;
		}

		if (lowExtension == ".png")
		{
			return myFolderIcons[FILETYPE_PNG];
		}

		if (lowExtension == ".fbx")
		{
			return myFolderIcons[FILETYPE_FBX];
		}

		if (lowExtension == ".cpp")
		{
			return myFolderIcons[FILETYPE_CPP];
		}

		if (lowExtension == ".h" || lowExtension == ".hpp")
		{
			return myFolderIcons[FILETYPE_H];
		}

		if (lowExtension == ".hlsl" || lowExtension == ".fx" || lowExtension == ".hlsli")
		{
			return myFolderIcons[FILETYPE_HLSL];
		}

		if (lowExtension == ".wav")
		{
			return myFolderIcons[FILETYPE_WAV];
		}

		return myFolderIcons[FILETYPE_UNKNOWN];
	}

	bool ContentBrowser::IsExcludedPath(const std::filesystem::path& aPath)
	{
		auto extension = aPath.extension();
		std::string lowExtension = CU::StringUtils::ToLower(extension.string());
		if (myExcludedExtensions.Contains(lowExtension))
			return true;
		return false;
	}
}

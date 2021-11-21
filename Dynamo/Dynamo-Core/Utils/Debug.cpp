#include "pch.h"
#include "Debug.h"

namespace Dynamo
{
	bool Debug::isOpen = false;
	std::vector<std::pair<std::string, std::function<void(void)>>> Debug::myImGuiDraws;

	void Dynamo::Debug::ImGui(const std::string& aName, const std::function<void(void)>& aFunction)
	{
		if (isOpen)
		{
			myImGuiDraws.push_back({ aName, aFunction });
		}
	}

	void Debug::Update()
	{
		if (isOpen)
		{
			ImGui::SetNextWindowSize(ImVec2(600, 400), ImGuiCond_FirstUseEver);

			ImGui::Begin("Debug##DebugManager");
			{
				ImGui::Text("F1 to toggle");
				ImGui::Separator();
				int index = 0;
				for (auto& entry : myImGuiDraws)
				{
					const auto& label = entry.first;
					const auto& callback = entry.second;

					ImGui::PushID(index);
					if (ImGui::CollapsingHeader(label.c_str(), ImGuiTreeNodeFlags_None))
					{
						callback();
					}
					ImGui::PopID();

					index++;
				}
			}
			ImGui::End();

			myImGuiDraws.clear();
		}

		if (Input::IsKeyDown(KeyCode::F1))
		{
			isOpen = !isOpen;
		}
	}
}

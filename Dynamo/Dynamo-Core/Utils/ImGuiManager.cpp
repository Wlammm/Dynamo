#include "pch.h"
#include "ImGuiManager.h"
#include "tahoma.h"

namespace Dynamo
{
	void ImGuiManager::Start()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGui::StyleColorsDark();
		ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

		ImGui_ImplWin32_Init(Main::GetHWND());
		ImGui_ImplDX11_Init(Main::GetDevice(), Main::GetContext());

		ImGuiIO* io = &ImGui::GetIO();
		ImFontConfig fontConfig;
		fontConfig.FontDataOwnedByAtlas = false;
		io->Fonts->AddFontFromMemoryTTF((void*)tahoma, sizeof(tahoma), 16.0f, &fontConfig);
		ImGui::MergeIconsWithLatestFont(12.f, false);
	}

	void ImGuiManager::BeginFrame()
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();

		ImGui::NewFrame();
		ImGuizmo::BeginFrame();
	}

	void ImGuiManager::Render()
	{
		// ImGui Notify.
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.f); 
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(43.f / 255.f, 43.f / 255.f, 43.f / 255.f, 100.f / 255.f)); 
		ImGui::RenderNotifications(); 
		ImGui::PopStyleVar(1); 
		ImGui::PopStyleColor(1);

		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		ImGui::GetIO().DisplaySize = ImGui::GetMainViewport()->WorkSize;


		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		ImGui::EndFrame();
	}

	void ImGuiManager::Destroy()
	{
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}
}
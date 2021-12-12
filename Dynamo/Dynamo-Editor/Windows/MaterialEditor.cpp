#include "EditorPch.h"
#include "MaterialEditor.h"

namespace Editor
{
	MaterialEditor::MaterialEditor()
		: EditorWindow("Material Editor")
	{
	}

	void MaterialEditor::Update()
	{
		ImGui::Text("Material name");
		ImGui::Separator();

		ImGui::Columns(2, 0, false);
		ImGui::SetColumnWidth(0, 150);

		ImGui::Text("Pixel Shader");
		ImGui::NextColumn();
		ImGui::Button("ForwardPS.cso##materialeditorpixelshader", ImVec2(450, 20));
		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip("ForwardPS.cso");
		}

		ImGui::NextColumn();
		ImGui::Text("Vertex Shader");
		ImGui::NextColumn();
		ImGui::Button("VertexShader.cso##materialeditorvertexshader", ImVec2(450, 20));
		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip("VertexShader.cso");
		}

		ImGui::Separator();

		ImGui::NextColumn();
		ImGui::Text("Albedo");
		ImGui::NextColumn();
		ImGui::Image(Dyn::ResourceFactory::GetSRV("Assets/Textures/Default_C.dds"), ImVec2(16, 16));
		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip("Assets/Textures/Default_C.dds");
		}

		ImGui::NextColumn();
		ImGui::Text("Normal");
		ImGui::NextColumn();
		ImGui::Image(Dyn::ResourceFactory::GetSRV("Assets/Textures/Default_N.dds"), ImVec2(16, 16));
		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip("Assets/Textures/Default_N.dds");
		}

		ImGui::NextColumn();
		ImGui::Text("Material");
		ImGui::NextColumn();
		ImGui::Image(Dyn::ResourceFactory::GetSRV("Assets/Textures/Default_M.dds"), ImVec2(16, 16));
		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip("Assets/Textures/Default_M.dds");
		}

		ImGui::Separator();

		ImGui::NextColumn();
		ImGui::Text("Roughness Interp");
		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip("Interpolation between material roughness and roughness. \n0 = roughness\n1 = material roughness");
		}

		ImGui::NextColumn();
		static float test = 0;
		ImGui::DragFloat("##materialeditorroughnessinterp", &test, 0.01f, 0, 1);
		ImGui::NextColumn();
		ImGui::Text("Roughness");
		ImGui::NextColumn();
		ImGui::DragFloat("##materialeditorroughness", &test, 0.01f, 0, 1);

		ImGui::Spacing();

		ImGui::NextColumn();
		ImGui::Text("Metalness Interp");
		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip("Interpolation between material metalness and metalness. \n0 = metalness\n1 = material metalness");
		}

		ImGui::NextColumn();
		ImGui::DragFloat("##materialeditormetalnessinterp", &test, 0.01f, 0, 1);
		ImGui::NextColumn();
		ImGui::Text("Metalness");
		ImGui::NextColumn();
		ImGui::DragFloat("##materialeditormetalness", &test, 0.01f, 0, 1);

		ImGui::Separator();

		ImGui::Columns(1);
		if (ImGui::CollapsingHeader("Custom##materialeditorcustom"))
		{
			ImGui::Columns(2, 0, false);
			ImGui::Text("Custom Texture 1");
			ImGui::NextColumn();
			ImGui::Image(Dyn::ResourceFactory::GetSRV("Assets/Textures/NoImage.dds"), ImVec2(16, 16));
			if (ImGui::IsItemHovered())
			{
				ImGui::SetTooltip("No image");
			}

			ImGui::NextColumn();
			ImGui::Text("Custom Texture 2");
			ImGui::NextColumn();
			ImGui::Image(Dyn::ResourceFactory::GetSRV("Assets/Textures/NoImage.dds"), ImVec2(16, 16));
			if (ImGui::IsItemHovered())
			{
				ImGui::SetTooltip("No image");
			}

			ImGui::NextColumn();
			ImGui::Text("Custom Texture 3");
			ImGui::NextColumn();
			ImGui::Image(Dyn::ResourceFactory::GetSRV("Assets/Textures/NoImage.dds"), ImVec2(16, 16));
			if (ImGui::IsItemHovered())
			{
				ImGui::SetTooltip("No image");
			}

			ImGui::NextColumn();
			ImGui::Text("Custom Texture 4");
			ImGui::NextColumn();
			ImGui::Image(Dyn::ResourceFactory::GetSRV("Assets/Textures/NoImage.dds"), ImVec2(16, 16));
			if (ImGui::IsItemHovered())
			{
				ImGui::SetTooltip("No image");
			}

			ImGui::Separator();

			ImGui::NextColumn();
			ImGui::Text("Custom Value 1");
			ImGui::NextColumn();
			ImGui::InputFloat("##materialeditorcustomvalue1", &test);

			ImGui::NextColumn();
			ImGui::Text("Custom Value 2");
			ImGui::NextColumn();
			ImGui::InputFloat("##materialeditorcustomvalue2", &test);

			ImGui::NextColumn();
			ImGui::Text("Custom Value 3");
			ImGui::NextColumn();
			ImGui::InputFloat("##materialeditorcustomvalue3", &test);

			ImGui::NextColumn();
			ImGui::Text("Custom Value 4");
			ImGui::NextColumn();
			ImGui::InputFloat("##materialeditorcustomvalue4", &test);
		}
	}
}

#include "EditorPch.h"
#include "MaterialEditor.h"
#include "Rendering/Material.h"

namespace Editor
{
	MaterialEditor::MaterialEditor()
		: EditorWindow("Material Editor")
	{
	}

	MaterialEditor::~MaterialEditor()
	{
		if (myMaterial)
			Dyn::MaterialFactory::SaveMaterial(myMaterial);
	}

	void MaterialEditor::Update()
	{
		if (!myMaterial)
		{
			ImGui::Text("Double-click a material to edit it here.");
			return;
		}

		Save();

		ImGui::Text(myMaterial->myMaterialPath.filename().string().c_str());
		ImGui::Separator();

		ImGui::Columns(2, 0, false);
		ImGui::SetColumnWidth(0, 150);

		DrawRenderers();

		ImGui::NextColumn();

		ImGui::Separator();

		DrawShaderPart();

		ImGui::Separator();
		DrawAlbedoTexture();

		ImGui::NextColumn();
		DrawNormalTexture();

		ImGui::NextColumn();
		DrawMaterialTexture();

		ImGui::Separator();
		ImGui::NextColumn();

		DrawMaterialSettings();

		ImGui::Separator();

		ImGui::NextColumn();
		
		DrawCustomMatrial();

		ImGui::Separator();

		DrawCustomPart();
	}

	void MaterialEditor::SetSelectedMaterial(Dyn::Material* aMaterial)
	{
		SetFocus();
		if (myMaterial)
		{
			Dyn::MaterialFactory::SaveMaterial(myMaterial);
		}

		myMaterial = aMaterial;
	}

	void MaterialEditor::Save()
	{
		mySaveProgress += Time::GetDeltaTime();

		if (mySaveProgress > mySaveDuration)
		{
			mySaveProgress = 0;
			Dyn::MaterialFactory::SaveMaterial(myMaterial);
		}
	}
	void MaterialEditor::DrawAlbedoTexture()
	{
		ImGui::NextColumn();
		ImGui::Text("Albedo");
		ImGui::NextColumn();
		Dyn::SRV* albedoSRV = Dyn::ResourceFactory::GetSRV("Assets/Textures/Default_C.dds");
		if (myMaterial->myAlbedo)
			albedoSRV = myMaterial->myAlbedo;

		ImGui::Image(albedoSRV->Get(), ImVec2(16, 16));

		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip("Assets/Textures/Default_C.dds");
		}

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(".dds"))
			{
				std::string path = *(std::string*)payload->Data;
				myMaterial->myAlbedo = Dyn::ResourceFactory::GetSRV(path);
			}

			ImGui::EndDragDropTarget();
		}
	}

	void MaterialEditor::DrawNormalTexture()
	{
		ImGui::Text("Normal");
		ImGui::NextColumn();
		Dyn::SRV* normalSRV = Dyn::ResourceFactory::GetSRV("Assets/Textures/Default_N.dds");
		if (myMaterial->myNormal)
			normalSRV = myMaterial->myNormal;

		ImGui::Image(normalSRV->Get(), ImVec2(16, 16));
		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip("Assets/Textures/Default_N.dds");
		}

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(".dds"))
			{
				std::string path = *(std::string*)payload->Data;
				myMaterial->myNormal = Dyn::ResourceFactory::GetSRV(path);
			}

			ImGui::EndDragDropTarget();
		}
	}

	void MaterialEditor::DrawMaterialTexture()
	{
		ImGui::Text("Material");
		ImGui::NextColumn();
		Dyn::SRV* materialSRV = Dyn::ResourceFactory::GetSRV("Assets/Textures/Default_M.dds");
		if (myMaterial->myMaterial)
			materialSRV = myMaterial->myMaterial;

		ImGui::Image(materialSRV->Get(), ImVec2(16, 16));
		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip("Assets/Textures/Default_M.dds");
		}

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(".dds"))
			{
				std::string path = *(std::string*)payload->Data;
				myMaterial->myMaterial = Dyn::ResourceFactory::GetSRV(path);
			}

			ImGui::EndDragDropTarget();
		}
	}

	void MaterialEditor::DrawCustomPart()
	{
		ImGui::Columns(1);
		if (ImGui::CollapsingHeader("Custom##materialeditorcustom"))
		{
			ImGui::Columns(2, 0, false);
			ImGui::Text("Custom Texture 1");
			ImGui::NextColumn();
			Dyn::SRV* customSRV1 = Dyn::ResourceFactory::GetSRV("Assets/Textures/NoImage.dds");
			if (myMaterial->myCustomTextures[0])
				customSRV1 = myMaterial->myCustomTextures[0];

			ImGui::Image(customSRV1->Get(), ImVec2(16, 16));
			if (ImGui::IsItemHovered())
			{
				ImGui::SetTooltip("No image");
			}

			ImGui::NextColumn();
			ImGui::Text("Custom Texture 2");
			ImGui::NextColumn();
			Dyn::SRV* customSRV2 = Dyn::ResourceFactory::GetSRV("Assets/Textures/NoImage.dds");
			if (myMaterial->myCustomTextures[1])
				customSRV2 = myMaterial->myCustomTextures[2];
			ImGui::Image(customSRV2->Get(), ImVec2(16, 16));
			if (ImGui::IsItemHovered())
			{
				ImGui::SetTooltip("No image");
			}

			ImGui::NextColumn();
			ImGui::Text("Custom Texture 3");
			ImGui::NextColumn();
			Dyn::SRV* customSRV3 = Dyn::ResourceFactory::GetSRV("Assets/Textures/NoImage.dds");
			if (myMaterial->myCustomTextures[2])
				customSRV3 = myMaterial->myCustomTextures[2];
			ImGui::Image(customSRV3->Get(), ImVec2(16, 16));
			if (ImGui::IsItemHovered())
			{
				ImGui::SetTooltip("No image");
			}

			ImGui::NextColumn();
			ImGui::Text("Custom Texture 4");
			ImGui::NextColumn();
			Dyn::SRV* customSRV4 = Dyn::ResourceFactory::GetSRV("Assets/Textures/NoImage.dds");
			if (myMaterial->myCustomTextures[3])
				customSRV4 = myMaterial->myCustomTextures[3];

			ImGui::Image(customSRV4->Get(), ImVec2(16, 16));
			if (ImGui::IsItemHovered())
			{
				ImGui::SetTooltip("No image");
			}

			ImGui::Separator();

			ImGui::NextColumn();
			ImGui::Text("Custom Value 1");
			ImGui::NextColumn();
			ImGui::InputFloat("##materialeditorcustomvalue1", &myMaterial->myCustomValues[0]);

			ImGui::NextColumn();
			ImGui::Text("Custom Value 2");
			ImGui::NextColumn();
			ImGui::InputFloat("##materialeditorcustomvalue2", &myMaterial->myCustomValues[1]);

			ImGui::NextColumn();
			ImGui::Text("Custom Value 3");
			ImGui::NextColumn();
			ImGui::InputFloat("##materialeditorcustomvalue3", &myMaterial->myCustomValues[2]);

			ImGui::NextColumn();
			ImGui::Text("Custom Value 4");
			ImGui::NextColumn();
			ImGui::InputFloat("##materialeditorcustomvalue4", &myMaterial->myCustomValues[3]);
		}
	}

	void MaterialEditor::DrawShaderPart()
	{
		ImGui::Text("Pixel Shader");
		ImGui::NextColumn();
		Dyn::Shader* pixelShader = myMaterial->myPixelShader;
		std::string psName = "";
		if (pixelShader)
		{
			psName = pixelShader->GetPath().filename().string();
		}
		psName += "##materialeditorpixelshader";

		ImGui::Button(psName.c_str(), ImVec2(450, 20));
		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip(psName.c_str());
		}

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(".cso"))
			{
				std::string path = *(std::string*)payload->Data;
				myMaterial->myPixelShader = Dyn::ShaderFactory::GetShader(path, Dyn::ShaderType::PixelShader);
			}
			ImGui::EndDragDropTarget();
		}

		ImGui::NextColumn();

		ImGui::Text("Vertex Shader");
		ImGui::NextColumn();
		Dyn::Shader* vertexShader = myMaterial->myVertexShader;
		std::string vsName = "";
		if (vertexShader)
		{
			vsName = vertexShader->GetPath().filename().string();
		}
		vsName += "##materialeditorvertexshader";

		ImGui::Button(vsName.c_str(), ImVec2(450, 20));
		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip(vsName.c_str());
		}

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(".cso"))
			{
				std::string path = *(std::string*)payload->Data;
				myMaterial->myVertexShader = Dyn::ShaderFactory::GetShader(path, Dyn::ShaderType::VertexShader);
			}

			ImGui::EndDragDropTarget();
		}
	}

	void MaterialEditor::DrawCustomMatrial()
	{
		ImGui::Text("Roughness Interp");
		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip("Interpolation between material roughness and roughness. \n0 = roughness\n1 = material roughness");
		}

		ImGui::NextColumn();
		static float test = 0;
		ImGui::DragFloat("##materialeditorroughnessinterp", &myMaterial->myRoughnessInterpolation, 0.01f, 0, 1);
		ImGui::NextColumn();
		ImGui::Text("Roughness");
		ImGui::NextColumn();
		ImGui::DragFloat("##materialeditorroughness", &myMaterial->myRoughnessConstant, 0.01f, 0, 1);

		ImGui::Spacing();

		ImGui::NextColumn();
		ImGui::Text("Metalness Interp");
		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip("Interpolation between material metalness and metalness. \n0 = metalness\n1 = material metalness");
		}

		ImGui::NextColumn();
		ImGui::DragFloat("##materialeditormetalnessinterp", &myMaterial->myMetalnessInterpolation, 0.01f, 0, 1);
		ImGui::NextColumn();
		ImGui::Text("Metalness");
		ImGui::NextColumn();
		ImGui::DragFloat("##materialeditormetalness", &myMaterial->myMetalnessConstant, 0.01f, 0, 1);
	}

	void MaterialEditor::DrawRenderers()
	{
		ImGui::Text("Renderer");
		ImGui::NextColumn();
		if (ImGui::BeginCombo("##materialeditorrenderertype", myRendererTypes[(int)myMaterial->myRenderer].c_str()))
		{
			for (int i = 0; i < 2; ++i)
			{
				if (ImGui::Selectable(myRendererTypes[i].c_str(), i == (int)myMaterial->myRenderer))
				{
					myMaterial->myRenderer = (Dyn::Renderer)i;

					if (myMaterial->myRenderer == Dyn::Renderer::Deferred)
					{
						myMaterial->myPixelShader = Dyn::ShaderFactory::GetShader("Shaders/GBuffer.cso", Dyn::ShaderType::PixelShader);
					}
					else
					{
						myMaterial->myPixelShader = Dyn::ShaderFactory::GetShader("Shaders/ForwardPS.cso", Dyn::ShaderType::PixelShader);
					}
				}
			}
			ImGui::EndCombo();
		}
	}

	void MaterialEditor::DrawMaterialSettings()
	{
		ImGui::Text("Receive Shadows");
		ImGui::NextColumn();
		ImGui::Checkbox("##materialeditorreceiveshadows", &myMaterial->myReceiveShadows);

		if (myMaterial->myRenderer == Dyn::Renderer::Forward)
		{
			ImGui::NextColumn();
			ImGui::Text("Depth Tested");
			ImGui::NextColumn();
			ImGui::Checkbox("##materialeditordepthtested", &myMaterial->myIsDepthTested);
		}
	}
}

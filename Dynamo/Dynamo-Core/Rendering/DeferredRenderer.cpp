#include "pch.h"
#include "DeferredRenderer.h"
#include "Components/Camera.h"
#include "Components/MeshRenderer.h"
#include "Rendering/Mesh.h"
#include "Rendering/Material.h"
#include "Utils/RenderUtils.h"

#include "Rendering/FullscreenEffects/BloomEffect.h"

#include "Components/DirectionalLight.h"
#include "Components/AmbientLight.h"
#include "Components/PointLight.h"
#include "Components/SpotLight.h"

namespace Dynamo
{
	DeferredRenderer::DeferredRenderer()
	{
		CreateBuffers();
		CreateShaders();
	}

	DeferredRenderer::~DeferredRenderer()
	{
	}

	void DeferredRenderer::GenerateGBuffer(const CU::DArray<MeshRenderer*>& someModels)
	{
		Camera* camera = Main::GetMainCamera();
		if (!camera)
			return;

		myFrameBufferData.myToCamera = camera->GetTransform()->GetMatrix().FastInverse();
		myFrameBufferData.myToProjection = camera->GetProjectionMatrix();
		myFrameBufferData.myCameraPosition = { camera->GetTransform()->GetPosition(), 1.0f };
		RenderUtils::MapBuffer<FrameBuffer>(myFrameBufferData, myFrameBuffer);

		Main::GetContext()->VSSetConstantBuffers(FRAME_BUFFER_SLOT, 1, &myFrameBuffer);
		Main::GetContext()->PSSetConstantBuffers(FRAME_BUFFER_SLOT, 1, &myFrameBuffer);

		myGBufferShader->Bind();
		for (MeshRenderer* model : someModels.AsVector())
		{
			if (!model)
				return;

			if (!model->IsInitialized())
				return;

			myObjectBufferData.myToWorld = model->GetTransform().GetMatrix();
			myObjectBufferData.myUVScale = { 1.0f, 1.0f };
			myObjectBufferData.myColor = model->GetColor();
			RenderUtils::MapBuffer<ObjectBuffer>(myObjectBufferData, myObjectBuffer);

			Main::GetContext()->VSSetConstantBuffers(OBJECT_BUFFER_SLOT, 1, &myObjectBuffer);
			Main::GetContext()->PSSetConstantBuffers(OBJECT_BUFFER_SLOT, 1, &myObjectBuffer);

			Main::GetContext()->PSSetShaderResources(ALBEDO_TEXTURE_SLOT, 3, &model->GetMaterial()->myAlbedo);

			myMeshVertexShader->Bind();
			if (Material* mat = model->GetMaterial())
			{
				if (mat->myVertexShader)
					mat->myVertexShader->Bind();
			}

			for (const Mesh& mesh : model->GetMeshes())
			{
				Main::GetContext()->IASetPrimitiveTopology((D3D11_PRIMITIVE_TOPOLOGY)mesh.myPrimitiveTopology);
				Main::GetContext()->IASetInputLayout(mesh.myInputLayout);
				Main::GetContext()->IASetVertexBuffers(0, 1, &mesh.myVertexBuffer, &mesh.myStride, &mesh.myOffset);
				Main::GetContext()->IASetIndexBuffer(mesh.myIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

				Main::GetContext()->DrawIndexed(mesh.myNumIndicies, 0, 0);
			}
		}
	}

	void DeferredRenderer::Render(const CU::DArray<DirectionalLight*>& someDirLights, const CU::DArray<AmbientLight*>& someAmbLights, const CU::DArray<PointLight*>& somePointLights, const CU::DArray<SpotLight*>& someSpotLights)
	{
		Main::GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		Main::GetContext()->IASetInputLayout(nullptr);
		Main::GetContext()->IASetVertexBuffers(0, 0, nullptr, nullptr, nullptr);
		Main::GetContext()->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);

		Camera* camera = Main::GetMainCamera();
		if (!camera)
			return;

		myFrameBufferData.myToCamera = camera->GetTransform()->GetMatrix().FastInverse();
		myFrameBufferData.myToProjection = camera->GetProjectionMatrix();
		myFrameBufferData.myCameraPosition = { camera->GetTransform()->GetPosition(), 1.0f };
		RenderUtils::MapBuffer<FrameBuffer>(myFrameBufferData, myFrameBuffer);
		Main::GetContext()->VSSetConstantBuffers(FRAME_BUFFER_SLOT, 1, &myFrameBuffer);
		Main::GetContext()->PSSetConstantBuffers(FRAME_BUFFER_SLOT, 1, &myFrameBuffer);

		myFSVertexShader->Bind();

		myDirLightShader->Bind();
		for (auto& dirLight : someDirLights.AsVector())
		{
			myDirLightBufferData.myToLight = { dirLight->GetDirection() * -1.0f, 0.0f };
			myDirLightBufferData.myColor = dirLight->GetColor();
			myDirLightBufferData.myIntensity = dirLight->GetIntensity();
			RenderUtils::MapBuffer<DirectionalLightBuffer>(myDirLightBufferData, myDirLightBuffer);

			Main::GetContext()->PSSetConstantBuffers(DIRECTIONAL_LIGHT_BUFFER_SLOT, 1, &myDirLightBuffer);
			Main::GetContext()->Draw(3, 0);
		}

		myAmbLightShader->Bind();
		for (auto& ambLight : someAmbLights.AsVector())
		{
			myAmbLightBufferData.myIntensity = ambLight->GetIntensity();
			RenderUtils::MapBuffer<AmbientLightBuffer>(myAmbLightBufferData, myAmbLightBuffer);
			Main::GetContext()->PSSetConstantBuffers(AMBIENT_LIGHT_BUFFER_SLOT, 1, &myAmbLightBuffer);

			Main::GetContext()->PSSetShaderResources(CUBEMAP_TEXTURE_SLOT, 1, ambLight->GetCubeMapConst());

			Main::GetContext()->Draw(3, 0);
		}

		myPointLightShader->Bind();
		uint numPointLights = DEFERRED_POINT_LIGHT_COUNT;
		for (uint i = 0; i < DEFERRED_POINT_LIGHT_COUNT; ++i)
		{
			if (i == somePointLights.size())
			{
				numPointLights = i;
				break;
			}

			myPointLightBufferData.myLights[i].myColor = somePointLights[i]->GetColor();
			myPointLightBufferData.myLights[i].myIntensity = somePointLights[i]->GetIntensity() * globalPointLightIntensityMultiplier;
			myPointLightBufferData.myLights[i].myPosition = { somePointLights[i]->GetPosition(), 1.0f };
			myPointLightBufferData.myLights[i].myRange = somePointLights[i]->GetRange();
		}
		myPointLightBufferData.myLightCount = numPointLights;

		RenderUtils::MapBuffer<DeferredPointLightBuffer>(myPointLightBufferData, myPointLightBuffer);
		Main::GetContext()->PSSetConstantBuffers(POINT_LIGHT_BUFFER_SLOT, 1, &myPointLightBuffer);
		Main::GetContext()->Draw(3, 0);

		mySpotLightShader->Bind();
		uint numSpotLights = DEFERRED_SPOT_LIGHT_COUNT;
		for (uint i = 0; i < DEFERRED_SPOT_LIGHT_COUNT; ++i)
		{
			if (i == someSpotLights.size())
			{
				numSpotLights = i;
				break;
			}

			mySpotLightBufferData.myLights[i].myColor = someSpotLights[i]->GetColor();
			mySpotLightBufferData.myLights[i].myDirection = { someSpotLights[i]->GetDirection(), 0.0f };
			mySpotLightBufferData.myLights[i].myPosition = { someSpotLights[i]->GetTransform().GetPosition(),  1.0f};
			mySpotLightBufferData.myLights[i].myInnerAngle = someSpotLights[i]->GetInnerAngle();
			mySpotLightBufferData.myLights[i].myOuterAngle = someSpotLights[i]->GetOuterAngle();
			mySpotLightBufferData.myLights[i].myIntensity = someSpotLights[i]->GetIntensity() * globalSpotLightIntensityMultiplier;
			mySpotLightBufferData.myLights[i].myRange = someSpotLights[i]->GetRange();
		}
		mySpotLightBufferData.myLightCount = numSpotLights;

		RenderUtils::MapBuffer<DeferredSpotLightBuffer>(mySpotLightBufferData, mySpotLightBuffer);
		Main::GetContext()->PSSetConstantBuffers(SPOT_LIGHT_BUFFER_SLOT, 1, &mySpotLightBuffer);
		Main::GetContext()->Draw(3, 0);

		myEmissiveShader->Bind();
		myEmissiveBufferData.myIntensity = Main::GetScene()->GetEmissiveIntensity();
		RenderUtils::MapBuffer<EmissiveBuffer>(myEmissiveBufferData, myEmissiveBuffer);
		Main::GetContext()->PSSetConstantBuffers(CUSTOM_BUFFER_SLOT, 1, &myEmissiveBuffer);

		Main::GetContext()->Draw(3, 0);
	}

	void DeferredRenderer::DrawRenderPass(const int aPass)
	{
		switch (aPass)
		{
		case 1:
			Console::Log("RenderPass: Albedo");
			break;

		case 2:
			Console::Log("RenderPass: Normal");
			break;

		case 3:
			Console::Log("RenderPass: Roughness");
			break;

		case 4:
			Console::Log("RenderPass: Metalness");
			break;

		case 5:
			Console::Log("RenderPass: Ambient Occlusion");
			break;

		case 6:
			Console::Log("RenderPass: Emissive");
			break;
		}

		myPassBufferData.myPass = aPass;
		RenderUtils::MapBuffer<PassBuffer>(myPassBufferData, myPassBuffer);
		Main::GetContext()->PSSetConstantBuffers(CUSTOM_BUFFER_SLOT, 1, &myPassBuffer);

		Main::GetRenderManager().GetFullscreenRenderer().Render(myMaterialPassShader);
	}

	void DeferredRenderer::CreateBuffers()
	{
		RenderUtils::CreateBuffer<FrameBuffer>(myFrameBuffer);
		RenderUtils::CreateBuffer<ObjectBuffer>(myObjectBuffer);
		RenderUtils::CreateBuffer<DirectionalLightBuffer>(myDirLightBuffer);
		RenderUtils::CreateBuffer<AmbientLightBuffer>(myAmbLightBuffer);
		RenderUtils::CreateBuffer<PassBuffer>(myPassBuffer);
		RenderUtils::CreateBuffer<DeferredPointLightBuffer>(myPointLightBuffer);
		RenderUtils::CreateBuffer<DeferredSpotLightBuffer>(mySpotLightBuffer);
		RenderUtils::CreateBuffer<EmissiveBuffer>(myEmissiveBuffer);
	}

	void DeferredRenderer::CreateShaders()
	{
		myFSVertexShader = ShaderFactory::GetShader("Shaders/FullscreenVS.cso", ShaderType::VertexShader);
		myMeshVertexShader = ShaderFactory::GetShader("Shaders/VertexShader.cso", ShaderType::VertexShader);
		myMaterialPassShader = ShaderFactory::GetShader("Shaders/FullscreenPS-MaterialPass.cso", ShaderType::PixelShader);

		myGBufferShader = ShaderFactory::GetShader("Shaders/GBuffer.cso", ShaderType::PixelShader);
		myDirLightShader = ShaderFactory::GetShader("Shaders/DirectionalLightShader.cso", ShaderType::PixelShader);
		myAmbLightShader = ShaderFactory::GetShader("Shaders/AmbientLightShader.cso", ShaderType::PixelShader);
		myPointLightShader = ShaderFactory::GetShader("Shaders/PointLightShader.cso", ShaderType::PixelShader);
		mySpotLightShader = ShaderFactory::GetShader("Shaders/SpotLightShader.cso", ShaderType::PixelShader);
		myEmissiveShader = ShaderFactory::GetShader("Shaders/EmissiveShader.cso", ShaderType::PixelShader);
	}
}

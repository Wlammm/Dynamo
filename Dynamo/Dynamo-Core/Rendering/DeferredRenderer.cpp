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

	void DeferredRenderer::GenerateGBuffer(const CU::DArray<MeshCommand>& someModels)
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
		for (MeshCommand model : someModels.AsVector())
		{
			myObjectBufferData.myToWorld = model.myMatrix;
			myObjectBufferData.myUVScale = { 1.0f, 1.0f };
			myObjectBufferData.myColor = model.myColor;
			RenderUtils::MapBuffer<ObjectBuffer>(myObjectBufferData, myObjectBuffer);

			Main::GetContext()->VSSetConstantBuffers(OBJECT_BUFFER_SLOT, 1, &myObjectBuffer);
			Main::GetContext()->PSSetConstantBuffers(OBJECT_BUFFER_SLOT, 1, &myObjectBuffer);

			model.myMaterial->myAlbedo->Bind(ALBEDO_TEXTURE_SLOT);
			model.myMaterial->myNormal->Bind(NORMAL_TEXTURE_SLOT);
			model.myMaterial->myMaterial->Bind(MATERIAL_TEXTURE_SLOT);

			myMeshVertexShader->Bind();
			myMaterialBufferData.myRoughnessInterp = 1;
			myMaterialBufferData.myMetalnessInterp = 1;
			if (Material* mat = model.myMaterial)
			{
				if (mat->myVertexShader)
					mat->myVertexShader->Bind();

				myMaterialBufferData.myRoughnessConstant = mat->myRoughnessConstant;
				myMaterialBufferData.myRoughnessInterp = mat->myRoughnessInterpolation;
				myMaterialBufferData.myMetalnessConstant = mat->myMetalnessConstant;
				myMaterialBufferData.myMetalnessInterp = mat->myMetalnessInterpolation;
			}
			RenderUtils::MapBuffer<MaterialBuffer>(myMaterialBufferData, myMaterialBuffer);
			Main::GetContext()->PSSetConstantBuffers(MATERIAL_BUFFER_SLOT, 1, &myMaterialBuffer);

			Main::GetContext()->IASetPrimitiveTopology((D3D11_PRIMITIVE_TOPOLOGY)model.myMesh->myPrimitiveTopology);
			Main::GetContext()->IASetInputLayout(model.myMesh->myInputLayout);
			Main::GetContext()->IASetVertexBuffers(0, 1, &model.myMesh->myVertexBuffer, &model.myMesh->myStride, &model.myMesh->myOffset);
			Main::GetContext()->IASetIndexBuffer(model.myMesh->myIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

			Main::GetContext()->DrawIndexed(model.myMesh->myNumIndicies, 0, 0);
		}
	}

	void DeferredRenderer::Render(const CU::DArray<DirectionalLightCommand>& someDirLights, const CU::DArray<AmbientLightCommand>& someAmbLights, const CU::DArray<PointLightCommand>& somePointLights, const CU::DArray<SpotLightCommand>& someSpotLights)
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
			myDirLightBufferData.myToLight = { dirLight.myDirection * -1.0f, 0.0f };
			myDirLightBufferData.myColor = dirLight.myColor;
			myDirLightBufferData.myIntensity = dirLight.myIntensity;
			RenderUtils::MapBuffer<DirectionalLightBuffer>(myDirLightBufferData, myDirLightBuffer);

			Main::GetContext()->PSSetConstantBuffers(DIRECTIONAL_LIGHT_BUFFER_SLOT, 1, &myDirLightBuffer);
			Main::GetContext()->Draw(3, 0);
		}

		myAmbLightShader->Bind();
		for (auto& ambLight : someAmbLights.AsVector())
		{
			myAmbLightBufferData.myIntensity = ambLight.myIntensity;
			RenderUtils::MapBuffer<AmbientLightBuffer>(myAmbLightBufferData, myAmbLightBuffer);
			Main::GetContext()->PSSetConstantBuffers(AMBIENT_LIGHT_BUFFER_SLOT, 1, &myAmbLightBuffer);

			ambLight.myCubeMap->Bind(CUBEMAP_TEXTURE_SLOT);

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

			myPointLightBufferData.myLights[i].myColor = somePointLights[i].myColor;
			myPointLightBufferData.myLights[i].myIntensity = somePointLights[i].myIntensity * globalPointLightIntensityMultiplier;
			myPointLightBufferData.myLights[i].myPosition = { somePointLights[i].myPosition, 1.0f };
			myPointLightBufferData.myLights[i].myRange = somePointLights[i].myRange;
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

			mySpotLightBufferData.myLights[i].myColor = someSpotLights[i].myColor;
			mySpotLightBufferData.myLights[i].myDirection = { someSpotLights[i].myDirection, 0.0f };
			mySpotLightBufferData.myLights[i].myPosition = { someSpotLights[i].myPosition, 1.0f};
			mySpotLightBufferData.myLights[i].myInnerAngle = someSpotLights[i].myInnerAngle * Deg2Rad;
			mySpotLightBufferData.myLights[i].myOuterAngle = someSpotLights[i].myOuterAngle * Deg2Rad;
			mySpotLightBufferData.myLights[i].myIntensity = someSpotLights[i].myIntensity * globalSpotLightIntensityMultiplier;
			mySpotLightBufferData.myLights[i].myRange = someSpotLights[i].myRange;
		}
		mySpotLightBufferData.myLightCount = numSpotLights;

		RenderUtils::MapBuffer<DeferredSpotLightBuffer>(mySpotLightBufferData, mySpotLightBuffer);
		Main::GetContext()->PSSetConstantBuffers(SPOT_LIGHT_BUFFER_SLOT, 1, &mySpotLightBuffer);
		Main::GetContext()->Draw(3, 0);

		myEmissiveShader->Bind();
		myEmissiveBufferData.myIntensity = Main::GetScene()->GetEmissiveIntensity();
		RenderUtils::MapBuffer<EmissiveBuffer>(myEmissiveBufferData, myEmissiveBuffer);
		Main::GetContext()->PSSetConstantBuffers(EMISSIVE_BUFFER_SLOT, 1, &myEmissiveBuffer);

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

		case 7:
			Console::Log("RenderPass: NormalTexture");
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
		RenderUtils::CreateBuffer<MaterialBuffer>(myMaterialBuffer);
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

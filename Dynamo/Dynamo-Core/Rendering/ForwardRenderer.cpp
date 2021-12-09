#include "pch.h"
#include "ForwardRenderer.h"

#include "Utils/RenderUtils.h"

#include "Rendering/Mesh.h"
#include "Rendering/Material.h"

#include "Components/Camera.h"
#include "Components/MeshRenderer.h"
#include "Components/DirectionalLight.h"
#include "Components/AmbientLight.h"
#include "Components/PointLight.h"
#include "Components/SpotLight.h"

namespace Dynamo
{
	ForwardRenderer::ForwardRenderer()
	{
		myDefaultPixelShader = ShaderFactory::GetShader("Shaders/ForwardPS.cso", ShaderType::PixelShader);
		myDefaultVertexShader = ShaderFactory::GetShader("Shaders/VertexShader.cso", ShaderType::VertexShader);
		myDefaultMaterial = MaterialFactory::GetDefaultMaterial();
		
		CreateBuffers();
	}

	ForwardRenderer::~ForwardRenderer()
	{
		//myFrameBuffer->Release();
		//myObjectBuffer->Release();
		//myRSState->Release();
	}

	void ForwardRenderer::Render(const CU::DArray<MeshRenderer*>& someModels, const CU::DArray<DirectionalLight*>& someDirLights, const CU::DArray<AmbientLight*>& someAmbLights,
		const CU::DArray<PointLight*>& somePointLights, const CU::DArray<SpotLight*>& someSpotLights)
	{
		Camera* camera = Main::GetMainCamera();
		if (!camera)
			return;

		MapFrameBuffer(camera);
		MapDirLightBuffer(someDirLights.AsVector());
		MapAmbLightBuffer(someAmbLights.AsVector());
		MapEmissiveBuffer();
		
		for (MeshRenderer* model : someModels.AsVector())
		{
			MapPointLightBuffer(GetSortedPointLights(somePointLights.AsVector(), model));
			MapSpotLightBuffer(GetSortedSpotLights(someSpotLights.AsVector(), model));

			for (auto& mesh : model->GetMeshes())
			{
				MapObjectBuffer(model);
				SetMeshSettings(mesh);

				if (model->GetMaterial())
				{
					Main::GetContext()->VSSetShaderResources(ALBEDO_TEXTURE_SLOT, 3, &model->GetMaterial()->myAlbedo);
					Main::GetContext()->PSSetShaderResources(ALBEDO_TEXTURE_SLOT, 3, &model->GetMaterial()->myAlbedo);

					Shader* vs = model->GetMaterial()->myVertexShader;
					vs ? vs->Bind() : myDefaultVertexShader->Bind();

					Shader* ps = model->GetMaterial()->myPixelShader;
					ps ? ps->Bind() : myDefaultPixelShader->Bind();
				}
				else
				{
					Main::GetContext()->VSSetShaderResources(ALBEDO_TEXTURE_SLOT, 3, &myDefaultMaterial->myAlbedo);
					Main::GetContext()->PSSetShaderResources(ALBEDO_TEXTURE_SLOT, 3, &myDefaultMaterial->myAlbedo);

					myDefaultPixelShader->Bind();
					myDefaultVertexShader->Bind();
				}

				Main::GetContext()->DrawIndexed(mesh.myNumIndicies, 0, 0);
			}
		}
	}

	void ForwardRenderer::CreateBuffers()
	{
		RenderUtils::CreateBuffer<FrameBuffer>(myFrameBuffer);
		RenderUtils::CreateBuffer<ObjectBuffer>(myObjectBuffer);
		RenderUtils::CreateBuffer<DirectionalLightBuffer>(myDirLightBuffer);
		RenderUtils::CreateBuffer<AmbientLightBuffer>(myAmbLightBuffer);
		RenderUtils::CreateBuffer<ForwardPointLightBuffer>(myPointLightBuffer);
		RenderUtils::CreateBuffer<ForwardSpotLightBuffer>(mySpotLightBuffer);
		RenderUtils::CreateBuffer<EmissiveBuffer>(myEmissiveBuffer);
	}

	void ForwardRenderer::CreateRSStates()
	{
		D3D11_RASTERIZER_DESC desc;
		desc.FillMode = D3D11_FILL_SOLID;
		desc.CullMode = D3D11_CULL_BACK;

		HRESULT result = Main::GetDevice()->CreateRasterizerState(&desc, &myRSState);
		assert(SUCCEEDED(result));
	}

	std::vector<PointLight*> ForwardRenderer::GetSortedPointLights(const std::vector<PointLight*>& someInData, MeshRenderer* aInstance)
	{
		std::vector<PointLight*> lights = someInData;

		std::sort(lights.begin(), lights.end(), [aInstance](PointLight* aFirst, PointLight* aSecond)
			{
				float firstDist = (aFirst->GetPosition() - aInstance->GetGameObject()->GetTransform().GetPosition()).LengthSqr();
				float secondDist = (aSecond->GetPosition() - aInstance->GetGameObject()->GetTransform().GetPosition()).LengthSqr();
				return firstDist < secondDist;
			});

		return lights;
	}

	std::vector<SpotLight*> ForwardRenderer::GetSortedSpotLights(const std::vector<SpotLight*>& someInData, MeshRenderer* aInstance)
	{
		std::vector<SpotLight*> lights = someInData;

		std::sort(lights.begin(), lights.end(), [aInstance](SpotLight* aFirst, SpotLight* aSecond)
			{
				float firstDist = (aFirst->GetGameObject()->GetTransform().GetPosition() - aInstance->GetGameObject()->GetTransform().GetPosition()).LengthSqr();
				float secondDist = (aSecond->GetGameObject()->GetTransform().GetPosition() - aInstance->GetGameObject()->GetTransform().GetPosition()).LengthSqr();
				return firstDist < secondDist;
			});

		return lights;
	}

	void ForwardRenderer::MapFrameBuffer(Camera* aCamera)
	{
		myFrameBufferData.myToCamera = aCamera->GetTransform()->GetMatrix().FastInverse();
		myFrameBufferData.myToProjection = aCamera->GetProjectionMatrix();
		myFrameBufferData.myCameraPosition = { aCamera->GetTransform()->GetPosition(), 1.0f };
		RenderUtils::MapBuffer<FrameBuffer>(myFrameBufferData, myFrameBuffer);

		Main::GetContext()->VSSetConstantBuffers(FRAME_BUFFER_SLOT, 1, &myFrameBuffer);
		Main::GetContext()->PSSetConstantBuffers(FRAME_BUFFER_SLOT, 1, &myFrameBuffer);
	}

	void ForwardRenderer::MapObjectBuffer(MeshRenderer* aModel)
	{
		myObjectBufferData.myToWorld = aModel->GetTransform().GetMatrix();
		myObjectBufferData.myUVScale = { 1.0f, 1.0f };
		myObjectBufferData.myColor = aModel->GetColor();
		RenderUtils::MapBuffer<ObjectBuffer>(myObjectBufferData, myObjectBuffer);

		Main::GetContext()->VSSetConstantBuffers(OBJECT_BUFFER_SLOT, 1, &myObjectBuffer);
		Main::GetContext()->PSSetConstantBuffers(OBJECT_BUFFER_SLOT, 1, &myObjectBuffer);
	}

	void ForwardRenderer::SetMeshSettings(const Mesh& aMesh)
	{
		Main::GetContext()->IASetPrimitiveTopology(aMesh.myPrimitiveTopology);
		Main::GetContext()->IASetInputLayout(aMesh.myInputLayout);
		Main::GetContext()->IASetVertexBuffers(0, 1, &aMesh.myVertexBuffer, &aMesh.myStride, &aMesh.myOffset);
		Main::GetContext()->IASetIndexBuffer(aMesh.myIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	}

	void ForwardRenderer::MapDirLightBuffer(const std::vector<DirectionalLight*>& someDirLights)
	{
		if (!someDirLights.empty())
		{
			myDirLightBufferData.myToLight = { someDirLights.front()->GetDirection() * -1.0f, 0.0f};
			myDirLightBufferData.myColor = someDirLights.front()->GetColor();
			myDirLightBufferData.myIntensity = someDirLights.front()->GetIntensity();
		}
		RenderUtils::MapBuffer<DirectionalLightBuffer>(myDirLightBufferData, myDirLightBuffer);

		Main::GetContext()->PSSetConstantBuffers(DIRECTIONAL_LIGHT_BUFFER_SLOT, 1, &myDirLightBuffer);
	}
	
	void ForwardRenderer::MapAmbLightBuffer(const std::vector<AmbientLight*>& someAmbLights)
	{
		if (!someAmbLights.empty())
		{
			myAmbLightBufferData.myIntensity = someAmbLights.front()->GetIntensity();
			Main::GetContext()->PSSetShaderResources(CUBEMAP_TEXTURE_SLOT, 1, someAmbLights.front()->GetCubeMapConst());
		}

		RenderUtils::MapBuffer<AmbientLightBuffer>(myAmbLightBufferData, myAmbLightBuffer);

		Main::GetContext()->PSSetConstantBuffers(AMBIENT_LIGHT_BUFFER_SLOT, 1, &myAmbLightBuffer);
	}

	void ForwardRenderer::MapPointLightBuffer(const std::vector<PointLight*>& somePointLights)
	{
		uint numPointLights = FORWARD_POINT_LIGHT_COUNT;
		for (uint i = 0; i < FORWARD_POINT_LIGHT_COUNT; ++i)
		{
			if (i == somePointLights.size())
			{
				numPointLights = i;
				break;
			}

			myPointLightBufferData.myPointLights[i].myColor = somePointLights[i]->GetColor();
			myPointLightBufferData.myPointLights[i].myIntensity = somePointLights[i]->GetIntensity() * globalPointLightIntensityMultiplier;
			myPointLightBufferData.myPointLights[i].myRange = somePointLights[i]->GetRange();
			myPointLightBufferData.myPointLights[i].myPosition = { somePointLights[i]->GetPosition(), 1.0f };
		}

		myPointLightBufferData.myNumPointLights = numPointLights;

		RenderUtils::MapBuffer<ForwardPointLightBuffer>(myPointLightBufferData, myPointLightBuffer);

		Main::GetContext()->PSSetConstantBuffers(POINT_LIGHT_BUFFER_SLOT, 1, &myPointLightBuffer);
	}

	void ForwardRenderer::MapSpotLightBuffer(const std::vector<SpotLight*>& someSpotLights)
	{
		uint numSpotLights = FORWARD_SPOT_LIGHT_COUNT;
		for (uint i = 0; i < FORWARD_SPOT_LIGHT_COUNT; ++i)
		{
			if (i == someSpotLights.size())
			{
				numSpotLights = i;
				break;
			}

			mySpotLightBufferData.myLights[i].myColor = someSpotLights[i]->GetColor();
			mySpotLightBufferData.myLights[i].myDirection = { someSpotLights[i]->GetDirection(), 0.0f };
			mySpotLightBufferData.myLights[i].myPosition = { someSpotLights[i]->GetTransform().GetPosition(), 1.0f };
			mySpotLightBufferData.myLights[i].myIntensity = someSpotLights[i]->GetIntensity() * globalSpotLightIntensityMultiplier;
			mySpotLightBufferData.myLights[i].myRange = someSpotLights[i]->GetRange();
			mySpotLightBufferData.myLights[i].myInnerAngle = someSpotLights[i]->GetInnerAngle() * Deg2Rad;
			mySpotLightBufferData.myLights[i].myOuterAngle = someSpotLights[i]->GetOuterAngle() * Deg2Rad;
		}

		mySpotLightBufferData.myNumSpotLights = numSpotLights;

		RenderUtils::MapBuffer<ForwardSpotLightBuffer>(mySpotLightBufferData, mySpotLightBuffer);

		Main::GetContext()->PSSetConstantBuffers(SPOT_LIGHT_BUFFER_SLOT, 1, &mySpotLightBuffer);
	}

	void ForwardRenderer::MapEmissiveBuffer()
	{
		myEmissiveBufferData.myIntensity = Main::GetScene()->GetEmissiveIntensity();
		RenderUtils::MapBuffer<EmissiveBuffer>(myEmissiveBufferData, myEmissiveBuffer);
		Main::GetContext()->PSSetConstantBuffers(EMISSIVE_BUFFER_SLOT, 1, &myEmissiveBuffer);
	}
}
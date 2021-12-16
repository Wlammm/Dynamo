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
		myDefaultPixelShader = ShaderFactory::GetShader("Assets/Shaders/ForwardPS.cso", ShaderType::PixelShader);
		myDefaultVertexShader = ShaderFactory::GetShader("Assets/Shaders/VertexShader.cso", ShaderType::VertexShader);
		myDefaultMaterial = MaterialFactory::GetDefaultMaterial();
		
		CreateBuffers();
	}

	ForwardRenderer::~ForwardRenderer()
	{
		//myFrameBuffer->Release();
		//myObjectBuffer->Release();
		//myRSState->Release();
	}

	void ForwardRenderer::Render(const CU::DArray<MeshCommand>& someModels, const CU::DArray<DirectionalLightCommand>& someDirLights, const CU::DArray<AmbientLightCommand>& someAmbLights,
		const CU::DArray<PointLightCommand>& somePointLights, const CU::DArray<SpotLightCommand>& someSpotLights)
	{
		Camera* camera = Main::GetMainCamera();
		if (!camera)
			return;

		MapFrameBuffer(camera);
		MapDirLightBuffer(someDirLights.AsVector());
		MapAmbLightBuffer(someAmbLights.AsVector());
		MapEmissiveBuffer();
		
		for (const MeshCommand& model : someModels.AsVector())
		{
			MapPointLightBuffer(GetSortedPointLights(somePointLights.AsVector(), model));
			MapSpotLightBuffer(GetSortedSpotLights(someSpotLights.AsVector(), model));

			MapMaterialBuffer(model.myMaterial);

			MapObjectBuffer(model);
			SetMeshSettings(*model.myMesh);

			if (model.myMaterial)
			{
				model.myMaterial->myAlbedo->Bind(ALBEDO_TEXTURE_SLOT);
				model.myMaterial->myNormal->Bind(NORMAL_TEXTURE_SLOT);
				model.myMaterial->myMaterial->Bind(MATERIAL_TEXTURE_SLOT);

				Shader* vs = model.myMaterial->myVertexShader;
				vs ? vs->Bind() : myDefaultVertexShader->Bind();

				Shader* ps = model.myMaterial->myPixelShader;
				ps ? ps->Bind() : myDefaultPixelShader->Bind();
			}
			else
			{
				myDefaultMaterial->myAlbedo->Bind(ALBEDO_TEXTURE_SLOT);
				myDefaultMaterial->myNormal->Bind(NORMAL_TEXTURE_SLOT);
				myDefaultMaterial->myMaterial->Bind(MATERIAL_TEXTURE_SLOT);

				myDefaultPixelShader->Bind();
				myDefaultVertexShader->Bind();
			}

			Main::GetContext()->DrawIndexed(model.myMesh->myNumIndicies, 0, 0);
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
		RenderUtils::CreateBuffer<MaterialBuffer>(myMaterialBuffer);
	}

	void ForwardRenderer::CreateRSStates()
	{
		D3D11_RASTERIZER_DESC desc;
		desc.FillMode = D3D11_FILL_SOLID;
		desc.CullMode = D3D11_CULL_BACK;

		HRESULT result = Main::GetDevice()->CreateRasterizerState(&desc, &myRSState);
		DYN_ASSERT(SUCCEEDED(result));
	}

	std::vector<PointLightCommand> ForwardRenderer::GetSortedPointLights(const std::vector<PointLightCommand>& someInData, const MeshCommand& aInstance)
	{
		std::vector<PointLightCommand> lights = someInData;

		Vec3f pos;
		pos.x = aInstance.myMatrix(4, 1);
		pos.y = aInstance.myMatrix(4, 2);
		pos.z = aInstance.myMatrix(4, 3);

		std::sort(lights.begin(), lights.end(), [aInstance, pos](PointLightCommand aFirst, PointLightCommand aSecond)
			{
				float firstDist = (aFirst.myPosition - pos).LengthSqr();
				float secondDist = (aSecond.myPosition - pos).LengthSqr();
				return firstDist < secondDist;
			});

		return lights;
	}

	std::vector<SpotLightCommand> ForwardRenderer::GetSortedSpotLights(const std::vector<SpotLightCommand>& someInData, const MeshCommand& aInstance)
	{
		std::vector<SpotLightCommand> lights = someInData;

		Vec3f pos;
		pos.x = aInstance.myMatrix(4, 1);
		pos.y = aInstance.myMatrix(4, 2);
		pos.z = aInstance.myMatrix(4, 3);
		
		std::sort(lights.begin(), lights.end(), [aInstance, pos](SpotLightCommand aFirst, SpotLightCommand aSecond)
			{
				float firstDist = (aFirst.myPosition - pos).LengthSqr();
				float secondDist = (aSecond.myPosition - pos).LengthSqr();
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

	void ForwardRenderer::MapObjectBuffer(const MeshCommand& aMesh)
	{
		myObjectBufferData.myToWorld = aMesh.myMatrix;
		myObjectBufferData.myUVScale = { 1.0f, 1.0f };
		myObjectBufferData.myColor = aMesh.myColor;
		myObjectBufferData.myAdditiveColor = aMesh.myAdditiveColor;
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

	void ForwardRenderer::MapDirLightBuffer(const std::vector<DirectionalLightCommand>& someDirLights)
	{
		myDirLightBufferData.myIntensity = 0;
		if (!someDirLights.empty())
		{
			myDirLightBufferData.myToLight = { someDirLights.front().myDirection * -1.0f, 0.0f};
			myDirLightBufferData.myColor = someDirLights.front().myColor;
			myDirLightBufferData.myIntensity = someDirLights.front().myIntensity;
		}
		RenderUtils::MapBuffer<DirectionalLightBuffer>(myDirLightBufferData, myDirLightBuffer);

		Main::GetContext()->PSSetConstantBuffers(DIRECTIONAL_LIGHT_BUFFER_SLOT, 1, &myDirLightBuffer);
	}
	
	void ForwardRenderer::MapAmbLightBuffer(const std::vector<AmbientLightCommand>& someAmbLights)
	{
		myAmbLightBufferData.myIntensity = 0;
		if (!someAmbLights.empty())
		{
			myAmbLightBufferData.myIntensity = someAmbLights.front().myIntensity;
			someAmbLights.front().myCubeMap->Bind(CUBEMAP_TEXTURE_SLOT);
		}

		RenderUtils::MapBuffer<AmbientLightBuffer>(myAmbLightBufferData, myAmbLightBuffer);

		Main::GetContext()->PSSetConstantBuffers(AMBIENT_LIGHT_BUFFER_SLOT, 1, &myAmbLightBuffer);
	}

	void ForwardRenderer::MapPointLightBuffer(const std::vector<PointLightCommand>& somePointLights)
	{
		uint numPointLights = FORWARD_POINT_LIGHT_COUNT;
		for (uint i = 0; i < FORWARD_POINT_LIGHT_COUNT; ++i)
		{
			if (i == somePointLights.size())
			{
				numPointLights = i;
				break;
			}

			myPointLightBufferData.myPointLights[i].myColor = somePointLights[i].myColor;
			myPointLightBufferData.myPointLights[i].myIntensity = somePointLights[i].myIntensity * globalPointLightIntensityMultiplier;
			myPointLightBufferData.myPointLights[i].myRange = somePointLights[i].myRange;
			myPointLightBufferData.myPointLights[i].myPosition = { somePointLights[i].myPosition, 1.0f };
		}

		myPointLightBufferData.myNumPointLights = numPointLights;

		RenderUtils::MapBuffer<ForwardPointLightBuffer>(myPointLightBufferData, myPointLightBuffer);

		Main::GetContext()->PSSetConstantBuffers(POINT_LIGHT_BUFFER_SLOT, 1, &myPointLightBuffer);
	}

	void ForwardRenderer::MapSpotLightBuffer(const std::vector<SpotLightCommand>& someSpotLights)
	{
		uint numSpotLights = FORWARD_SPOT_LIGHT_COUNT;
		for (uint i = 0; i < FORWARD_SPOT_LIGHT_COUNT; ++i)
		{
			if (i == someSpotLights.size())
			{
				numSpotLights = i;
				break;
			}

			mySpotLightBufferData.myLights[i].myColor = someSpotLights[i].myColor;
			mySpotLightBufferData.myLights[i].myDirection = { someSpotLights[i].myDirection, 0.0f };
			mySpotLightBufferData.myLights[i].myPosition = { someSpotLights[i].myPosition, 1.0f };
			mySpotLightBufferData.myLights[i].myIntensity = someSpotLights[i].myIntensity * globalSpotLightIntensityMultiplier;
			mySpotLightBufferData.myLights[i].myRange = someSpotLights[i].myRange;
			mySpotLightBufferData.myLights[i].myInnerAngle = someSpotLights[i].myInnerAngle * Deg2Rad;
			mySpotLightBufferData.myLights[i].myOuterAngle = someSpotLights[i].myOuterAngle * Deg2Rad;
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

	void ForwardRenderer::MapMaterialBuffer(Material* aMaterial)
	{
		myMaterialBufferData.myMetalnessInterp = 1;
		myMaterialBufferData.myRoughnessInterp = 1;

		if (aMaterial)
		{
			myMaterialBufferData.myRoughnessConstant = aMaterial->myRoughnessConstant;
			myMaterialBufferData.myRoughnessInterp = aMaterial->myRoughnessInterpolation;
			myMaterialBufferData.myMetalnessConstant = aMaterial->myMetalnessConstant;
			myMaterialBufferData.myMetalnessInterp = aMaterial->myMetalnessInterpolation;

			myMaterialBufferData.myCustomValue1 = aMaterial->myCustomValues[0];
			myMaterialBufferData.myCustomValue2 = aMaterial->myCustomValues[1];
			myMaterialBufferData.myCustomValue3 = aMaterial->myCustomValues[2];
			myMaterialBufferData.myCustomValue4 = aMaterial->myCustomValues[3];
		}

		RenderUtils::MapBuffer<MaterialBuffer>(myMaterialBufferData, myMaterialBuffer);
		Main::GetContext()->PSSetConstantBuffers(MATERIAL_BUFFER_SLOT, 1, &myMaterialBuffer);
	}
}
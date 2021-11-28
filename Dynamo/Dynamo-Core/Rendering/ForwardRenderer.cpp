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
		myFrameBuffer->Release();
		myObjectBuffer->Release();
		myRSState->Release();
	}

	void ForwardRenderer::Render(const CU::DArray<MeshRenderer*>& someModels, const CU::DArray<DirectionalLight*>& someDirLights, const CU::DArray<AmbientLight*>& someAmbLights,
		const CU::DArray<PointLight*>& somePointLights, const CU::DArray<SpotLight*>& someSpotLights)
	{
		Camera* camera = Main::GetMainCamera();
		if (!camera)
			return;
		
		MapFrameBuffer(camera);
		MapDirLightBuffer(someDirLights);
		MapAmbLightBuffer(someAmbLights);
		
		for (MeshRenderer* model : someModels.AsVector())
		{
			MapPointLightBuffer(somePointLights);
			MapSpotLightBuffer(someSpotLights);

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
	}

	void ForwardRenderer::CreateRSStates()
	{
		D3D11_RASTERIZER_DESC desc;
		desc.FillMode = D3D11_FILL_SOLID;
		desc.CullMode = D3D11_CULL_BACK;

		HRESULT result = Main::GetDevice()->CreateRasterizerState(&desc, &myRSState);
		assert(SUCCEEDED(result));
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

	void ForwardRenderer::MapDirLightBuffer(const CU::DArray<DirectionalLight*>& someDirLights)
	{
		if (!someDirLights.empty())
		{
			myDirLightBufferData.myToLight = { someDirLights.front()->GetToLight(), 0.0f };
			myDirLightBufferData.myColor = someDirLights.front()->GetColor();
			myDirLightBufferData.myIntensity = someDirLights.front()->GetIntensity();
		}
		RenderUtils::MapBuffer<DirectionalLightBuffer>(myDirLightBufferData, myDirLightBuffer);

		Main::GetContext()->PSSetConstantBuffers(DIRECTIONAL_LIGHT_BUFFER_SLOT, 1, &myDirLightBuffer);
	}
	
	void ForwardRenderer::MapAmbLightBuffer(const CU::DArray<AmbientLight*>& someAmbLights)
	{
		if (!someAmbLights.empty())
		{
			myAmbLightBufferData.myIntensity = someAmbLights.front()->GetIntensity();
			Main::GetContext()->PSSetShaderResources(CUBEMAP_TEXTURE_SLOT, 1, someAmbLights.front()->GetCubeMapConst());
		}

		RenderUtils::MapBuffer<AmbientLightBuffer>(myAmbLightBufferData, myAmbLightBuffer);

		Main::GetContext()->PSSetConstantBuffers(AMBIENT_LIGHT_BUFFER_SLOT, 1, &myAmbLightBuffer);
	}

	void ForwardRenderer::MapPointLightBuffer(const CU::DArray<PointLight*>& somePointLights)
	{
		for (uint i = 0; i < 8; ++i)
		{
			if (i == somePointLights.size())
				break;

			myPointLightBufferData.myPointLights[i].myColor = somePointLights[i]->GetColor();
			myPointLightBufferData.myPointLights[i].myIntensity = somePointLights[i]->GetIntensity();
			myPointLightBufferData.myPointLights[i].myRange = somePointLights[i]->GetRange();
			myPointLightBufferData.myPointLights[i].myPosition = { somePointLights[i]->GetPosition(), 1.0f };
		}

		myPointLightBufferData.myNumPointLights = somePointLights.size();
		if (somePointLights.size() > 8)
		{
			myPointLightBufferData.myNumPointLights = 8;
		}

		RenderUtils::MapBuffer<ForwardPointLightBuffer>(myPointLightBufferData, myPointLightBuffer);

		Main::GetContext()->PSSetConstantBuffers(POINT_LIGHT_BUFFER_SLOT, 1, &myPointLightBuffer);
	}

	void ForwardRenderer::MapSpotLightBuffer(const CU::DArray<SpotLight*>& someSpotLights)
	{
		for (uint i = 0; i < 8; ++i)
		{
			if (i == someSpotLights.size())
				break;

			mySpotLightBufferData.myLights[i].myColor = someSpotLights[i]->GetColor();
			mySpotLightBufferData.myLights[i].myDirection = { someSpotLights[i]->GetDirection(), 0.0f };
			mySpotLightBufferData.myLights[i].myPosition = { someSpotLights[i]->GetTransform().GetPosition(), 1.0f };
			mySpotLightBufferData.myLights[i].myIntensity = someSpotLights[i]->GetIntensity();
			mySpotLightBufferData.myLights[i].myRange = someSpotLights[i]->GetRange();
			mySpotLightBufferData.myLights[i].myInnerAngle = someSpotLights[i]->GetInnerAngle() * Deg2Rad;
			mySpotLightBufferData.myLights[i].myOuterAngle = someSpotLights[i]->GetOuterAngle() * Deg2Rad;
		}

		mySpotLightBufferData.myNumSpotLights = someSpotLights.size();
		if (someSpotLights.size() > 8)
		{
			mySpotLightBufferData.myNumSpotLights = 8;
		}

		RenderUtils::MapBuffer<ForwardSpotLightBuffer>(mySpotLightBufferData, mySpotLightBuffer);

		Main::GetContext()->PSSetConstantBuffers(SPOT_LIGHT_BUFFER_SLOT, 1, &mySpotLightBuffer);
	}
}
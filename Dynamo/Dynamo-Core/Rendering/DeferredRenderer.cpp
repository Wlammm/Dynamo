#include "pch.h"
#include "DeferredRenderer.h"
#include "Components/Camera.h"
#include "Components/MeshRenderer.h"
#include "Rendering/Mesh.h"
#include "Rendering/Material.h"

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

	void DeferredRenderer::GenerateGBuffer(const CU::DArray<MeshRenderer*>& someModels)
	{
		Camera* camera = Main::GetMainCamera();
		if (!camera)
			return;

		myFrameBufferData.myToCamera = camera->GetTransform()->GetMatrix().FastInverse();
		myFrameBufferData.myToProjection = camera->GetProjectionMatrix();
		myFrameBufferData.myCameraPosition = { camera->GetTransform()->GetPosition(), 1.0f };
		MapBuffer<FrameBuffer>(myFrameBufferData, myFrameBuffer);

		Main::GetContext()->VSSetConstantBuffers(FRAME_BUFFER_SLOT, 1, &myFrameBuffer);
		Main::GetContext()->PSSetConstantBuffers(FRAME_BUFFER_SLOT, 1, &myFrameBuffer);

		myGBufferShader->Bind();
		for (MeshRenderer* model : someModels.AsVector())
		{
			if (!model)
				return;

			myObjectBufferData.myToWorld = model->GetTransform().GetMatrix();
			myObjectBufferData.myUVScale = { 1.0f, 1.0f };
			myObjectBufferData.myColor = model->GetColor();
			MapBuffer<ObjectBuffer>(myObjectBufferData, myObjectBuffer);

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

		myFSVertexShader->Bind();

		myDirLightShader->Bind();
		for (auto& dirLight : someDirLights.AsVector())
		{
			myDirLightBufferData.myToLight = { dirLight->GetDirection() * -1.0f, 0.0f };
			myDirLightBufferData.myColor = dirLight->GetColor();
			myDirLightBufferData.myIntensity = dirLight->GetIntensity();
			MapBuffer<DirectionalLightBuffer>(myDirLightBufferData, myDirLightBuffer);

			Main::GetContext()->PSSetConstantBuffers(DIRECTIONAL_LIGHT_BUFFER_SLOT, 1, &myDirLightBuffer);
			Main::GetContext()->Draw(3, 0);
		}

		myAmbLightShader->Bind();
		for (auto& ambLight : someAmbLights.AsVector())
		{
			myAmbLightBufferData.myIntensity = ambLight->GetIntensity();
			MapBuffer<AmbientLightBuffer>(myAmbLightBufferData, myAmbLightBuffer);
			Main::GetContext()->PSSetConstantBuffers(AMBIENT_LIGHT_BUFFER_SLOT, 1, &myAmbLightBuffer);

			Main::GetContext()->PSSetShaderResources(CUBEMAP_TEXTURE_SLOT, 1, ambLight->GetCubeMapConst());

			Main::GetContext()->Draw(3, 0);
		}
	}

	void DeferredRenderer::CreateBuffers()
	{
		HRESULT result;

		D3D11_BUFFER_DESC bufferDesc = { 0 };
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		bufferDesc.ByteWidth = sizeof(FrameBuffer);
		result = Main::GetDevice()->CreateBuffer(&bufferDesc, nullptr, &myFrameBuffer);
		assert(SUCCEEDED(result));

		bufferDesc.ByteWidth = sizeof(ObjectBuffer);
		result = Main::GetDevice()->CreateBuffer(&bufferDesc, nullptr, &myObjectBuffer);
		assert(SUCCEEDED(result));

		bufferDesc.ByteWidth = sizeof(DirectionalLightBuffer);
		result = Main::GetDevice()->CreateBuffer(&bufferDesc, nullptr, &myDirLightBuffer);
		assert(SUCCEEDED(result));

		bufferDesc.ByteWidth = sizeof(AmbientLightBuffer);
		result = Main::GetDevice()->CreateBuffer(&bufferDesc, nullptr, &myAmbLightBuffer);
		assert(SUCCEEDED(result));
	}

	void DeferredRenderer::CreateShaders()
	{
		myFSVertexShader = ShaderFactory::GetShader("Shaders/FullscreenVS.cso", ShaderType::VertexShader);
		myMeshVertexShader = ShaderFactory::GetShader("Shaders/VertexShader.cso", ShaderType::VertexShader);

		myGBufferShader = ShaderFactory::GetShader("Shaders/GBuffer.cso", ShaderType::PixelShader);
		myDirLightShader = ShaderFactory::GetShader("Shaders/DirectionalLightShader.cso", ShaderType::PixelShader);
		myAmbLightShader = ShaderFactory::GetShader("Shaders/AmbientLightShader.cso", ShaderType::PixelShader);
	}
}

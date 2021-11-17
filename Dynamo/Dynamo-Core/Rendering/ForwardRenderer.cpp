#include "pch.h"
#include "ForwardRenderer.h"

#include "Components/Camera.h"
#include "Components/MeshRenderer.h"

#include "Rendering/Mesh.h"
#include "Rendering/Material.h"

namespace Dynamo
{
	ForwardRenderer::ForwardRenderer()
	{
		myDefaultPixelShader = ShaderFactory::GetShader("Shaders/ForwardPS.cso", ShaderType::PixelShader);
		myDefaultVertexShader = ShaderFactory::GetShader("Shaders/VertexShader.cso", ShaderType::VertexShader);
		myDefaultMaterial = MaterialFactory::GetDefaultMaterial();
		
		CreateBuffers();
	}

	void ForwardRenderer::Render(const CU::DArray<MeshRenderer*>& someModels)
	{
		HRESULT result;

		D3D11_MAPPED_SUBRESOURCE bufferData;
		
		Camera* camera = Main::GetMainCamera();
		if (!camera)
			return;
		
		myFrameBufferData.buffer.myToCamera = camera->GetTransform()->GetMatrix().FastInverse();
		myFrameBufferData.buffer.myToProjection = camera->GetProjectionMatrix();
		myFrameBufferData.buffer.myCameraPosition = { camera->GetTransform()->GetPosition(), 1.0f };
		MapBuffer<FrameBuffer>(myFrameBufferData, myFrameBuffer);
		
		Main::GetContext()->VSSetConstantBuffers(FRAME_BUFFER_SLOT, 1, &myFrameBuffer);
		Main::GetContext()->PSSetConstantBuffers(FRAME_BUFFER_SLOT, 1, &myFrameBuffer);
		
		for (MeshRenderer* model : someModels.AsVector())
		{
			for (auto& mesh : model->GetMeshes())
			{
				myObjectBufferData.buffer.myToWorld = model->GetTransform().GetMatrix();
				myObjectBufferData.buffer.myUVScale = { 1.0f, 1.0f };
				myObjectBufferData.buffer.myColor = model->GetColor();
				MapBuffer<ObjectBuffer>(myObjectBufferData, myObjectBuffer);
		
				Main::GetContext()->VSSetConstantBuffers(OBJECT_BUFFER_SLOT, 1, &myObjectBuffer);
				Main::GetContext()->PSSetConstantBuffers(OBJECT_BUFFER_SLOT, 1, &myObjectBuffer);
		
				Main::GetContext()->IASetPrimitiveTopology(mesh.myPrimitiveTopology);
				Main::GetContext()->IASetInputLayout(mesh.myInputLayout);
				Main::GetContext()->IASetVertexBuffers(0, 1, &mesh.myVertexBuffer, &mesh.myStride, &mesh.myOffset);
				Main::GetContext()->IASetIndexBuffer(mesh.myIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
		
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
	}
}
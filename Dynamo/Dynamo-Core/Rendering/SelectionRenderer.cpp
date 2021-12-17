#include "pch.h"
#include "SelectionRenderer.h"
#include "Utils/RenderUtils.h"

#include "Rendering/Material.h"
#include "Rendering/Mesh.h"
#include "Components/Camera.h"

namespace Dynamo
{
	SelectionRenderer::SelectionRenderer()
	{
		CreateBuffers();

		mySelectionPS = ShaderFactory::GetShader("Assets/Shaders/SelectionPS.cso", ShaderType::PixelShader);
		myVertexShader = ShaderFactory::GetShader("Assets/Shaders/VertexShader.cso", ShaderType::VertexShader);
	}

	void SelectionRenderer::Render(const CU::DArray<MeshCommand>& someMeshCommands)
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

		for (const MeshCommand& command : someMeshCommands.AsVector())
		{
			myObjectBufferData.myToWorld = command.myMatrix;
			myObjectBufferData.myUVScale = { 1.0f, 1.0f };
			myObjectBufferData.myColor = command.myColor;
			myObjectBufferData.myAdditiveColor = command.myAdditiveColor;
			myObjectBufferData.myIsAnimated = 0;
			if (command.myIsAnimated)
				myObjectBufferData.myIsAnimated = 1;

			RenderUtils::MapBuffer<ObjectBuffer>(myObjectBufferData, myObjectBuffer);
			Main::GetContext()->VSSetConstantBuffers(OBJECT_BUFFER_SLOT, 1, &myObjectBuffer);
			Main::GetContext()->PSSetConstantBuffers(OBJECT_BUFFER_SLOT, 1, &myObjectBuffer);

			mySelectionBufferData.myGameObjectID = (uint)command.myGameObjectID;
			RenderUtils::MapBuffer<SelectionBuffer>(mySelectionBufferData, mySelectionBuffer);
			Main::GetContext()->PSSetConstantBuffers(CUSTOM_BUFFER_SLOT, 1, &mySelectionBuffer);

			if (command.myIsAnimated)
			{
				myBoneBufferData.myBoneTransforms = command.myBoneTransforms;
			}
			RenderUtils::MapBuffer<BoneBuffer>(myBoneBufferData, myBoneBuffer);
			Main::GetContext()->VSSetConstantBuffers(BONE_BUFFER_SLOT, 1, &myBoneBuffer);

			Main::GetContext()->IASetPrimitiveTopology(command.myMesh->myPrimitiveTopology);
			Main::GetContext()->IASetInputLayout(command.myMesh->myInputLayout);
			Main::GetContext()->IASetVertexBuffers(0, 1, &command.myMesh->myVertexBuffer, &command.myMesh->myStride, &command.myMesh->myOffset);
			Main::GetContext()->IASetIndexBuffer(command.myMesh->myIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

			if (command.myMaterial)
				command.myMaterial->myVertexShader->Bind();
			else
				myVertexShader->Bind();

			mySelectionPS->Bind();
			Main::GetContext()->DrawIndexed(command.myMesh->myNumIndicies, 0, 0);
		}
	}

	void SelectionRenderer::CreateBuffers()
	{
		RenderUtils::CreateBuffer<FrameBuffer>(myFrameBuffer);
		RenderUtils::CreateBuffer<ObjectBuffer>(myObjectBuffer);
		RenderUtils::CreateBuffer<BoneBuffer>(myBoneBuffer);
		RenderUtils::CreateBuffer<SelectionBuffer>(mySelectionBuffer);
	}
}

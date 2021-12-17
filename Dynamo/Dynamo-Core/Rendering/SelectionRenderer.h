#pragma once

namespace Dynamo
{
	class SelectionRenderer
	{
		struct SelectionBuffer
		{
			uint myGameObjectID = 0;
			Vec3f padding;
		};

	public:
		SelectionRenderer();

		void Render(const CU::DArray<MeshCommand>& someMeshCommands);

	private:
		void CreateBuffers();

	private:
		Shader* mySelectionPS = nullptr;
		Shader* myVertexShader = nullptr;

		FrameBuffer myFrameBufferData;
		ID3D11Buffer* myFrameBuffer = nullptr;

		ObjectBuffer myObjectBufferData;
		ID3D11Buffer* myObjectBuffer = nullptr;

		BoneBuffer myBoneBufferData;
		ID3D11Buffer* myBoneBuffer = nullptr;

		SelectionBuffer mySelectionBufferData;
		ID3D11Buffer* mySelectionBuffer = nullptr;
	};
}
#pragma once

#include "pch.h"

namespace Dynamo
{
	struct Mesh
	{
		uint myNumVertices = 0;
		uint myNumIndicies = 0;
		uint myStride = 0;
		uint myOffset = 0;

		uint myMaterialIndex = 0;

		ID3D11Buffer* myVertexBuffer = nullptr;
		ID3D11Buffer* myIndexBuffer = nullptr;

		ID3D11InputLayout* myInputLayout = nullptr;
		D3D11_PRIMITIVE_TOPOLOGY myPrimitiveTopology = {};
	};
}
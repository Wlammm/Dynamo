#include "pch.h"
#include "DebugRenderer.h"
#include <fstream>

#include "Components/Camera.h"

void Dynamo::DebugRenderer::DrawLine2D(
	const CU::Vector2f& aFromNormalized,
	const CU::Vector2f& aToNormalized,
	const CU::Vector4f& aColor)
{
	AddLineToDraw2D(aFromNormalized, aToNormalized, aColor);
}

void Dynamo::DebugRenderer::DrawCircle2D(
	const CU::Vector2f& aPosNormalized,
	const float aRadiusRadians,
	const CU::Vector4f& aColor)
{
	// TODO: Precompute a unit circle, and only upscale it with radians?

	constexpr int TotalPoints = 30;

	CU::Vector2f firstPos;

	CU::Vector2f prevPos;

	const auto resolution = Main::GetWindowResolution();
	const float aspectRatio = static_cast<float>(resolution.x) / static_cast<float>(resolution.y);

	for (int i = 0; i < TotalPoints; ++i)
	{
		const float percentOfCircle = static_cast<float>(i) / static_cast<float>(TotalPoints);

		const float radiansOfCircle = CU::PI * 2.f * percentOfCircle;

		const float x = cos(radiansOfCircle) * aRadiusRadians * (1.f / aspectRatio);
		const float y = sin(radiansOfCircle) * aRadiusRadians;

		if (i == 0)
		{
			firstPos = aPosNormalized + CU::Vector2f(x, y);
			prevPos = aPosNormalized + CU::Vector2f(x, y);
			continue;
		}

		const auto toPos = aPosNormalized + CU::Vector2f(x, y);

		AddLineToDraw2D(prevPos, toPos, aColor);

		prevPos = toPos;
	}

	// Koppla (swedish word, cant english) together the last part with the first part of the circle
	AddLineToDraw2D(prevPos, firstPos, aColor);
}

void Dynamo::DebugRenderer::DrawRectangle2D(
	const CU::Vector2f& aPosNormalized,
	const CU::Vector2f& aHalfExtents,
	const CU::Vector4f& aColor)
{
	const auto& center = aPosNormalized;

	const CU::Vector2f cornerPositions[] =
	{
		// Top left
		CU::Vector2f(-aHalfExtents.x, -aHalfExtents.y),

		// Top right
		CU::Vector2f(aHalfExtents.x, -aHalfExtents.y),

		// Bottom right
		CU::Vector2f(aHalfExtents.x, aHalfExtents.y),

		// Bottom left
		CU::Vector2f(-aHalfExtents.x, aHalfExtents.y),
	};

	const int indices[] =
	{
		0, 1,
		1, 2,
		2, 3,
		3, 0
	};

	for (int i = 0; i < std::size(indices) - 1; ++i)
	{
		DrawLine2D(
			cornerPositions[indices[i]] + center,
			cornerPositions[indices[i + 1]] + center,
			aColor);
	}
}

void Dynamo::DebugRenderer::DrawLine3D(
	const CU::Vector3f& aFrom,
	const CU::Vector3f& aTo,
	const CU::Vector4f& aColor,
	const bool aDepthTested)
{
	AddLineToDraw3D(aFrom, aTo, aColor, aDepthTested);
}

void Dynamo::DebugRenderer::DrawCube3D(
	const CU::Vector3f& aCenterWorldPos,
	const CU::Vector3f& aRotationRadians,
	const CU::Vector3f& aSizeHalfExtents,
	const CU::Vector4f& aColor,
	const bool aDepthTested)
{
	const auto& center = aCenterWorldPos;
	const auto& e = aSizeHalfExtents;
	CU::Quaternion quaternion(aRotationRadians);

	const CU::Vector3f cornerPositions[] =
	{
		CU::Vector3f(-e.x, e.y, e.z) * quaternion, // 0
		CU::Vector3f(e.x, e.y, e.z) * quaternion, // 1
		CU::Vector3f(e.x, -e.y, e.z) * quaternion, // 2
		CU::Vector3f(-e.x, -e.y, e.z) * quaternion, // 3
		CU::Vector3f(-e.x, -e.y, -e.z) * quaternion, // 4
		CU::Vector3f(e.x, -e.y, -e.z) * quaternion, // 5
		CU::Vector3f(e.x, e.y, -e.z) * quaternion, // 6
		CU::Vector3f(-e.x, e.y, -e.z) * quaternion, // 7
	};

	constexpr int sides = 6;
	constexpr int linesPerFace = 8;

	const int indices[sides][linesPerFace] =
	{
		// NOTE: Indices marked with the as using line list
		// We could save some stuff by using line strip? 
		// But then other primitives would be harder?

		// NOTE: Must be in a order of connections, meaning 0 to 1, must 
		// have a line directly to each other on the cube
		// The reason being the way I do the for loop on the indices below

		// TODO: Can optimize because lines are inside of each other, but I dont care =)

		{ 0, 1, 1, 2, 2, 3, 3, 0 }, // +Z Face
		{ 4, 5, 5, 6, 6, 7, 7, 4 }, // -Z Face
		{ 0, 3, 3, 4, 4, 7, 7, 0 }, // -X Face
		{ 1, 2, 2, 5, 5, 6, 6, 1 }, // +X Face
		{ 0, 1, 1, 6, 6, 7, 7, 0 }, // +Y Face
		{ 2, 3, 3, 4, 4, 5, 5, 2 }, // -Y Face
	};

	// Draw each face side
	for (int i = 0; i < sides - 1; ++i)
	{
		// Draw each face lines
		for (int j = 0; j < linesPerFace - 1; ++j)
		{
			DrawLine3D(
				CU::Vector3f(
					cornerPositions[indices[i][j]].x,
					cornerPositions[indices[i][j]].y,
					cornerPositions[indices[i][j]].z) + center,
				CU::Vector3f(
					cornerPositions[indices[i][j + 1]].x,
					cornerPositions[indices[i][j + 1]].y,
					cornerPositions[indices[i][j + 1]].z) + center,
				aColor,
				aDepthTested);
		}
	}
}

void Dynamo::DebugRenderer::DrawSphere3D(
	const CU::Vector3f& aCenterWorldPos,
	const float aRadiusRadians,
	const CU::Vector4f& aColor,
	const bool aDepthTested)
{
	enum Axis
	{
		X = 0,
		Y,
		Z,

		Count
	};

	// For each axis
	for (int axis = X; axis < Axis::Count; axis++)
	{
		constexpr int TotalPoints = 30;

		CU::Vector3f firstPos;
		CU::Vector3f prevPos;

		// Draw a circle
		for (int i = 0; i < TotalPoints; ++i)
		{
			const float percentOfCircle = static_cast<float>(i) / static_cast<float>(TotalPoints);

			const float radiansOfCircle = CU::PI * 2.f * percentOfCircle;

			CU::Vector3f posInCircle;

			switch (axis)
			{
			case X:
				posInCircle = CU::Vector3f(
					cos(radiansOfCircle) * aRadiusRadians,
					sin(radiansOfCircle) * aRadiusRadians,
					0.f);
				break;
			case Y:
				posInCircle = CU::Vector3f(
					cos(radiansOfCircle) * aRadiusRadians,
					0.f,
					sin(radiansOfCircle) * aRadiusRadians);
				break;
			case Z:
				posInCircle = CU::Vector3f(
					0.f,
					cos(radiansOfCircle) * aRadiusRadians,
					sin(radiansOfCircle) * aRadiusRadians);
				break;
			default:
				assert(false && "whot");
				break;
			}

			if (i == 0)
			{
				firstPos = aCenterWorldPos + posInCircle;
				prevPos = aCenterWorldPos + posInCircle;
				continue;
			}

			const auto toPos = aCenterWorldPos + posInCircle;

			AddLineToDraw3D(prevPos, toPos, aColor, aDepthTested);

			prevPos = toPos;
		}

		// Koppla (swedish word, cant english) together the last part with the first part of the circle
		AddLineToDraw3D(prevPos, firstPos, aColor, aDepthTested);
	}
}

bool Dynamo::DebugRenderer::Init(DirectXFramework* aFramework)
{
	myFramework = aFramework;

	if (!CreatePixelShader())
	{
		return false;
	}

	if (!CreateVertexShader2DLines())
	{
		return false;
	}

	if (!CreateVertexBuffer())
	{
		return false;
	}

	if (!CreateInputLayout())
	{
		return false;
	}

	if (!CreateFrameConstantBuffer())
	{
		return false;
	}

	if (!CreateVertexShader3DLines())
	{
		return false;
	}

	return true;
}

void Dynamo::DebugRenderer::Render(const Camera& aCamera)
{
	Main::GetContext()->OMSetBlendState(nullptr, nullptr, 0xffffffff);

	Render2DLines();
	Render3DLines(aCamera, my3dLineVerticesToRender);

	my3dLineVerticesToRender.clear();
}

void Dynamo::DebugRenderer::RenderDepthTested(const Camera& aCamera)
{
	Render3DLines(aCamera, my3dLineVerticesToRenderDepthTested);

	my3dLineVerticesToRenderDepthTested.clear();
}

void Dynamo::DebugRenderer::ResetDrawCalls()
{
	myDrawCallCount = 0;
}

const int Dynamo::DebugRenderer::GetDrawCallCount() const
{
	return myDrawCallCount;
}

bool Dynamo::DebugRenderer::CreatePixelShader()
{
	std::ifstream pixelShaderFile("Assets/Shaders/DebugLinePixelShader.cso", std::ios::binary);

	if (!pixelShaderFile.is_open())
	{
		Console::ErrorLog("Failed to open DebugLinePixelShader.cso");
		return false;
	}

	std::vector<unsigned char> pixelShaderBuffer(std::istreambuf_iterator<char>(pixelShaderFile), { });
	HRESULT result = Main::GetDevice()->CreatePixelShader(pixelShaderBuffer.data(), pixelShaderBuffer.size(), nullptr, &myPixelShader);

	if (FAILED(result))
	{
		Console::ErrorLog("CreatePixelShader failed");
		return false;
	}

	pixelShaderFile.close();

	return true;
}

bool Dynamo::DebugRenderer::CreateVertexShader2DLines()
{
	std::ifstream vertexShaderFile("Assets/Shaders/DebugLineVertexShader.cso", std::ios::binary);

	if (!vertexShaderFile.is_open())
	{
		Console::ErrorLog("Failed to open DebugLineVertexShader.cso");
		return false;
	}

	myVertexShaderBuffer2DLines = std::vector<unsigned char>(std::istreambuf_iterator<char>(vertexShaderFile), { });

	HRESULT result = Main::GetDevice()->CreateVertexShader(
		myVertexShaderBuffer2DLines.data(),
		myVertexShaderBuffer2DLines.size(),
		nullptr,
		&myVertexShader2DLines);

	if (FAILED(result))
	{
		Console::ErrorLog("CreateVertexShader failed");
		return false;
	}

	vertexShaderFile.close();

	return true;
}

bool Dynamo::DebugRenderer::CreateVertexBuffer()
{
	D3D11_BUFFER_DESC bufferDesc = { };
	{
		bufferDesc.ByteWidth = sizeof(LineVertex) * ourLineVertexBatchSize;
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bufferDesc.MiscFlags = 0;
		bufferDesc.StructureByteStride = 0;
	}

	HRESULT result = Main::GetDevice()->CreateBuffer(&bufferDesc, nullptr, &myVertexBuffer);

	if (FAILED(result))
	{
		Console::ErrorLog("Failed to create debug vertex buffer");
		return false;
	}

	return true;
}

bool Dynamo::DebugRenderer::CreateInputLayout()
{
	D3D11_INPUT_ELEMENT_DESC positionDesc = { };
	{
		positionDesc.SemanticName = "POSITION";
		positionDesc.SemanticIndex = 0;
		positionDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT;
		positionDesc.InputSlot = 0;
		positionDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		positionDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		positionDesc.InstanceDataStepRate = 0;
	}

	D3D11_INPUT_ELEMENT_DESC colorDesc = { };
	{
		colorDesc.SemanticName = "COLOR";
		colorDesc.SemanticIndex = 0;
		colorDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT;
		colorDesc.InputSlot = 0;
		colorDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		colorDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		colorDesc.InstanceDataStepRate = 0;
	}

	std::array<D3D11_INPUT_ELEMENT_DESC, 2> layoutDesc = {
		positionDesc,
		colorDesc
	};

	HRESULT result = Main::GetDevice()->CreateInputLayout(
		layoutDesc.data(),
		static_cast<UINT>(layoutDesc.size()),
		myVertexShaderBuffer2DLines.data(),
		myVertexShaderBuffer2DLines.size(),
		&myInputLayout);

	if (FAILED(result))
	{
		Console::ErrorLog("CreateInputLayout failed");
		return false;
	}

	return true;
}

bool Dynamo::DebugRenderer::CreateFrameConstantBuffer()
{
	D3D11_BUFFER_DESC desc = { };
	{
		desc.ByteWidth = sizeof(FrameBufferData);
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}

	HRESULT result = Main::GetDevice()->CreateBuffer(&desc, nullptr, &myFrameBuffer3DLines);

	if (FAILED(result))
	{
		return false;
	}

	return true;
}

bool Dynamo::DebugRenderer::CreateVertexShader3DLines()
{
	std::ifstream vertexShaderFile("Assets/Shaders/DebugLine3DVertexShader.cso", std::ios::binary);

	if (!vertexShaderFile.is_open())
	{
		Console::ErrorLog("Failed to open DebugLine3DVertexShader.cso");
		return false;
	}

	myVertexShaderBuffer3DLines = std::vector<unsigned char>(std::istreambuf_iterator<char>(vertexShaderFile), { });

	HRESULT result = Main::GetDevice()->CreateVertexShader(
		myVertexShaderBuffer3DLines.data(),
		myVertexShaderBuffer3DLines.size(),
		nullptr,
		&myVertexShader3DLines);

	if (FAILED(result))
	{
		Console::ErrorLog("CreateVertexShader failed");
		return false;
	}

	vertexShaderFile.close();

	return true;
}

void Dynamo::DebugRenderer::Render2DLines()
{
	auto& context = *Main::GetContext();

	int iterateFromIndex = 0;

	while (iterateFromIndex < my2dLineVerticesToRender.size())
	{
		D3D11_MAPPED_SUBRESOURCE subData = { };

		HRESULT result = context.Map(myVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subData);

		if (FAILED(result))
		{
			Console::ErrorLog("Context.Map() failed");
			return;
		}

		// Ensure we never go above the batch size
		const auto linesToIterateCount = CU::Min(
			ourLineVertexBatchSize,
			static_cast<int>(my2dLineVerticesToRender.size()) - iterateFromIndex);

		LineVertex* vertices = reinterpret_cast<LineVertex*>(subData.pData);
		memcpy(vertices, &my2dLineVerticesToRender[iterateFromIndex], linesToIterateCount * sizeof(LineVertex));

		iterateFromIndex += linesToIterateCount;

		context.Unmap(myVertexBuffer, 0);

		// Draw the batch
		{
			context.IASetInputLayout(myInputLayout);

			context.VSSetShader(myVertexShader2DLines, nullptr, 0);
			context.PSSetShader(myPixelShader, nullptr, 0);

			context.IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

			// Set vertices
			{
				UINT strides = sizeof(LineVertex);
				UINT offsets = 0;
				context.IASetVertexBuffers(0, 1, &myVertexBuffer, &strides, &offsets);
			}

			context.Draw(linesToIterateCount, 0);
		}
	}

	my2dLineVerticesToRender.clear();
}

void Dynamo::DebugRenderer::Render3DLines(const Camera& aCamera, const std::vector<LineVertex>& aLineVerticesToRender)
{
	auto& context = *Main::GetContext();

	// Update the frame buffer
	{
		D3D11_MAPPED_SUBRESOURCE bufferData;
		ZeroMemory(&bufferData, sizeof(bufferData));

		HRESULT result = context.Map(myFrameBuffer3DLines, 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData);

		if (FAILED(result))
		{
			Console::ErrorLog("Context.Map() failed");
			return;
		}

		myFrameBufferData.myToCamera = CU::Matrix4x4f::GetFastInverse(aCamera.GetTransform()->GetMatrix());
		myFrameBufferData.myToProjection = aCamera.GetProjectionMatrix();

		memcpy(bufferData.pData, &myFrameBufferData, sizeof(myFrameBufferData));

		context.Unmap(myFrameBuffer3DLines, 0);

		// Vertex shader is the only that uses the frame buffer
		context.VSSetConstantBuffers(0, 1, &myFrameBuffer3DLines);
	}

	int iterateFromIndex = 0;

	while (iterateFromIndex < aLineVerticesToRender.size())
	{
		D3D11_MAPPED_SUBRESOURCE subData = { };

		HRESULT result = context.Map(myVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subData);

		if (FAILED(result))
		{
			Console::ErrorLog("Context.Map() failed");
			return;
		}

		// Ensure we never go above the batch size
		const auto linesToIterateCount = CU::Min(
			ourLineVertexBatchSize,
			static_cast<int>(aLineVerticesToRender.size()) - iterateFromIndex);

		LineVertex* vertices = reinterpret_cast<LineVertex*>(subData.pData);
		memcpy(vertices, &aLineVerticesToRender[iterateFromIndex], linesToIterateCount * sizeof(LineVertex));

		iterateFromIndex += linesToIterateCount;

		context.Unmap(myVertexBuffer, 0);

		// Draw the batch
		{
			context.IASetInputLayout(myInputLayout);

			context.VSSetShader(myVertexShader3DLines, nullptr, 0);
			context.PSSetShader(myPixelShader, nullptr, 0);

			context.IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

			// Set vertices
			{
				UINT strides = sizeof(LineVertex);
				UINT offsets = 0;
				context.IASetVertexBuffers(0, 1, &myVertexBuffer, &strides, &offsets);
			}

			context.Draw(linesToIterateCount, 0);
			myDrawCallCount++;
		}
	}

	// aLineVerticesToRender.clear();
}

void Dynamo::DebugRenderer::AddLineToDraw2D(
	const CU::Vector2f& aFromNormalized,
	const CU::Vector2f& aToNormalized,
	const CU::Vector4f& aColor)
{
	LineVertex v1;
	{
		v1.myPosition = CU::Vector4f(
			aFromNormalized.x,
			aFromNormalized.y,
			0.f,
			1.f);
		v1.myColor = aColor;
	}

	LineVertex v2;
	{
		v2.myPosition = CU::Vector4f(
			aToNormalized.x,
			aToNormalized.y,
			0.f,
			1.f);
		v2.myColor = aColor;
	}

	my2dLineVerticesToRender.push_back(v1);
	my2dLineVerticesToRender.push_back(v2);
}

void Dynamo::DebugRenderer::AddLineToDraw3D(
	const CU::Vector3f& aFrom,
	const CU::Vector3f& aTo,
	const CU::Vector4f& aColor,
	const bool aDepthTested)
{
	LineVertex v1;
	{
		v1.myPosition = CU::Vector4f(
			aFrom.x,
			aFrom.y,
			aFrom.z,
			1.f);
		v1.myColor = aColor;
	}

	LineVertex v2;
	{
		v2.myPosition = CU::Vector4f(
			aTo.x,
			aTo.y,
			aTo.z,
			1.f);
		v2.myColor = aColor;
	}

	if (aDepthTested)
	{
		my3dLineVerticesToRenderDepthTested.push_back(v1);
		my3dLineVerticesToRenderDepthTested.push_back(v2);
	}
	else
	{
		my3dLineVerticesToRender.push_back(v1);
		my3dLineVerticesToRender.push_back(v2);
	}
}
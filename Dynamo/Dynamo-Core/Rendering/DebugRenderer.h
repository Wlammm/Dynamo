#pragma once

struct ID3D11PixelShader;
struct ID3D11Buffer;
struct ID3D11VertexShader;
struct ID3D11InputLayout;

// Full credits to Filip Tynell!
namespace Dynamo
{
	class DirectXFramework;
	class Camera;
	class WindowHandler;

	class DebugRenderer
	{
	private:
		// TODO: A more organized solution is to make a shader class that is 
		// then inherited from to hold input layout, buffers, and more..?
		// DebugLine3DVertexShader constant buffer structs
		// Register b0
		struct FrameBufferData
		{
			CU::Matrix4x4f myToCamera;
			CU::Matrix4x4f myToProjection;
		} myFrameBufferData = { };

		// Register b1
		struct ObjectBufferData
		{
			CU::Matrix4x4f myToWorld;
		} myObjectBufferData = { };

		struct LineVertex
		{
			LineVertex() = default;

			LineVertex(CU::Vector4f aPosition, CU::Vector4f aColor)
				: myPosition(aPosition),
				myColor(aColor)
			{
			}

			CU::Vector4f myPosition;
			CU::Vector4f myColor;
		};

	public:
		// In normalized space 
		// 0,0 top left corner
		// 1,1 bottom right corner
		void DrawLine2D(
			const CU::Vector2f& aFromNormalized,
			const CU::Vector2f& aToNormalized,
			const CU::Vector4f& aColor = ourWhiteColor);

		void DrawCircle2D(
			const CU::Vector2f& aPosNormalized,
			const float aRadiusRadians,
			const CU::Vector4f& aColor = ourWhiteColor);

		void DrawRectangle2D(
			const CU::Vector2f& aPosNormalized,
			const CU::Vector2f& aHalfExtents,
			const CU::Vector4f& aColor = ourWhiteColor);

		void DrawLine3D(
			const CU::Vector3f& aFrom,
			const CU::Vector3f& aTo,
			const CU::Vector4f& aColor = ourWhiteColor,
			const bool aDepthTested = true);

		void DrawCube3D(
			const CU::Vector3f& aCenterWorldPos,
			const CU::Vector3f& aRotationRadians,
			const CU::Vector3f& aSizeHalfExtents = ourOneVector,
			const CU::Vector4f& aColor = ourWhiteColor,
			const bool aDepthTested = true);

		void DrawSphere3D(
			const CU::Vector3f& aCenterWorldPos,
			const float aRadiusRadians = 1.f,
			const CU::Vector4f& aColor = ourWhiteColor,
			const bool aDepthTested = true);

		bool Init(DirectXFramework* aFramework);
		void Render(const Camera& aCamera);
		void RenderDepthTested(const Camera& aCamera);

		void ResetDrawCalls();
		const int GetDrawCallCount() const;

	private:
		// 2D & 3D
		bool CreatePixelShader();
		bool CreateInputLayout();

		bool CreateVertexBuffer();

		// 2D
		bool CreateVertexShader2DLines();

		// 3D
		bool CreateFrameConstantBuffer();
		bool CreateVertexShader3DLines();

		void Render2DLines();
		void Render3DLines(const Camera& aCamera, const std::vector<LineVertex>& aLineVerticesToRender);

		void AddLineToDraw2D(
			const CU::Vector2f& aFromNormalized,
			const CU::Vector2f& aToNormalized,
			const CU::Vector4f& aColor);

		void AddLineToDraw3D(
			const CU::Vector3f& aFrom,
			const CU::Vector3f& aTo,
			const CU::Vector4f& aColor,
			const bool aDepthTested);

	private:
		DirectXFramework* myFramework = nullptr;

		inline static const int ourLineVertexBatchSize = 8192;

		// Both 2D & 3D
		ID3D11PixelShader* myPixelShader = nullptr;
		ID3D11Buffer* myVertexBuffer = nullptr;

		// 2D
		std::vector<LineVertex> my2dLineVerticesToRender;

		ID3D11VertexShader* myVertexShader2DLines = nullptr;
		std::vector<unsigned char> myVertexShaderBuffer2DLines;

		ID3D11InputLayout* myInputLayout = nullptr;

		// 3D
		std::vector<LineVertex> my3dLineVerticesToRender;
		std::vector<LineVertex> my3dLineVerticesToRenderDepthTested;

		ID3D11VertexShader* myVertexShader3DLines = nullptr;
		std::vector<unsigned char> myVertexShaderBuffer3DLines;

		ID3D11Buffer* myFrameBuffer3DLines = nullptr;

		int myDrawCallCount = 0;

		inline const static CU::Vector4f ourWhiteColor = CU::Vector4f(1.f, 1.f, 1.f, 1.f);
		inline const static CU::Vector3f ourOneVector = CU::Vector3f(1.f, 1.f, 1.f);
	};
}
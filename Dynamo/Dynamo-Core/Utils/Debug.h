#pragma once

namespace Dynamo
{
	class Debug
	{
#pragma region Structs
	private:
		struct DrawLine2DData
		{
			Vec2f myFrom;
			Vec2f myTo;
			Color myColor;
		};

		struct DrawLine3DData
		{
			Vec3f myFrom;
			Vec3f myTo;
			Color myColor;
		};

		struct DrawRay3DData
		{
			Vec3f myOrigin;
			Vec3f myDirection;
			float myDistance;
			Color myColor;
		};

		struct DrawRectangle2DData
		{
			Vec2f myStartPos;
			Vec2f myEndPos;
			Color myColor;
		};

		struct DrawRectangle3DData
		{
			Vec3f myMinPos;
			Vec3f myMaxPos;
			Color myColor;
		};

		struct DrawCircle2DData
		{
			Vec2f myStartPos;
			float myRadius;
			Color myColor;
		};

		struct DrawCircle3DData
		{
			Vec3f myCenterPos;
			float myRadius;
			Color myColor;
		};
#pragma endregion
	public:
		static void ImGui(const std::string& aName, const std::function<void(void)>& aFunction);
		static void Update();

		static void DrawLine2D(const Vec2f& aFromNormalized, const Vec2f& aToNormalized, const Color& aColor = { 1, 1, 1, 1 }, const float aDuration = 0);
		static void DrawLine3D(const Vec3f& aFromPosition, const Vec3f& aToPosition, const Color& aColor = { 1, 1, 1, 1 }, const float aDuration = 0);

		static void DrawRay3D(const Vec3f& anOrigin, const Vec3f& aDirection, const float aDistance = 10, const Color& aColor = { 1, 1, 1, 1 }, const float aDuration = 0);

		static void DrawRectangle2D(const Vec2f& aStartPos, const Vec2f& aEndPos, const Color& aColor = { 1, 1, 1, 1 }, const float aDuration = 0);
		static void DrawRectangle3D(const Vec3f& aMinPos, const Vec3f& aMaxPos, const Color& aCoor = { 1, 1, 1, 1 }, const float aDuration = 0);

		static void DrawCircle2D(const Vec2f& aPosNormalized, const float aRadius, const Color& aColor = { 1, 1, 1, 1 }, const float aDuration = 0);
		static void DrawSphere3D(const Vec3f& aCenterWorldPos, const float aRadius = 20.0f, const Color& aColor = { 1, 1, 1, 1 }, const float aDuration = 0);

	private:
		static void AddDebugDrawsToRender();

		static CU::DArray<std::pair<float, DrawLine2DData>> myDraw2DLines;
		static CU::DArray<std::pair<float, DrawLine3DData>> myDraw3DLines;
		static CU::DArray<std::pair<float, DrawRay3DData>> myDraw3DRays;
		static CU::DArray<std::pair<float, DrawRectangle2DData>> myDraw2DRectangles;
		static CU::DArray<std::pair<float, DrawRectangle3DData>> myDraw3DRectangles;
		static CU::DArray<std::pair<float, DrawCircle2DData>> myDraw2DCircles;
		static CU::DArray<std::pair<float, DrawCircle3DData>> myDraw3DCircles;

		static std::vector<std::pair<std::string, std::function<void(void)>>> myImGuiDraws;
		static bool isOpen;

	
	};
}
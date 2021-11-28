#include "pch.h"
#include "Debug.h"

namespace Dynamo
{
	bool Debug::isOpen = false;
	std::vector<std::pair<std::string, std::function<void(void)>>> Debug::myImGuiDraws;

	CU::DArray<std::pair<float, Debug::DrawLine2DData>> Debug::myDraw2DLines;
	CU::DArray<std::pair<float, Debug::DrawLine3DData>> Debug::myDraw3DLines;
	CU::DArray<std::pair<float, Debug::DrawRay3DData>> Debug::myDraw3DRays;
	CU::DArray<std::pair<float, Debug::DrawRectangle2DData>> Debug::myDraw2DRectangles;
	CU::DArray<std::pair<float, Debug::DrawRectangle3DData>> Debug::myDraw3DRectangles;
	CU::DArray<std::pair<float, Debug::DrawCircle2DData>> Debug::myDraw2DCircles;
	CU::DArray<std::pair<float, Debug::DrawCircle3DData>> Debug::myDraw3DCircles;

	void Dynamo::Debug::ImGui(const std::string& aName, const std::function<void(void)>& aFunction)
	{
		if (isOpen)
		{
			myImGuiDraws.push_back({ aName, aFunction });
		}
	}

	void Debug::Update()
	{
		if (isOpen)
		{
			ImGui::SetNextWindowSize(ImVec2(600, 400), ImGuiCond_FirstUseEver);

			ImGui::Begin("Debug##DebugManager");
			{
				ImGui::Text("F1 to toggle");
				ImGui::Separator();
				int index = 0;
				for (auto& entry : myImGuiDraws)
				{
					const auto& label = entry.first;
					const auto& callback = entry.second;

					ImGui::PushID(index);
					if (ImGui::CollapsingHeader(label.c_str(), ImGuiTreeNodeFlags_None))
					{
						callback();
					}
					ImGui::PopID();

					index++;
				}
			}
			ImGui::End();

			myImGuiDraws.clear();
		}

		if (Input::IsKeyDown(KeyCode::F1))
		{
			isOpen = !isOpen;
		}

		AddDebugDrawsToRender();
	}

	void Debug::DrawLine2D(const Vec2f& aFromNormalized, const Vec2f& aToNormalized, const Color& aColor, const float aDuration)
	{
		Main::GetRenderManager().GetDebugRenderer().DrawLine2D(aFromNormalized, aToNormalized, aColor);

		if (aDuration > 0)
		{
			DrawLine2DData data;
			data.myFrom = aFromNormalized;
			data.myTo = aToNormalized;
			data.myColor = aColor;
			myDraw2DLines.Add({ aDuration, data });
		}
	}

	void Debug::DrawLine3D(const Vec3f& aFromPosition, const Vec3f& aToPosition, const Color& aColor, const float aDuration)
	{
		Main::GetRenderManager().GetDebugRenderer().DrawLine3D(aFromPosition, aToPosition, aColor);

		if (aDuration > 0)
		{
			DrawLine3DData data;
			data.myFrom = aFromPosition;
			data.myTo = aToPosition;
			data.myColor = aColor;
			myDraw3DLines.Add({ aDuration, data });
		}
	}

	void Debug::DrawRay3D(const Vec3f& anOrigin, const Vec3f& aDirection, const float aDistance, const Color& aColor, const float aDuration)
	{
		Main::GetRenderManager().GetDebugRenderer().DrawLine3D(anOrigin, anOrigin + aDirection.GetNormalized() * aDistance, aColor);

		if (aDuration > 0)
		{
			DrawRay3DData data;
			data.myOrigin = anOrigin;
			data.myDirection = aDirection;
			data.myDistance = aDistance;
			data.myColor = aColor;
			myDraw3DRays.Add({ aDuration, data });
		}
	}

	void Debug::DrawRectangle2D(const Vec2f& aStartPos, const Vec2f& aEndPos, const Color& aColor, const float aDuration)
	{
		Vec2f size = aEndPos - aStartPos;
		Vec2f midPoint = aEndPos - (size * 0.5f);
		Main::GetRenderManager().GetDebugRenderer().DrawRectangle2D(midPoint, size * 0.5f);

		if (aDuration > 0)
		{
			DrawRectangle2DData data;
			data.myStartPos = aStartPos;
			data.myEndPos = aEndPos;
			data.myColor = aColor;
			myDraw2DRectangles.Add({ aDuration, data });
		}
	}

	void Debug::DrawRectangle3D(const Vec3f& aMinPos, const Vec3f& aMaxPos, const Color& aColor, const float aDuration)
	{
		Vec3f size = aMaxPos - aMinPos;
		Vec3f centerPos = aMaxPos - (size * .5f);
		Main::GetRenderManager().GetDebugRenderer().DrawCube3D(centerPos, { 0, 0, 0 }, size * 0.5f, aColor);

		if (aDuration > 0)
		{
			DrawRectangle3DData data;
			data.myMinPos = aMinPos;
			data.myMaxPos = aMaxPos;
			data.myColor = aColor;
			myDraw3DRectangles.Add({ aDuration, data });
		}
	}

	void Debug::DrawCircle2D(const Vec2f& aPosNormalized, const float aRadius, const Color& aColor, const float aDuration)
	{
		Main::GetRenderManager().GetDebugRenderer().DrawCircle2D(aPosNormalized, aRadius, aColor);

		if (aDuration > 0)
		{
			DrawCircle2DData data;
			data.myStartPos = aPosNormalized;
			data.myRadius = aRadius;
			data.myColor = aColor;
			myDraw2DCircles.Add({ aDuration, data });
		}
	}

	void Debug::DrawSphere3D(const Vec3f& aCenterWorldPos, const float aRadius, const Color& aColor, const float aDuration)
	{
		Main::GetRenderManager().GetDebugRenderer().DrawSphere3D(aCenterWorldPos, aRadius, aColor);

		if (aDuration > 0)
		{
			DrawCircle3DData data;
			data.myCenterPos = aCenterWorldPos;
			data.myRadius = aRadius;
			data.myColor = aColor;
			myDraw3DCircles.Add({ aDuration, data });
		}
	}

	void Debug::AddDebugDrawsToRender()
	{
		for (int i = myDraw2DLines.sizeI() - 1; i >= 0; --i)
		{
			myDraw2DLines[i].first -= Time::GetUnscaledDeltaTime();
			auto data = myDraw2DLines[i].second;
			
			Debug::DrawLine2D(data.myFrom, data.myTo, data.myColor);

			if (myDraw2DLines[i].first <= 0)
			{
				myDraw2DLines.RemoveIndex(i);
			}
		}

		for (int i = myDraw3DLines.sizeI() - 1; i >= 0; --i)
		{
			myDraw3DLines[i].first -= Time::GetUnscaledDeltaTime();
			auto data = myDraw3DLines[i].second;

			Debug::DrawLine3D(data.myFrom, data.myTo, data.myColor);

			if (myDraw3DLines[i].first <= 0)
			{
				myDraw3DLines.RemoveIndex(i);
			}
		}

		for (int i = myDraw3DRays.sizeI() - 1; i >= 0; --i)
		{
			myDraw3DRays[i].first -= Time::GetUnscaledDeltaTime();
			auto data = myDraw3DRays[i].second;

			Debug::DrawRay3D(data.myOrigin, data.myDirection, data.myDistance, data.myColor);

			if (myDraw3DRays[i].first <= 0)
			{
				myDraw3DRays.RemoveIndex(i);
			}
		}

		for (int i = myDraw2DRectangles.sizeI() - 1; i >= 0; --i)
		{
			myDraw2DRectangles[i].first -= Time::GetUnscaledDeltaTime();
			auto data = myDraw2DRectangles[i].second;

			Debug::DrawRectangle2D(data.myStartPos, data.myEndPos, data.myColor);

			if (myDraw2DRectangles[i].first <= 0)
			{
				myDraw2DRectangles.RemoveIndex(i);
			}
		}

		for (int i = myDraw3DRectangles.sizeI() - 1; i >= 0; --i)
		{
			myDraw3DRectangles[i].first -= Time::GetUnscaledDeltaTime();
			auto data = myDraw3DRectangles[i].second;

			Debug::DrawRectangle3D(data.myMinPos, data.myMaxPos, data.myColor);

			if (myDraw3DRectangles[i].first <= 0)
			{
				myDraw3DRectangles.RemoveIndex(i);
			}
		}

		for (int i = myDraw2DCircles.sizeI() - 1; i >= 0; --i)
		{
			myDraw2DCircles[i].first -= Time::GetUnscaledDeltaTime();
			auto data = myDraw2DCircles[i].second;

			Debug::DrawCircle2D(data.myStartPos, data.myRadius, data.myColor);

			if (myDraw2DCircles[i].first <= 0)
			{
				myDraw2DCircles.RemoveIndex(i);
			}
		}

		for (int i = myDraw3DCircles.sizeI() - 1; i >= 0; --i)
		{
			myDraw3DCircles[i].first -= Time::GetUnscaledDeltaTime();
			auto data = myDraw3DCircles[i].second;

			Debug::DrawSphere3D(data.myCenterPos, data.myRadius, data.myColor);

			if (myDraw3DCircles[i].first <= 0)
			{
				myDraw3DCircles.RemoveIndex(i);
			}
		}
	}
}

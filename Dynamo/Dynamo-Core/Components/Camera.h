#pragma once

namespace Dynamo
{
	class Camera : public Component
	{
	public:
		Camera();

		void Init(const float aFoV, const Vec2f& aResolution, const float aNearPlane = 1.0f, const float aFarPlane = 10000.0f);
		void Init(const Vec2f& aResolution, const float aNearPlane = 1.0f, const float aFarPlane = 10000.0f);

		void SetFoV(const float aFoV);

	private:
		Transform* myTransform;

		Mat4f myProjection;
		Vec2f myResolution;
		float myNearPlane = 1.0f;
		float myFarPlane = 10000.0f;
	};
}
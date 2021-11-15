#pragma once

namespace Dynamo
{
	class Transform;

	class Camera : public Component
	{
	public:
		Camera();

		void OnCreate() override;
		void OnDestroy() override;

		void Init(const float aFoV, const Vec2f& aResolution, const float aNearPlane = 1.0f, const float aFarPlane = 10000.0f);
		void Init(const Vec2f& aResolution, const float aNearPlane = 1.0f, const float aFarPlane = 10000.0f);

		void SetFoV(const float aFoV);

		Transform* GetTransform();

		bool IsValid() const;

	private:
		bool myIsValid = false;

		Transform* myTransform;

		Mat4f myProjection;
		Vec2f myResolution;
		float myNearPlane = 1.0f;
		float myFarPlane = 10000.0f;
		float myFoV = 90.0f;
	};
}
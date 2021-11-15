#include "pch.h"
#include "Camera.h"

namespace Dynamo
{
	Camera::Camera()
	{
		myTransform = GetComponent<Transform>();
	}

	void Camera::Init(const float aFoV, const Vec2f& aResolution, const float aNearPlane, const float aFarPlane)
	{
		myNearPlane = aNearPlane;
		myFarPlane = aFarPlane;

		myResolution = aResolution;

		const float hFoVRad = aFoV * (PI / 180.0f);
		const float vFoVRad = 2.f * std::atan(std::tan(hFoVRad / 2.f) * (aResolution.y / aResolution.x));

		const float scaleX = 1.f / std::tan(hFoVRad * .5f);
		const float scaleY = 1.f / std::tan(vFoVRad * .5f);

		const float Q = myFarPlane / (myFarPlane - myNearPlane);

		myProjection(1, 1) = scaleX;
		myProjection(2, 2) = scaleY;
		myProjection(3, 3) = Q;
		myProjection(3, 4) = 1.f / Q;
		myProjection(4, 3) = -Q * myNearPlane;
		myProjection(4, 4) = 0.0f;
	}
	
	void Camera::Init(const Vec2f& aResolution, const float aNearPlane, const float aFarPlane)
	{
		myNearPlane = aNearPlane;
		myFarPlane = aFarPlane;

		myResolution = aResolution;

		myProjection(1, 1) = 2.f / aResolution.x;
		myProjection(2, 2) = 2.f / aResolution.y;
		myProjection(3, 3) = 1.f / (myFarPlane - myNearPlane);
		myProjection(4, 3) = myNearPlane / (myNearPlane - myFarPlane);
		myProjection(4, 4) = 1.f;
	}

	void Camera::SetFoV(const float aFoV)
	{
		Init(aFoV, myResolution);
	}
}

#include "pch.h"
#include "SpotLight.h"

namespace Dynamo
{
	void SpotLight::OnCreate()
	{
		myTransform = GetComponent<Transform>();

		Main::GetRenderManager().AddSpotLight(this);
	}

	void SpotLight::OnDisable()
	{
		Main::GetRenderManager().RemoveSpotLight(this);
	}

	void SpotLight::SetColor(const Color& aColor)
	{
		myColor = aColor;
	}

	const Color& SpotLight::GetColor() const
	{
		return myColor;
	}

	void SpotLight::SetIntensity(const float aIntensity)
	{
		myIntensity = aIntensity;
	}

	float SpotLight::GetIntensity() const
	{
		return myIntensity;
	}

	void SpotLight::SetRange(const float aRange)
	{
		myRange = aRange;
	}

	float SpotLight::GetRange() const
	{
		return myRange;
	}

	void SpotLight::SetInnerAngle(const float aInnerAngle)
	{
		myInnerAngle = aInnerAngle;
	}

	float SpotLight::GetInnerAngle() const
	{
		return myInnerAngle;
	}

	void SpotLight::SetOuterAngle(const float aOuterAngle)
	{
		myOuterAngle = aOuterAngle;
	}

	float SpotLight::GetOuterAngle() const
	{
		return myOuterAngle;
	}

	Vec3f SpotLight::GetDirection() const
	{
		return myTransform->GetForward();
	}

	void SpotLight::OnEnable()
	{
		Main::GetRenderManager().AddSpotLight(this);
	}

	void SpotLight::OnDestroy()
	{
		Main::GetRenderManager().RemoveSpotLight(this);
	}
}

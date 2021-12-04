#include "pch.h"
#include "PointLight.h"

namespace Dynamo
{
	PointLight::PointLight()
		: Component("Lights")
	{
	}

	void PointLight::ExposeValues()
	{
		ImGui::Text("Intensity");
		ImGui::SameLine(0.0f, 40);
		ImGui::DragFloat("##pointlightIntensity", &myIntensity, 0.1f, 0, 1000000);

		ImGui::Text("Range");
		ImGui::SameLine(0.0f, 50);
		ImGui::DragFloat("##pointlightRange", &myRange, 1, 0, 1000000);

		ImGui::Text("Color");
		ImGui::SameLine(0.0f, 68);
		ImGui::ColorEdit3("##pointlightColor", &myColor.r);
	}

	void PointLight::OnCreate()
	{
		myTransform = GetComponent<Transform>();

		Main::GetRenderManager().AddPointLight(this);
	}

	void PointLight::OnDestroy()
	{
		Main::GetRenderManager().RemovePointLight(this);
	}

	void PointLight::OnEnable()
	{
		Main::GetRenderManager().AddPointLight(this);
	}

	void PointLight::OnDisable()
	{
		Main::GetRenderManager().RemovePointLight(this);
	}

	void PointLight::SetColor(const Color& aColor)
	{
		myColor = aColor;
	}

	const Color& PointLight::GetColor() const
	{
		return myColor;
	}
	
	void PointLight::SetIntensity(const float aIntensity)
	{
		myIntensity = aIntensity;
	}
	
	float PointLight::GetIntensity() const
	{
		return myIntensity;
	}
	
	void PointLight::SetRange(const float aRange)
	{
		myRange = aRange;
	}
	
	float PointLight::GetRange() const
	{
		return myRange;
	}
	Vec3f PointLight::GetPosition() const
	{
		return myTransform->GetPosition();
	}
}

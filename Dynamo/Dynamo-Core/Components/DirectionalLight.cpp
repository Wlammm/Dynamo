#include "pch.h"
#include "DirectionalLight.h"

namespace Dynamo
{
	DirectionalLight::DirectionalLight()
		: Component("Lights")
	{
	}
	void DirectionalLight::ExposeValues()
	{
		ImGui::Text("Intensity");
		ImGui::SameLine(0.0f, 40);
		ImGui::DragFloat("##dirlightIntensity", &myIntensity, 0.1f, 0, 1000000);

		ImGui::Text("Color");
		ImGui::SameLine(0.0f, 68);
		ImGui::ColorEdit3("##dirlightColor", &myColor.r);
	}

	void DirectionalLight::OnCreate()
	{
		myTransform = GetComponent<Transform>();

		Main::GetRenderManager().AddDirectionalLight(this);
	}

	void DirectionalLight::OnDestroy()
	{
		Main::GetRenderManager().RemoveDirectionalLight(this);
	}

	void DirectionalLight::OnEnable()
	{
		Main::GetRenderManager().AddDirectionalLight(this);
	}

	void DirectionalLight::OnDisable()
	{
		Main::GetRenderManager().RemoveDirectionalLight(this);
	}

	void DirectionalLight::SetIntensity(const float aIntensity)
	{
		myIntensity = aIntensity;
	}

	float DirectionalLight::GetIntensity() const
	{
		return myIntensity;
	}

	void DirectionalLight::SetColor(const Color& aColor)
	{
		myColor = aColor;
	}

	const Color& DirectionalLight::GetColor() const
	{
		return myColor;
	}

	Vec3f DirectionalLight::GetDirection() const
	{
		return myTransform->GetForward();
	}

	Vec3f DirectionalLight::GetToLight() const
	{
		return myTransform->GetForward() * -1.0f;
	}
}
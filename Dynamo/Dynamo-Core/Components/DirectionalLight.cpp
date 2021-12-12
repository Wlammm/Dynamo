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

	nlohmann::json DirectionalLight::Save()
	{
		nlohmann::json json;

		json["intensity"] = myIntensity;
		json["color"]["r"] = myColor.r;
		json["color"]["g"] = myColor.g;
		json["color"]["b"] = myColor.b;
		json["color"]["a"] = myColor.a;

		return json;
	}

	void DirectionalLight::Load(nlohmann::json& aJson)
	{
		myIntensity = aJson["intensity"];
		myColor.r = aJson["color"]["r"];
		myColor.g = aJson["color"]["g"];
		myColor.b = aJson["color"]["b"];
		myColor.a = aJson["color"]["a"];
	}

	void DirectionalLight::Update()
	{
		DirectionalLightCommand command;
		command.myColor = myColor;
		command.myIntensity = myIntensity;
		command.myDirection = GetDirection();
		Main::GetRenderManager().AddDirectionalLight(command);
	}

	void DirectionalLight::EditorUpdate()
	{
		Update();
	}

	void DirectionalLight::OnCreate()
	{
		myTransform = GetComponent<Transform>();
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
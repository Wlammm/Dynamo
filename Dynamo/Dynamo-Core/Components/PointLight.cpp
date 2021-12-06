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

	nlohmann::json PointLight::Save()
	{
		nlohmann::json json;
		json["intensity"] = myIntensity;
		json["range"] = myRange;
		json["color"]["r"] = myColor.r;
		json["color"]["g"] = myColor.g;
		json["color"]["b"] = myColor.b;
		json["color"]["a"] = myColor.a;

		return json;
	}

	void PointLight::Load(nlohmann::json& aJson)
	{
		myIntensity = aJson["intensity"];
		myRange = aJson["range"];
		myColor.r = aJson["color"]["r"];
		myColor.g = aJson["color"]["g"];
		myColor.b = aJson["color"]["b"];
		myColor.a = aJson["color"]["a"];
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

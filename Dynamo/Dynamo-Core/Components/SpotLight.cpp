#include "pch.h"
#include "SpotLight.h"

namespace Dynamo
{
	SpotLight::SpotLight()
		: Component("Lights")
	{
	}

	void SpotLight::ExposeValues()
	{
		ImGui::Text("Intensity");
		ImGui::SameLine(0.0f, 40);
		ImGui::DragFloat("##spotlightIntensity", &myIntensity, 0.1f, 0, 1000000);

		ImGui::Text("Range");
		ImGui::SameLine(0.0f, 50);
		ImGui::DragFloat("##spotlightRange", &myRange, 1, 0, 1000000);

		ImGui::Text("Inner Angle");
		ImGui::SameLine(0.0f, 30);
		ImGui::DragFloat("##spotlightInnerAngle", &myInnerAngle, 1, 0, myOuterAngle);

		ImGui::Text("Outer Angle");
		ImGui::SameLine(0.0f, 30);
		ImGui::DragFloat("##spotlightOuterAngle", &myOuterAngle, 1, myInnerAngle, 360);

		ImGui::Text("Color");
		ImGui::SameLine(0.0f, 68);
		ImGui::ColorEdit3("##spotlightColor", &myColor.r);
	}

	nlohmann::json SpotLight::Save()
	{
		nlohmann::json json;

		json["intensity"] = myIntensity;
		json["color"]["r"] = myColor.r;
		json["color"]["g"] = myColor.g;
		json["color"]["b"] = myColor.b;
		json["color"]["a"] = myColor.a;
		json["innerAngle"] = myInnerAngle;
		json["outerAngle"] = myOuterAngle;

		return json;
	}

	void SpotLight::Load(nlohmann::json& aJson)
	{
		myIntensity = aJson["intensity"];
		myColor.r = aJson["color"]["r"];
		myColor.g = aJson["color"]["g"];
		myColor.b = aJson["color"]["b"];
		myColor.a = aJson["color"]["a"];
		myInnerAngle = aJson["innerAngle"];
		myOuterAngle = aJson["outerAngle"];
	}

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

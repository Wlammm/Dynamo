#include "pch.h"
#include "AmbientLight.h"

namespace Dynamo
{
	AmbientLight::AmbientLight()
		: Component("Lights")
	{
	}

	void AmbientLight::ExposeValues()
	{
		ImGui::Text("Intensity");
		ImGui::SameLine(0.0f, 40);
		ImGui::DragFloat("##amblightIntensity", &myIntensity, 0.1f, 0, 1000000);
	}

	nlohmann::json AmbientLight::Save()
	{
		nlohmann::json json;

		json["intensity"] = myIntensity;

		return json;
	}

	void AmbientLight::Load(nlohmann::json& aJson)
	{
		myIntensity = aJson["intensity"];
	}

	void AmbientLight::Update()
	{
		AmbientLightCommand command;
		command.myCubeMap = &myCubeMap;
		command.myIntensity = myIntensity;
		Main::GetRenderManager().AddAmbientLight(command);
	}

	void AmbientLight::EditorUpdate()
	{
		Update();
	}

	void AmbientLight::OnCreate()
	{
		SetCubeMap("Assets/Textures/Default_Cubemap.dds");
	}

	void AmbientLight::SetIntensity(const float aIntensity)
	{
		myIntensity = aIntensity;
	}

	float AmbientLight::GetIntensity() const
	{
		return myIntensity;
	}

	void AmbientLight::SetCubeMap(const std::string& aCubeMapPath)
	{
		myCubeMap = ResourceFactory::GetSRV(aCubeMapPath);
	}

	DXSRV* AmbientLight::GetCubeMap()
	{
		return myCubeMap;
	}

	DXSRV* const* AmbientLight::GetCubeMapConst()
	{
		return &myCubeMap;
	}
}
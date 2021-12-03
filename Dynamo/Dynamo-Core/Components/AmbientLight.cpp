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

	void AmbientLight::OnCreate()
	{
		SetCubeMap("Assets/Textures/Default_Cubemap.dds");

		Main::GetRenderManager().AddAmbientLight(this);
	}

	void AmbientLight::OnDestroy()
	{
		Main::GetRenderManager().RemoveAmbientLight(this);
	}

	void AmbientLight::OnEnable()
	{
		Main::GetRenderManager().AddAmbientLight(this);
	}

	void AmbientLight::OnDisable()
	{
		Main::GetRenderManager().RemoveAmbientLight(this);
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
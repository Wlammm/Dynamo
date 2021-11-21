#include "pch.h"
#include "AmbientLight.h"

namespace Dynamo
{
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
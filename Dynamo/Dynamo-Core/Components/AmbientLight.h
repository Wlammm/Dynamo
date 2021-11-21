#pragma once

namespace Dynamo
{
	class AmbientLight : public Component
	{
	public:
		void OnCreate() override;
		void OnDestroy() override;

		void OnEnable() override;
		void OnDisable() override;

		void SetIntensity(const float aIntensity);
		float GetIntensity() const;

		void SetCubeMap(const std::string& aCubeMapPath);

		DXSRV* GetCubeMap();
		DXSRV* const* GetCubeMapConst();

	private:
		float myIntensity = 1.0f;

		DXSRV* myCubeMap = nullptr;
	};
}
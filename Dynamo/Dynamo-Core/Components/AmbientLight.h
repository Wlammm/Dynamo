#pragma once

namespace Dynamo
{
	class AmbientLight : public Component
	{
		COMP(AmbientLight);
	public:
		AmbientLight();

		void ExposeValues() override;
		nlohmann::json Save() override;
		void Load(nlohmann::json& aJson) override;

		void Update() override;
		void EditorUpdate() override;

		void OnCreate() override;

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
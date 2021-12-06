#pragma once

namespace Dynamo
{
	class DirectionalLight : public Component
	{
		COMP(DirectionalLight);
	public:
		DirectionalLight();

		void ExposeValues() override;
		nlohmann::json Save() override;
		void Load(nlohmann::json& aJson) override;

		void OnCreate() override;
		void OnDestroy() override;

		void OnEnable() override;
		void OnDisable() override;

		void SetIntensity(const float aIntensity);
		float GetIntensity() const;

		void SetColor(const Color& aColor);
		const Color& GetColor() const;

		Vec3f GetDirection() const;
		Vec3f GetToLight() const;

	private:
		Transform* myTransform = nullptr;

		Color myColor = { 1, 1, 1, 1 };
		float myIntensity = 10;
	};
}
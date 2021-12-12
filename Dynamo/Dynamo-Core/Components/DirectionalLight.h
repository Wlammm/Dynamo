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

		void Update() override;
		void EditorUpdate() override;

		void SetIntensity(const float aIntensity);
		float GetIntensity() const;

		void SetColor(const Color& aColor);
		const Color& GetColor() const;

		Vec3f GetDirection() const;

	private:
		Transform* myTransform = nullptr;

		Color myColor = { 1, 1, 1, 1 };
		float myIntensity = 10;
	};
}
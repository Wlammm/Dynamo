#pragma once

namespace Dynamo
{
	class PointLight : public Component
	{
		COMP(PointLight);
	public:
		PointLight();

		void ExposeValues() override;
		nlohmann::json Save() override;
		void Load(nlohmann::json& aJson) override;

		void Update() override;
		void EditorUpdate() override;

		void OnCreate() override;

		void SetColor(const Color& aColor);
		const Color& GetColor() const;

		void SetIntensity(const float aIntensity);
		float GetIntensity() const;

		void SetRange(const float aRange);
		float GetRange() const;

		Vec3f GetPosition() const;

	private:
		Transform* myTransform = nullptr;

		Color myColor = { 1, 1, 1, 1 };
		float myRange = 10000;
		float myIntensity = 10;
	};
}
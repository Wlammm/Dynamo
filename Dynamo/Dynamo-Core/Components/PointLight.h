#pragma once

namespace Dynamo
{
	class PointLight : public Component
	{
		COMP(PointLight);
	public:
		void ExposeValues() override;

		void OnCreate() override;
		void OnDestroy() override;
		void OnEnable() override;
		void OnDisable() override;

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
		float myIntensity = 500000;
	};
}
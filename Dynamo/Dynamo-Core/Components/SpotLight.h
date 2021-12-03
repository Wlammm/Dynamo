#pragma once

namespace Dynamo
{
	class SpotLight : public Component
	{
		COMP(SpotLight);
	public:
		SpotLight();

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

		void SetInnerAngle(const float aInnerAngle);
		float GetInnerAngle() const;

		void SetOuterAngle(const float aOuterAngle);
		float GetOuterAngle() const;

		Vec3f GetDirection() const;

	private:
		Transform* myTransform = nullptr;

		Color myColor = { 1, 1, 1, 1 };

		float myIntensity = 100000;
		float myRange = 10000;

		float myInnerAngle = 30.0f;
		float myOuterAngle = 80.0f;
	};
}
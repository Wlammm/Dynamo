#pragma once

#include "Component.h"

namespace Dynamo
{
	class Transform : public Component
	{
		COMP(Transform);
	public:
		void ExposeValues() override;
		nlohmann::json Save() override;
		void Load(nlohmann::json& aJson) override;

		void SetPosition(const Vec3f& aPosition);
		void SetPosition(float x, float y, float z);
		Vec3f GetPosition();

		void SetRotationRad(const Vec3f& aRotation);
		void SetRotationRad(float x, float y, float z);
		Vec3f GetRotationRad() const;

		void SetRotationDeg(const Vec3f& aRotation);
		void SetRotationDeg(float x, float y, float z);
		Vec3f GetRotationDeg() const;

		const Mat4f& GetMatrix() const;
		Mat4f GetRotationMatrix() const;

		Mat4f& GetMatrixRef();

		Quat GetRotation() const;
		void SetRotation(const Quat& aQuat);

		void Rotate(const Quat& aQuat);

		void SetScale(const Vec3f& aScale);
		void SetScale(float x, float y, float z);
		Vec3f GetScale() const;

		Vec3f GetForward() const;
		Vec3f GetRight() const;
		Vec3f GetUp() const;

		void Move(const Vec3f& aVec);
		void Move(float x, float y, float z);

	private:
		Mat4f myMatrix;

	};
}
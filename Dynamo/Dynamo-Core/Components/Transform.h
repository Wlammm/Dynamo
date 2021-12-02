#pragma once

#include "Component.h"

namespace Dynamo
{
	class Transform : public Component
	{
		COMP(Transform);
	public:
		void ExposeValues() override;

		void SetPosition(const Vec3f& aPosition);
		Vec3f GetPosition();

		void SetRotationRad(const Vec3f& aRotation);
		Vec3f GetRotationRad() const;

		void SetRotationDeg(const Vec3f& aRotation);
		Vec3f GetRotationDeg() const;

		const Mat4f& GetMatrix() const;
		Mat4f GetRotationMatrix() const;

		Mat4f& GetMatrixRef();

		Quat GetRotation() const;
		void SetRotation(const Quat& aQuat);

		void Rotate(const Quat& aQuat);

		void SetScale(const Vec3f& aScale);
		Vec3f GetScale() const;

		Vec3f GetForward() const;
		Vec3f GetRight() const;
		Vec3f GetUp() const;

		void Move(const Vec3f& aVec);

	private:
		Mat4f myMatrix;

	};
}
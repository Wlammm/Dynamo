#pragma once

#include "Component.h"

namespace Dynamo
{
	class Transform : public Component
	{
	public:
		void SetPosition(const Vec3f& aPosition);
		Vec3f GetPosition();

		void SetRotationRad(const Vec3f& aRotation);
		Vec3f GetRotationRad() const;

		void SetRotationDeg(const Vec3f& aRotation);
		Vec3f GetRotationDeg() const;

		const Mat4f& GetMatrix() const;
		Mat4f GetRotationMatrix() const;

		Quat GetRotation() const;
		void SetRotation(const Quat& aQuat);

		void Rotate(const Quat& aQuat);

		void SetScale(const Vec3f& aScale);
		Vec3f GetScale() const;

		Vec3f GetForward() const;
		Vec3f GetRight() const;
		Vec3f GetUp() const;

	private:
		Mat4f myMatrix;

	};
}
#include "pch.h"
#include "Transform.h"

namespace Dynamo
{
	void Transform::ExposeValues()
	{
		Vec3f position, rotation, scale;
		ImGuizmo::DecomposeMatrixToComponents(&myMatrix(1, 1), &position.x, &rotation.x, &scale.x);
		bool isDirty = false;

		ImGui::Text("Position");
		ImGui::SameLine(0.0f, 40);
		if (ImGui::DragFloat3("##pos", &position.x))
		{
			isDirty = true;
		}

		ImGui::Text("Rotation");
		ImGui::SameLine(0.0f, 40);
		if (ImGui::DragFloat3("##rot", &rotation.x))
		{
			isDirty = true;
		}

		ImGui::Text("Scale");
		ImGui::SameLine(0.0f, 60);
		if (ImGui::DragFloat3("##scale", &scale.x, 0.01f))
		{
			isDirty = true;
		}

		if (isDirty)
		{
			ImGuizmo::RecomposeMatrixFromComponents(&position.x, &rotation.x, &scale.x, &myMatrix(1, 1));
		}
	}

	void Dynamo::Transform::SetPosition(const Vec3f& aPosition)
	{
		myMatrix(4, 1) = aPosition.x;
		myMatrix(4, 2) = aPosition.y;
		myMatrix(4, 3) = aPosition.z;
	}

	Vec3f Transform::GetPosition()
	{
		return { myMatrix(4, 1), myMatrix(4, 2), myMatrix(4, 3) };
	}

	void Transform::SetRotationRad(const Vec3f& aRotation)
	{
		Mat4f translation;
		translation(4, 1) = myMatrix(4, 1);
		translation(4, 2) = myMatrix(4, 2);
		translation(4, 3) = myMatrix(4, 3);
		Vec3f scale = GetScale();

		Mat4f newMatrix;
		newMatrix =
			Mat4f::CreateRotationAroundZ(aRotation.z) *
			Mat4f::CreateRotationAroundX(aRotation.x) *
			Mat4f::CreateRotationAroundY(aRotation.y) *
			translation;

		myMatrix = newMatrix;
		SetScale(scale);
	}

	Vec3f Transform::GetRotationRad() const
	{
		float h, b, p;
		Mat4f transpose = GetRotationMatrix();
		transpose = Mat4f::Transpose(transpose);

		float sp = -transpose(3, 2);
		if (sp <= -1.0f)
		{
			p = PI * -0.5f;
		}
		else if (sp >= 1.0f)
		{
			p = PI * 0.5f;
		}
		else
		{
			p = asin(sp);
		}

		if (abs(sp) > 0.9999f)
		{
			b = 0.0f;
			h = atan2(-transpose(1, 3), transpose(1, 1));
		}
		else
		{
			h = atan2(transpose(3, 1), transpose(3, 3));
			b = atan2(transpose(1, 2), transpose(2, 2));
		}

		return { p, h, b };
	}

	void Transform::SetRotationDeg(const Vec3f& aRotation)
	{
		SetRotationRad(aRotation * Deg2Rad);
	}

	Vec3f Transform::GetRotationDeg() const
	{
		return GetRotationRad() * Rad2Deg;
	}

	const Mat4f& Transform::GetMatrix() const
	{
		return myMatrix;
	}

	Mat4f Transform::GetRotationMatrix() const
	{
		Vec4f xRot = myMatrix.GetRow(1).GetNormalized();
		Vec4f yRot = myMatrix.GetRow(2).GetNormalized();
		Vec4f zRot = myMatrix.GetRow(3).GetNormalized();

		return Mat4f(
			xRot.x, xRot.y, xRot.z, 0.0f,
			yRot.x, yRot.y, yRot.z, 0.0f,
			zRot.x, zRot.y, zRot.z, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
	}

	Mat4f& Transform::GetMatrixRef()
	{
		return myMatrix;
	}

	Quat Transform::GetRotation() const
	{
		Mat4f mat = GetRotationMatrix();
		return Quat(mat);
	}

	void Transform::SetRotation(const Quat& aQuat)
	{
		Vec3f pos = GetPosition();
		Vec3f scale = GetScale();
		myMatrix = aQuat.ToMatrix();
		SetScale(scale);
		SetPosition(pos);
	}

	void Transform::Rotate(const Quat& aQuat)
	{
		Quat rot = GetRotation();
		rot = aQuat * rot;
		SetRotation(rot);
	}
	
	void Transform::SetScale(const Vec3f& aScale)
	{
		Vec3f x = { myMatrix(1, 1), myMatrix(1, 2), myMatrix(1, 3) };
		x.Normalize();
		x *= aScale.x == 0 ? 0.0001f : aScale.x;
		myMatrix(1, 1) = x.x;
		myMatrix(1, 2) = x.y;
		myMatrix(1, 3) = x.z;

		Vec3f y = { myMatrix(2, 1), myMatrix(2, 2), myMatrix(2, 3) };
		y.Normalize();
		y *= aScale.y == 0 ? 0.0001f : aScale.y;
		myMatrix(2, 1) = y.x;
		myMatrix(2, 2) = y.y;
		myMatrix(2, 3) = y.z;

		Vec3f z = { myMatrix(3, 1), myMatrix(3, 2), myMatrix(3, 3) };
		z.Normalize();
		z *= aScale.z == 0 ? 0.0001f : aScale.z;
		myMatrix(3, 1) = z.x;
		myMatrix(3, 2) = z.y;
		myMatrix(3, 3) = z.z;
	}

	Vec3f Transform::GetScale() const
	{
		Vec3f scale;
		scale.x = Vec3f{ myMatrix(1, 1), myMatrix(1, 2), myMatrix(1, 3) }.Length();
		scale.y = Vec3f{ myMatrix(2, 1), myMatrix(2, 2), myMatrix(2, 3) }.Length();
		scale.z = Vec3f{ myMatrix(3, 1), myMatrix(3, 2), myMatrix(3, 3) }.Length();
		return scale;
	}
	
	Vec3f Transform::GetForward() const
	{
		Vec4f forward = Vec4f(0, 0, 1, 0) * myMatrix;
		return Vec3f(forward.x, forward.y, forward.z).GetNormalized();
	}

	Vec3f Transform::GetRight() const
	{
		Vec4f right = Vec4f(1, 0, 0, 0) * myMatrix;
		return Vec3f(right.x, right.y, right.z).GetNormalized();
	}
	
	Vec3f Transform::GetUp() const
	{
		Vec4f up = Vec4f(0, 1, 0, 0) * myMatrix;
		return Vec3f(up.x, up.y, up.z).GetNormalized();
	}
	void Transform::Move(const Vec3f& aVec)
	{
		Vec3f pos = GetPosition();
		pos += aVec;
		SetPosition(pos);
	}
}

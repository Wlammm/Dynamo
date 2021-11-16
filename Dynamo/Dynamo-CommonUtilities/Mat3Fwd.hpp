#pragma once

namespace CommonUtilities
{
	template<class T>
	class Matrix4x4;

	template<class T>
	class Matrix3x3
	{
	public:
		Matrix3x3<T>();
		Matrix3x3<T>(
			const T m11, const T m12, const T m13,
			const T m21, const T m22, const T m23,
			const T m31, const T m32, const T m33
			);
		Matrix3x3<T>(const Matrix3x3<T>& aMatrix);
		Matrix3x3<T>(const Matrix4x4<T>& aMatrix);

		[[nodiscard]] T& operator()(const int aRow, const int aColumn);
		[[nodiscard]] const T& operator()(const int aRow, const int aColumn) const;
		Matrix3x3<T> operator=(const Matrix3x3<T>& aMatrix);

		[[nodiscard]] T Minor(int aX, int aY) const;
		[[nodiscard]] T Cofactor(int aX, int aY) const;
		[[nodiscard]] T Determinant() const;
		[[nodiscard]] Matrix3x3<T> Cofactors() const;
		[[nodiscard]] Matrix3x3<T> Adjoint() const;
		[[nodiscard]] Matrix3x3<T> Inverse() const;

		[[nodiscard]] static Matrix3x3<T> CreateRotationAroundX(T aAngleInRadians);
		[[nodiscard]] static Matrix3x3<T> CreateRotationAroundY(T aAngleInRadians);
		[[nodiscard]] static Matrix3x3<T> CreateRotationAroundZ(T aAngleInRadians);

		[[nodiscard]] static Matrix3x3<T> Transpose(const Matrix3x3<T>& aMatrixToTranspose);

	private:
		T myData[3][3];
	};

	using Matrix3x3f = Matrix3x3<float>;
}
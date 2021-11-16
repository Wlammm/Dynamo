#pragma once

namespace CommonUtilities
{
	template<class T>
	class Vector4;

	template<class T>
	class Vector3;

	template<class T>
	class QuaternionT;

	template<class T>
	class Matrix4x4
	{
	public:
		Matrix4x4<T>();
		Matrix4x4<T>(
			const T m11, const T m12, const T m13, const T m14,
			const T m21, const T m22, const T m23, const T m24,
			const T m31, const T m32, const T m33, const T m34,
			const T m41, const T m42, const T m43, const T m44
			);
		Matrix4x4<T>(const Matrix4x4<T>& aMatrix);

		[[nodiscard]] T& operator()(const int aRow, const int aColumn);
		[[nodiscard]] const T& operator()(const int aRow, const int aColumn) const;
		Matrix4x4<T>& operator=(const Matrix4x4<T>& aMatrix);

		[[nodiscard]] Vector4<T> GetRow(const int aRow) const;
		void SetRow(const int aRow, const Vector4<T>& aValue);
		[[nodiscard]] Vector4<T> GetColumn(const int aColumn) const;
		void SetColumn(const int aColumn, const Vector4<T>& aValue);

		[[nodiscard]] T Minor(const int aX, const int aY) const;
		[[nodiscard]] T Cofactor(const int aX, const int aY) const;
		[[nodiscard]] T Determinant() const;
		[[nodiscard]] Matrix4x4<T> Cofactors() const;
		[[nodiscard]] Matrix4x4<T> Adjoint() const;
		[[nodiscard]] Matrix4x4<T> Inverse() const;
		[[nodiscard]] Matrix4x4<T> Transposed() const;
		[[nodiscard]] Matrix4x4<T> FastInverse() const;
		[[nodiscard]] static Matrix4x4<T> Transpose(const Matrix4x4<T>& aMatrixToTranspose);
		[[nodiscard]] static Matrix4x4<T> GetFastInverse(const Matrix4x4<T>& aTransform);

		[[nodiscard]] static Matrix4x4<T> CreateRotationAroundX(T aAngleInRadians);
		[[nodiscard]] static Matrix4x4<T> CreateRotationAroundY(T aAngleInRadians);
		[[nodiscard]] static Matrix4x4<T> CreateRotationAroundZ(T aAngleInRadians);
		[[nodiscard]] static Matrix4x4<T> CreateRotationAroundAxis(T anAngleInRadians, CU::Vector4<T> anAxis);
		[[nodiscard]] static Matrix4x4<T> CreateTranslation(const Vector3<T>& aPosition);
		[[nodiscard]] static Matrix4x4<T> CreateTranslationColumn(const Vector3<T>& aPosition);
		[[nodiscard]] static Matrix4x4<T> CreateRotation(const Vector3<T>& aRotation);
		[[nodiscard]] static Matrix4x4<T> CreateScale(const Vector3<T>& aScale);
		[[nodiscard]] static Matrix4x4<T> TRS(const Vector3<T>& aTranslation, const Vector3<T>& aRotation, const Vector3<T>& aScale);

		[[nodiscard]] static Matrix4x4<T> TRS(const Vector3<T>& aTranslation, const QuaternionT<T>& aRotation, const Vector3<T>& aScale);


		void LookTowards(const Vector3<T>& aPosition);

		void InitWithColumnMatrix(T* aFirst);

		static Matrix4x4<T> ColumnToRow(T* aFirst);
		static Matrix4x4<T> ColumnToColumn(T* aFirst);

	private:
		T myData[4][4];
	};

	template <class T>
	[[nodiscard]] bool operator==(const Matrix4x4<T>& aMatrix0, const Matrix4x4<T>& aMatrix1);
	template <class T>
	[[nodiscard]] bool operator!=(const Matrix4x4<T>& aMatrix0, const Matrix4x4<T>& aMatrix1);

	using Matrix4x4f = Matrix4x4<float>;
}
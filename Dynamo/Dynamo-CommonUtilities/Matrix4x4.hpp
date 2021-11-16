#pragma once

#include "Mat4Fwd.hpp"
#include "Matrix3x3.hpp"
#include "Quaternion.hpp"

namespace CommonUtilities
{
	template <class T>
	Matrix4x4<T>::Matrix4x4() :
		myData{
			{ 1, 0, 0, 0 },
			{ 0, 1, 0, 0 },
			{ 0, 0, 1, 0 },
			{ 0, 0, 0, 1 },
	}
	{
	}

	template <class T>
	Matrix4x4<T>::Matrix4x4(
		const T m11, const T m12, const T m13, const T m14,
		const T m21, const T m22, const T m23, const T m24,
		const T m31, const T m32, const T m33, const T m34,
		const T m41, const T m42, const T m43, const T m44
	) :
		myData{
			{ m11, m12, m13, m14 },
			{ m21, m22, m23, m24 },
			{ m31, m32, m33, m34 },
			{ m41, m42, m43, m44 },
	}
	{
	}

	template <class T>
	Matrix4x4<T>::Matrix4x4(const Matrix4x4<T>& aMatrix) :
		myData{
			{ aMatrix(1, 1), aMatrix(1, 2), aMatrix(1, 3), aMatrix(1, 4) },
			{ aMatrix(2, 1), aMatrix(2, 2), aMatrix(2, 3), aMatrix(2, 4) },
			{ aMatrix(3, 1), aMatrix(3, 2), aMatrix(3, 3), aMatrix(3, 4) },
			{ aMatrix(4, 1), aMatrix(4, 2), aMatrix(4, 3), aMatrix(4, 4) }
	}
	{
	};

	template <class T>
	T& Matrix4x4<T>::operator()(const int aRow, const int aColumn)
	{
		return myData[aRow - 1][aColumn - 1];
	}

	template <class T>
	const T& Matrix4x4<T>::operator()(const int aRow, const int aColumn) const
	{
		return myData[aRow - 1][aColumn - 1];
	}

	template <class T>
	void Matrix4x4<T>::SetRow(const int aRow, const Vector4<T>& aValue)
	{
		(*this)(aRow, 1) = aValue.x;
		(*this)(aRow, 2) = aValue.y;
		(*this)(aRow, 3) = aValue.z;
		(*this)(aRow, 4) = aValue.w;
	}

	template <class T>
	void Matrix4x4<T>::SetColumn(const int aColumn, const Vector4<T>& aValue)
	{
		(*this)(1, aColumn) = aValue.x;
		(*this)(2, aColumn) = aValue.y;
		(*this)(3, aColumn) = aValue.z;
		(*this)(4, aColumn) = aValue.w;
	}

	template <class T>
	Matrix4x4<T> Matrix4x4<T>::CreateRotationAroundX(T aAngleInRadians)
	{
		const T& a = aAngleInRadians;
		return Matrix4x4<T>(
			1, 0, 0, 0,
			0, std::cos(a), -std::sin(a), 0,
			0, std::sin(a), std::cos(a), 0,
			0, 0, 0, 1
			);
	}

	template <class T>
	Matrix4x4<T> Matrix4x4<T>::CreateRotationAroundY(T aAngleInRadians)
	{
		const T& a = aAngleInRadians;
		return Matrix4x4<T>(
			std::cos(a), 0, std::sin(a), 0,
			0, 1, 0, 0,
			-std::sin(a), 0, std::cos(a), 0,
			0, 0, 0, 1
			);
	}

	template <class T>
	Matrix4x4<T> Matrix4x4<T>::CreateRotationAroundZ(T aAngleInRadians)
	{
		const T& a = aAngleInRadians;
		return Matrix4x4<T>(
			std::cos(a), -std::sin(a), 0, 0,
			std::sin(a), std::cos(a), 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
			);
	}

	template<class T>
	inline Matrix4x4<T> Matrix4x4<T>::CreateRotationAroundAxis(T anAngleInRadians, CU::Vector4<T> anAxis)
	{
		CU::Matrix4x4<T> output;
		anAxis.Normalize();
		T cosAngle = cos(anAngleInRadians);
		T sinAngle = sin(anAngleInRadians);
		T oneMinusCos = 1 - cosAngle;
		output(1, 1) = anAxis.x * anAxis.x * (oneMinusCos)+cosAngle;
		output(1, 2) = anAxis.x * anAxis.y * (oneMinusCos)+anAxis.z * sinAngle;
		output(1, 3) = anAxis.x * anAxis.z * (oneMinusCos)-anAxis.y * sinAngle;

		output(2, 1) = anAxis.x * anAxis.y * (oneMinusCos)-anAxis.z * sinAngle;
		output(2, 2) = anAxis.y * anAxis.y * (oneMinusCos)+cosAngle;
		output(2, 3) = anAxis.y * anAxis.z * (oneMinusCos)+anAxis.x * sinAngle;

		output(3, 1) = anAxis.x * anAxis.z * (oneMinusCos)+anAxis.y * sinAngle;
		output(3, 2) = anAxis.y * anAxis.z * (oneMinusCos)-anAxis.x * sinAngle;
		output(3, 3) = anAxis.z * anAxis.z * (oneMinusCos)+cosAngle;

		output(4, 4) = 1;

		return output;
	}

	template<class T>
	inline Matrix4x4<T> Matrix4x4<T>::Transposed() const
	{
		return Transpose(*this);
	}

	template<class T>
	inline Matrix4x4<T> Matrix4x4<T>::FastInverse() const
	{
		return Matrix4x4<T>::GetFastInverse(*this);
	}

	template <class T>
	Matrix4x4<T> Matrix4x4<T>::Transpose(const Matrix4x4<T>& aMatrixToTranspose)
	{
		Matrix4x4<T> result;

		for (int row = 1; row <= 4; row++)
		{
			for (int col = 1; col <= 4; col++)
			{
				result(col, row) = aMatrixToTranspose(row, col);
			}
		}

		return result;
	}

	template <class T>
	Matrix4x4<T> Matrix4x4<T>::GetFastInverse(const Matrix4x4<T>& aTransform)
	{
		const Matrix4x4<T>& m4 = aTransform;
		Matrix3x3<T> m3(m4);
		m3 = Matrix3x3<T>::Transpose(m3);
		Vector3<T> t(-m4(4, 1), -m4(4, 2), -m4(4, 3));
		t *= m3;

		return Matrix4x4<T>(
			m3(1, 1), m3(1, 2), m3(1, 3), 0,
			m3(2, 1), m3(2, 2), m3(2, 3), 0,
			m3(3, 1), m3(3, 2), m3(3, 3), 0,
			t[0], t[1], t[2], 1
			);
	}

	template <class T>
	Vector4<T> Matrix4x4<T>::GetRow(const int aRow) const
	{
		return Vector4<T>((*this)(aRow, 1), (*this)(aRow, 2), (*this)(aRow, 3), (*this)(aRow, 4));
	}
	template <class T>
	Vector4<T> Matrix4x4<T>::GetColumn(const int aColumn) const
	{
		return Vector4<T>((*this)(1, aColumn), (*this)(2, aColumn), (*this)(3, aColumn), (*this)(4, aColumn));
	}


	template <class T>
	Matrix4x4<T> operator+(const Matrix4x4<T>& aMatrix0, const Matrix4x4<T>& aMatrix1)
	{
		Matrix4x4<T> result(aMatrix0);
		result += aMatrix1;
		return result;
	}

	template <class T>
	Matrix4x4<T> operator-(const Matrix4x4<T>& aMatrix0, const Matrix4x4<T>& aMatrix1)
	{
		Matrix4x4<T> result(aMatrix0);
		result -= aMatrix1;
		return result;
	}

	template <class T>
	Matrix4x4<T> operator*(const Matrix4x4<T>& aMatrix0, const Matrix4x4<T>& aMatrix1)
	{
		Matrix4x4<T> result(aMatrix0);
		result *= aMatrix1;
		return result;
	}

	template <class T>
	Vector4<T> operator*(const Vector4<T>& aVector, const Matrix4x4<T>& aMatrix)
	{
		Vector4<T> result(aVector);
		result *= aMatrix;
		return result;
	}

	template <class T>
	Matrix4x4<T> operator*(const Matrix4x4<T>& aMatrix, const T aScalar)
	{
		Matrix4x4<T> result(aMatrix);
		result *= aScalar;
		return result;
	}

	template <class T>
	void operator+=(Matrix4x4<T>& aMatrix0, const Matrix4x4<T>& aMatrix1)
	{
		for (int row = 1; row <= 4; row++)
		{
			for (int col = 1; col <= 4; col++)
			{
				aMatrix0(row, col) += aMatrix1(row, col);
			}
		}
	}

	template <class T>
	void operator-=(Matrix4x4<T>& aMatrix0, const Matrix4x4<T>& aMatrix1)
	{
		for (int row = 1; row <= 4; row++)
		{
			for (int col = 1; col <= 4; col++)
			{
				aMatrix0(row, col) -= aMatrix1(row, col);
			}
		}
	}

	template <class T>
	void operator*=(Matrix4x4<T>& aMatrix0, const Matrix4x4<T>& aMatrix1)
	{
		const Matrix4x4<T> matrix0Original = aMatrix0;
		for (int row = 1; row <= 4; row++)
		{
			for (int col = 1; col <= 4; col++)
			{
				T sum = 0;
				for (int i = 1; i <= 4; i++)
				{
					sum += matrix0Original(row, i) * aMatrix1(i, col);
				}
				aMatrix0(row, col) = sum;
			}
		}
	}

	template <class T>
	void operator*=(Vector4<T>& aVector, const Matrix4x4<T>& aMatrix)
	{
		const Vector4<T> vectorOriginal = aVector;
		for (int col = 1; col <= 4; col++)
		{
			T sum = 0;
			for (int i = 1; i <= 4; i++)
			{
				sum += vectorOriginal[i - 1] * aMatrix(i, col);
			}
			aVector[col - 1] = sum;
		}
	}

	template <class T>
	void operator*=(Matrix4x4<T>& aMatrix, const T aScalar)
	{
		for (int row = 1; row <= 4; row++)
		{
			for (int col = 1; col <= 4; col++)
			{
				aMatrix(row, col) *= aScalar;
			}
		}
	}

	template <class T>
	Matrix4x4<T>& Matrix4x4<T>::operator=(const Matrix4x4<T>& aMatrix)
	{
		for (int row = 1; row <= 4; row++)
		{
			for (int col = 1; col <= 4; col++)
			{
				(*this)(row, col) = aMatrix(row, col);
			}
		}

		return *this;
	}

	template <class T>
	bool operator==(const Matrix4x4<T>& aMatrix0, const Matrix4x4<T>& aMatrix1)
	{
		for (int row = 1; row <= 4; row++)
		{
			for (int col = 1; col <= 4; col++)
			{
				if (abs(aMatrix0(row, col) - aMatrix1(row, col)) > 0.01f)
				{
					return false;
				}
			}
		}

		return true;
	}

	template <class T>
	bool operator!=(const Matrix4x4<T>& aMatrix0, const Matrix4x4<T>& aMatrix1)
	{
		return !(aMatrix0 == aMatrix1);
	}

	template <class T>
	T Matrix4x4<T>::Minor(const int aX, const int aY) const
	{
		Matrix3x3<T> result;

		for (int outY = 1; outY <= 3; outY++)
		{
			int inY = outY;
			if (outY >= aY)
			{
				inY++;
			}
			for (int outX = 1; outX <= 3; outX++)
			{
				int inX = outX;
				if (outX >= aX)
				{
					inX++;
				}

				result(outY, outX) = (*this)(inY, inX);
			}
		}

		return result.Determinant();
	}

	template <class T>
	T Matrix4x4<T>::Cofactor(const int aX, const int aY) const
	{
		T result = Minor(aX, aY);

		if ((((aX - 1) ^ (aY - 1)) & 1) > 0)
		{
			result = -result;
		}

		return result;
	}

	template <class T>
	T Matrix4x4<T>::Determinant() const
	{
		T result = 0;

		for (int i = 1; i <= 4; i++)
		{
			result += (*this)(1, i) * Cofactor(i, 1);
		}

		return result;
	}

	template <class T>
	Matrix4x4<T> Matrix4x4<T>::Cofactors() const
	{
		Matrix4x4<T> result;

		for (unsigned int y = 1; y <= 4; y++)
		{
			for (unsigned int x = 1; x <= 4; x++)
			{
				result(y, x) = Cofactor(x, y);
			}
		}

		return result;
	}

	template <class T>
	Matrix4x4<T> Matrix4x4<T>::Adjoint() const
	{
		return Matrix4x4<T>::Transpose(Cofactors());
	}

	template <class T>
	Matrix4x4<T> Matrix4x4<T>::Inverse() const
	{
		return Adjoint() * (1 / Determinant());
	}

	template <class T>
	Matrix4x4<T> Matrix4x4<T>::CreateTranslation(const Vector3<T>& aPosition)
	{
		const Vector3<T>& p = aPosition;
		return Matrix4x4<T>(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			p.x, p.y, p.z, 1
			);
	}

	template<class T>
	inline Matrix4x4<T> Matrix4x4<T>::CreateTranslationColumn(const Vector3<T>& aPosition)
	{
		const Vector3<T>& p = aPosition;
		return Matrix4x4<T>(
			1, 0, 0, p.x,
			0, 1, 0, p.y,
			0, 0, 1, p.z,
			0, 0, 0, 1
			);
	}

	template <class T>
	Matrix4x4<T> Matrix4x4<T>::CreateRotation(const Vector3<T>& aRotation)
	{
		return CreateRotationAroundZ(aRotation.z) * CreateRotationAroundX(aRotation.x) * CreateRotationAroundY(aRotation.y);
	}

	template <class T>
	Matrix4x4<T> Matrix4x4<T>::CreateScale(const Vector3<T>& aScale)
	{
		const Vector3<T>& s = aScale;
		return Matrix4x4<T>(
			s.x, 0, 0, 0,
			0, s.y, 0, 0,
			0, 0, s.z, 0,
			0, 0, 0, 1
			);
	}

	template <class T>
	Matrix4x4<T> Matrix4x4<T>::TRS(const Vector3<T>& aTranslation, const Vector3<T>& aRotation, const Vector3<T>& aScale)
	{
		return CreateScale(aScale) * CreateRotation(aRotation) * CreateTranslation(aTranslation);
	}

	template <class T>
	Matrix4x4<T> Matrix4x4<T>::TRS(const Vector3<T>& aTranslation, const QuaternionT<T>& aRotation, const Vector3<T>& aScale)
	{
		return CreateScale(aScale) * aRotation.ToMatrix() * CreateTranslation(aTranslation);
	}

	template<class T>
	inline void Matrix4x4<T>::LookTowards(const Vector3<T>& aPosition)
	{
		CU::Vector3<T> up = { myData[1][0], myData[1][1], myData[1][2] };
		CU::Vector3<T> currentPos = { myData[3][0], myData[3][1], myData[3][2] };

		CU::Vector3<T> f = CU::Vector3<T>(aPosition - currentPos).GetNormalized();
		CU::Vector3<T> s = CU::Vector3<T>(up.Cross(f)).GetNormalized();
		CU::Vector3<T> u = CU::Vector3<T>(f.Cross(s));

		myData[0][0] = s.x;
		myData[0][1] = s.y;
		myData[0][2] = s.z;

		myData[1][0] = u.x;
		myData[1][1] = u.y;
		myData[1][2] = u.z;

		myData[2][0] = u.x;
		myData[2][1] = f.y;
		myData[2][2] = f.z;

		myData[0][3] = -s.Dot(currentPos);
		myData[1][3] = -u.Dot(currentPos);
		myData[2][3] = -f.Dot(currentPos);
	}

	template<class T>
	inline void Matrix4x4<T>::InitWithColumnMatrix(T* aFirst)
	{
		myData(1, 1) = aFirst[0];
		myData(1, 2) = aFirst[4];
		myData(1, 3) = aFirst[8];
		myData(1, 4) = aFirst[12];

		myData(2, 1) = aFirst[1];
		myData(2, 2) = aFirst[5];
		myData(2, 3) = aFirst[9];
		myData(2, 4) = aFirst[13];

		myData(3, 1) = aFirst[2];
		myData(3, 2) = aFirst[6];
		myData(3, 3) = aFirst[10];
		myData(3, 4) = aFirst[14];

		myData(4, 1) = aFirst[3];
		myData(4, 2) = aFirst[7];
		myData(4, 3) = aFirst[11];
		myData(4, 4) = aFirst[15];
	}

	template<class T>
	inline Matrix4x4<T> Matrix4x4<T>::ColumnToRow(T* aFirst)
	{
		Matrix4x4<T> mat;
		mat(1, 1) = aFirst[0];
		mat(1, 2) = aFirst[4];
		mat(1, 3) = aFirst[8];
		mat(1, 4) = aFirst[12];

		mat(2, 1) = aFirst[1];
		mat(2, 2) = aFirst[5];
		mat(2, 3) = aFirst[9];
		mat(2, 4) = aFirst[13];

		mat(3, 1) = aFirst[2];
		mat(3, 2) = aFirst[6];
		mat(3, 3) = aFirst[10];
		mat(3, 4) = aFirst[14];

		mat(4, 1) = aFirst[3];
		mat(4, 2) = aFirst[7];
		mat(4, 3) = aFirst[11];
		mat(4, 4) = aFirst[15];

		return mat;
	}

	template<class T>
	inline Matrix4x4<T> Matrix4x4<T>::ColumnToColumn(T* aFirst)
	{
		Matrix4x4<T> mat;
		mat(1, 1) = aFirst[0];
		mat(1, 2) = aFirst[1];
		mat(1, 3) = aFirst[2];
		mat(1, 4) = aFirst[3];

		mat(2, 1) = aFirst[4];
		mat(2, 2) = aFirst[5];
		mat(2, 3) = aFirst[6];
		mat(2, 4) = aFirst[7];

		mat(3, 1) = aFirst[8];
		mat(3, 2) = aFirst[9];
		mat(3, 3) = aFirst[10];
		mat(3, 4) = aFirst[11];

		mat(4, 1) = aFirst[12];
		mat(4, 2) = aFirst[13];
		mat(4, 3) = aFirst[14];
		mat(4, 4) = aFirst[15];

		return mat;
	}
}
#pragma once
#include "Matrix2x2.hpp"
#include "Vector3.hpp"

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

		T& operator()(const int aRow, const int aColumn);
		const T& operator()(const int aRow, const int aColumn) const;
		Matrix3x3<T> operator=(const Matrix3x3<T>& aMatrix);

		T Minor(int aX, int aY) const;
		T Cofactor(int aX, int aY) const;
		T Determinant() const;
		Matrix3x3<T> Cofactors() const;
		Matrix3x3<T> Adjoint() const;
		Matrix3x3<T> Inverse() const;

		static Matrix3x3<T> CreateRotationAroundX(T aAngleInRadians);
		static Matrix3x3<T> CreateRotationAroundY(T aAngleInRadians);
		static Matrix3x3<T> CreateRotationAroundZ(T aAngleInRadians);

		static Matrix3x3<T> Transpose(const Matrix3x3<T>& aMatrixToTranspose);

	private:
		T myData[3][3];
	};

	using Matrix3x3f = Matrix3x3<float>;
}

namespace CommonUtilities
{
	template <class T>
	Matrix3x3<T>::Matrix3x3() :
		myData{
			{ 1, 0, 0 },
			{ 0, 1, 0 },
			{ 0, 0, 1 },
	}
	{
	}

	template <class T>
	Matrix3x3<T>::Matrix3x3(
		const T m11, const T m12, const T m13,
		const T m21, const T m22, const T m23,
		const T m31, const T m32, const T m33
	) :
		myData{
			{ m11, m12, m13 },
			{ m21, m22, m23 },
			{ m31, m32, m33 },
	}
	{
	}

	template <class T>
	Matrix3x3<T>::Matrix3x3(const Matrix3x3<T>& aMatrix) :
		myData{
			{ aMatrix(1, 1), aMatrix(1, 2), aMatrix(1, 3) },
			{ aMatrix(2, 1), aMatrix(2, 2), aMatrix(2, 3) },
			{ aMatrix(3, 1), aMatrix(3, 2), aMatrix(3, 3) }
	}
	{
	};

	template <class T>
	Matrix3x3<T>::Matrix3x3(const Matrix4x4<T>& aMatrix) :
		myData{
			{ aMatrix(1, 1), aMatrix(1, 2), aMatrix(1, 3) },
			{ aMatrix(2, 1), aMatrix(2, 2), aMatrix(2, 3) },
			{ aMatrix(3, 1), aMatrix(3, 2), aMatrix(3, 3) }
	}
	{
	};

	template <class T>
	T& Matrix3x3<T>::operator()(const int aRow, const int aColumn)
	{
		return myData[aRow - 1][aColumn - 1];
	}

	template <class T>
	const T& Matrix3x3<T>::operator()(const int aRow, const int aColumn) const
	{
		return myData[aRow - 1][aColumn - 1];
	}

	template <class T>
	Matrix3x3<T> Matrix3x3<T>::CreateRotationAroundX(T aAngleInRadians)
	{
		const T& a = aAngleInRadians;
		return Matrix3x3<T>(
			1, 0, 0,
			0, std::cos(a), std::sin(a),
			0, -std::sin(a), std::cos(a)
			);
	}

	template <class T>
	Matrix3x3<T> Matrix3x3<T>::CreateRotationAroundY(T aAngleInRadians)
	{
		const T& a = aAngleInRadians;
		return Matrix3x3<T>(
			std::cos(a), 0, -std::sin(a),
			0, 1, 0,
			std::sin(a), 0, std::cos(a)
			);
	}

	template <class T>
	Matrix3x3<T> Matrix3x3<T>::CreateRotationAroundZ(T aAngleInRadians)
	{
		const T& a = aAngleInRadians;
		return Matrix3x3<T>(
			std::cos(a), std::sin(a), 0,
			-std::sin(a), std::cos(a), 0,
			0, 0, 1
			);
	}

	template <class T>
	Matrix3x3<T> Matrix3x3<T>::Transpose(const Matrix3x3<T>& aMatrixToTranspose)
	{
		Matrix3x3<T> result;

		for (int row = 1; row <= 3; row++)
		{
			for (int col = 1; col <= 3; col++)
			{
				result(col, row) = aMatrixToTranspose(row, col);
			}
		}

		return result;
	}

	template <class T>
	Matrix3x3<T> operator+(const Matrix3x3<T>& aMatrix0, const Matrix3x3<T>& aMatrix1)
	{
		Matrix3x3<T> result(aMatrix0);
		result += aMatrix1;
		return result;
	}

	template <class T>
	Matrix3x3<T> operator-(const Matrix3x3<T>& aMatrix0, const Matrix3x3<T>& aMatrix1)
	{
		Matrix3x3<T> result(aMatrix0);
		result -= aMatrix1;
		return result;
	}

	template <class T>
	Matrix3x3<T> operator*(const Matrix3x3<T>& aMatrix0, const Matrix3x3<T>& aMatrix1)
	{
		Matrix3x3<T> result(aMatrix0);
		result *= aMatrix1;
		return result;
	}

	template <class T>
	Vector3<T> operator*(const Vector3<T>& aVector, const Matrix3x3<T>& aMatrix)
	{
		Vector3<T> result(aVector);
		result *= aMatrix;
		return result;
	}

	template <class T>
	void operator+=(Matrix3x3<T>& aMatrix0, const Matrix3x3<T>& aMatrix1)
	{
		for (int row = 1; row <= 3; row++)
		{
			for (int col = 1; col <= 3; col++)
			{
				aMatrix0(row, col) += aMatrix1(row, col);
			}
		}
	}

	template <class T>
	void operator-=(Matrix3x3<T>& aMatrix0, const Matrix3x3<T>& aMatrix1)
	{
		for (int row = 1; row <= 3; row++)
		{
			for (int col = 1; col <= 3; col++)
			{
				aMatrix0(row, col) -= aMatrix1(row, col);
			}
		}
	}

	template <class T>
	void operator*=(Matrix3x3<T>& aMatrix0, const Matrix3x3<T>& aMatrix1)
	{
		const Matrix3x3<T> matrix0Original = aMatrix0;
		for (int row = 1; row <= 3; row++)
		{
			for (int col = 1; col <= 3; col++)
			{
				T sum = 0;
				for (int i = 1; i <= 3; i++)
				{
					sum += matrix0Original(row, i) * aMatrix1(i, col);
				}
				aMatrix0(row, col) = sum;
			}
		}
	}

	template <class T>
	void operator*=(Vector3<T>& aVector, const Matrix3x3<T>& aMatrix)
	{
		const Vector3<T> vectorOriginal = aVector;
		for (int col = 1; col <= 3; col++)
		{
			T sum = 0;
			for (int k = 1; k <= 3; k++)
			{
				sum += vectorOriginal[k - 1] * aMatrix(k, col);
			}
			aVector[col - 1] = sum;
		}
	}

	template <class T>
	Matrix3x3<T> Matrix3x3<T>::operator=(const Matrix3x3<T>& aMatrix)
	{
		for (int row = 1; row <= 3; row++)
		{
			for (int col = 1; col <= 3; col++)
			{
				(*this)(row, col) = aMatrix(row, col);
			}
		}
		return aMatrix;
	}

	template <class T>
	bool operator==(const Matrix3x3<T>& aMatrix0, const Matrix3x3<T>& aMatrix1)
	{
		for (int row = 1; row <= 3; row++)
		{
			for (int col = 1; col <= 3; col++)
			{
				if (aMatrix0(row, col) != aMatrix1(row, col))
				{
					return false;
				}
			}
		}

		return true;
	}

	template <class T>
	T Matrix3x3<T>::Minor(const int aX, const int aY) const
	{
		Matrix2x2<T> result;

		for (int outY = 1; outY <= 2; outY++)
		{
			int inY = outY;
			if (outY >= aY)
			{
				inY++;
			}
			for (int outX = 1; outX <= 2; outX++)
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
	T Matrix3x3<T>::Cofactor(const int aX, const int aY) const
	{
		T result = Minor(aX, aY);

		if ((((aX - 1) ^ (aY - 1)) & 1) > 0)
		{
			result = -result;
		}

		return result;
	}

	template <class T>
	T Matrix3x3<T>::Determinant() const
	{
		T result = 0;

		for (int i = 1; i <= 3; i++)
		{
			result += (*this)(1, i) * Cofactor(i, 1);
		}

		return result;
	}

	template <class T>
	Matrix3x3<T> Matrix3x3<T>::Cofactors() const
	{
		Matrix3x3<T> result;

		for (unsigned int y = 1; y <= 3; y++)
		{
			for (unsigned int x = 1; x <= 3; x++)
			{
				result(x, y) = Cofactor(x, y);
			}
		}

		return result;
	}

	template <class T>
	Matrix3x3<T> Matrix3x3<T>::Adjoint() const
	{
		return Matrix3x3<T>::Transpose(Cofactors());
	}

	template <class T>
	Matrix3x3<T> Matrix3x3<T>::Inverse() const
	{
		return Adjoint() * (1 / Determinant());
	}
}

namespace CU = CommonUtilities;
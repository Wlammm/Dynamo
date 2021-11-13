#pragma once

#include <assert.h>

namespace CommonUtilities
{
	template <class T>
	class Vector2
	{
	public:
		T x;
		T y;

		//Creates a null-vector
		Vector2<T>();
		//Creates a vector (aX, aY, aZ)
		Vector2<T>(const T& aX, const T& aY);
		//Copy constructor (compiler generated)
		Vector2<T>(const Vector2<T>& aVector) = default;
		//Assignment operator (compiler generated)
		Vector2<T>& operator=(const Vector2<T>& aVector3) = default;
		//Destructor (compiler generated)
		~Vector2<T>() = default;
		//Returns the squared length of the vector
		T LengthSqr() const;
		//Returns the length of the vector
		T Length() const;
		//Returns a normalized copy of this
		Vector2<T> GetNormalized() const;
		//Normalizes the vector
		void Normalize();
		//Returns the dot product of this and aVector
		T Dot(const Vector2<T>& aVector) const;
		//Returns the cross product of this and aVector
		Vector2<T> Cross(const Vector2<T>& aVector) const;
	};
	//Returns the vector sum of aVector0 and aVector1
	template <class T> Vector2<T> operator+(const Vector2<T>& aVector0, const Vector2<T>&
		aVector1)
	{
		return { aVector0.x + aVector1.x, aVector0.y + aVector1.y };
	}

	//Returns the vector difference of aVector0 and aVector1
	template <class T> Vector2<T> operator-(const Vector2<T>& aVector0, const Vector2<T>&
		aVector1)
	{
		return { aVector0.x - aVector1.x, aVector0.y - aVector1.y };
	}

	//Returns the vector aVector multiplied by the scalar aScalar
	template <class T> Vector2<T> operator*(const Vector2<T>& aVector, const T& aScalar)
	{
		return { aVector.x * aScalar, aVector.y * aScalar };
	}

	//Returns the vector aVector multiplied by the scalar aScalar
	template <class T> Vector2<T> operator*(const T& aScalar, const Vector2<T>& aVector)
	{
		return { aVector.x * aScalar, aVector.y * aScalar };
	}

	//Returns the vector aVector divided by the scalar aScalar (equivalent to aVector multiplied by 1 / aScalar)
	template <class T> Vector2<T> operator/(const Vector2<T>& aVector, const T& aScalar)
	{
		assert(aScalar != 0 && "Divide by 0");
		return { aVector.x / aScalar, aVector.y / aScalar };
	}

	//Equivalent to setting aVector0 to (aVector0 + aVector1)
	template <class T> void operator+=(Vector2<T>& aVector0, const Vector2<T>& aVector1)
	{
		aVector0 = aVector0 + aVector1;
	}

	//Equivalent to setting aVector0 to (aVector0 - aVector1)
	template <class T> void operator-=(Vector2<T>& aVector0, const Vector2<T>& aVector1)
	{
		aVector0 = aVector0 - aVector1;
	}

	//Equivalent to setting aVector to (aVector * aScalar)
	template <class T> void operator*=(Vector2<T>& aVector, const T& aScalar)
	{
		aVector = aVector * aScalar;
	}

	//Equivalent to setting aVector to (aVector / aScalar)
	template <class T> void operator/=(Vector2<T>& aVector, const T& aScalar)
	{
		assert(aScalar != 0 && "Divide by 0");
		aVector = aVector / aScalar;
	}

	template<class T>
	inline Vector2<T>::Vector2()
	{
		x = 0;
		y = 0;
	}

	template<class T>
	inline Vector2<T>::Vector2(const T& aX, const T& aY)
	{
		x = aX;
		y = aY;
	}

	template<class T>
	inline T Vector2<T>::LengthSqr() const
	{
		return x * x + y * y;
	}

	template<class T>
	inline T Vector2<T>::Length() const
	{
		return sqrt(LengthSqr());
	}

	template<class T>
	inline Vector2<T> Vector2<T>::GetNormalized() const
	{
		T length = Length();
		assert(length != 0 && "Divide by 0");

		return { x / length, y / length };
	}

	template<class T>
	inline void Vector2<T>::Normalize()
	{
		T length = Length();
		assert(length != 0 && "Divide by 0");

		x /= length;
		y /= length;
	}

	template<class T>
	inline T Vector2<T>::Dot(const Vector2<T>& aVector) const
	{
		return { x * aVector.x + y * aVector.y };
	}
}

namespace CU = CommonUtilities;
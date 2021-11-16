#pragma once
#include <cmath>
#include <assert.h>
#include "CU.h"

namespace CommonUtilities
{
	namespace CU = CommonUtilities;

	template <class T>
	class Vector4
	{
	public:
		union
		{
			T x;
			T r;
		};
		union
		{
			T y;
			T g;
		};
		union
		{
			T z;
			T b;
		};
		union
		{
			T w;
			T a;
		};

		Vector4<T>();
		Vector4<T>(const T& aX, const T& aY, const T& aZ, const T& aW);
		Vector4<T>(const Vector3<T>& aVector, const T& aW);
		Vector4<T>(const Vector4<T>& aVector) = default;
		Vector4<T>& operator=(const Vector4<T>& aVector4) = default;
		~Vector4<T>() = default;

		Vector4<T> Zero() const;

		T LengthSqr() const;
		T Length() const;

		Vector4<T> GetNormalized() const;
		void Normalize();

		T Dot(const Vector4<T>& aVector) const;

		template<typename Temp>
		Vector4<Temp> Cast() const
		{
			return { (Temp)x, (Temp)y, (Temp)z, (Temp)w };
		}

		T& operator[] (const int aIndex)
		{
			switch (aIndex)
			{
			case 0:
				return x;
			case 1:
				return y;
			case 2:
				return z;
			case 3:
				return w;
			default:
				assert("index out of range");
			}

			return x;
		}

		const T& operator[](const int aIndex) const
		{
			switch (aIndex)
			{
			case 0:
				return x;
			case 1:
				return y;
			case 2:
				return z;
			case 3:
				return w;
			default:
				assert("index out of range");
			}

			return x;
		}
	};

	template <class T>
	Vector4<T> operator+(const Vector4<T>& aVector0, const Vector4<T>& aVector1)
	{
		return Vector4<T>(aVector0.x + aVector1.x, aVector0.y + aVector1.y, aVector0.z + aVector1.z, aVector0.w + aVector1.w);
	}

	template <class T>
	Vector4<T> operator-(const Vector4<T>& aVector0, const Vector4<T>& aVector1)
	{
		return Vector4<T>(aVector0.x - aVector1.x, aVector0.y - aVector1.y, aVector0.z - aVector1.z, aVector0.w - aVector1.w);
	}

	template <class T>
	Vector4<T> operator*(const Vector4<T>& aVector0, const Vector4<T>& aVector1)
	{
		return Vector4<T>(aVector0.x * aVector1.x, aVector0.y * aVector1.y, aVector0.z * aVector1.z, aVector0.w * aVector1.w);
	}

	template <class T>
	Vector4<T> operator*(const Vector4<T>& aVector, const T& aScalar)
	{
		return Vector4<T>(aVector.x * aScalar, aVector.y * aScalar, aVector.z * aScalar, aVector.w * aScalar);
	}

	template <class T>
	Vector4<T> operator*(const T& aScalar, const Vector4<T>& aVector)
	{
		return aVector * aScalar;
	}

	template <class T>
	Vector4<T> operator/(const Vector4<T>& aVector, const T& aScalar)
	{
		assert(aScalar != 0 && "Cannot divide by zero");
		if (aScalar == static_cast<T>(0))
		{
			return aVector;
		}

		T multiplier = static_cast<T>(1) / aScalar;
		return Vector4<T>(aVector.x * multiplier, aVector.y * multiplier, aVector.z * multiplier, aVector.w * multiplier);
	}

	template <class T>
	void operator+=(Vector4<T>& aVector0, const Vector4<T>& aVector1)
	{
		aVector0 = aVector0 + aVector1;
	}

	template <class T>
	void operator-=(Vector4<T>& aVector0, const Vector4<T>& aVector1)
	{
		aVector0 = aVector0 - aVector1;
	}

	template <class T>
	void operator*=(Vector4<T>& aVector, const T& aScalar)
	{
		aVector = aVector * aScalar;
	}

	template <class T>
	void operator/=(Vector4<T>& aVector, const T& aScalar)
	{
		aVector = aVector / aScalar;
	}

	template<class T>
	inline Vector4<T>::Vector4() :
		x(0), y(0), z(0), w(0)
	{
	}

	template<class T>
	inline Vector4<T>::Vector4(const T& aX, const T& aY, const T& aZ, const T& aW) :
		x(aX), y(aY), z(aZ), w(aW)
	{
	}

	template<class T>
	inline Vector4<T>::Vector4(const Vector3<T>& aVector, const T& aW)
	{
		x = aVector.x;
		y = aVector.y;
		z = aVector.z;
		w = aW;
	}

	template<class T>
	inline Vector4<T> Vector4<T>::Zero() const
	{
		return Vector4<T>();
	}

	template<class T>
	inline T Vector4<T>::LengthSqr() const
	{
		return x * x + y * y + z * z + w * w;
	}

	template<class T>
	inline T Vector4<T>::Length() const
	{
		return static_cast<T>(sqrt(LengthSqr()));
	}

	template<class T>
	inline Vector4<T> Vector4<T>::GetNormalized() const
	{
		T length = Length();
		if (length == 0)
		{
			return *this;
		}

		return *this / length;
	}

	template<class T>
	inline void Vector4<T>::Normalize()
	{
		*this = GetNormalized();
	}

	template<class T>
	inline T Vector4<T>::Dot(const Vector4<T>& aVector) const
	{
		return x * aVector.x + y * aVector.y + z * aVector.z + w * aVector.w;
	}

	using Color = Vector4<float>;
	using Vector4f = Vector4<float>;
	using Vector4i = Vector4<int>;
	using Vector4ui = Vector4<unsigned>;
}
namespace CU = CommonUtilities;

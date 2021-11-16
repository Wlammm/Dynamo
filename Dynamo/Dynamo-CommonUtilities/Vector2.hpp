#pragma once
#include <cmath>
#include <assert.h>

namespace CommonUtilities
{
	template <class T>
	class Vector2
	{
	public:
		union
		{
			T x;
			T myX;
		};
		union
		{
			T y;
			T myY;
		};

		Vector2<T>();
		Vector2<T>(const T& aX, const T& aY);
		Vector2<T>(const Vector2<T>& aVector) = default;
		~Vector2<T>() = default;

		Vector2<T> Zero() const;

		T LengthSqr() const;
		T Length() const;

		Vector2<T> GetNormalized() const;
		void Normalize();

		Vector2<T> Normal() const { return Vector2<T>(y, -x); }

		T Dot(const Vector2<T>& aVector) const;

		template <class OtherT>
		Vector2<T>& operator=(const Vector2<OtherT>& aVector) { x = static_cast<T>(aVector.x); y = static_cast<T>(aVector.y); return *this; }
		template<class OtherT>
		Vector2<OtherT> Cast() const { return Vector2<OtherT>{static_cast<OtherT>(x), static_cast<OtherT>(y)}; }
		Vector2<T>& operator=(const Vector2<T>& aVector2) = default;
		bool operator==(const Vector2<T>& aVector) const { return (x == aVector.x && y == aVector.y); }
		bool operator!=(const Vector2<T>& aVector) const { return !(*this == aVector); }
		void Set(T aT, T aBT) { x = aT; y = aBT; }

		void Rotate(const T anAngleInRadians);

		bool IsWithin(const Vector2<T>& aVector, float aTolerance);
	};

	template <class T>
	inline Vector2<T> operator+(const Vector2<T>& aVector0, const Vector2<T>& aVector1)
	{
		return Vector2<T>(aVector0.x + aVector1.x, aVector0.y + aVector1.y);
	}

	template <class T>
	inline Vector2<T> operator-(const Vector2<T>& aVector0, const Vector2<T>& aVector1)
	{
		return Vector2<T>(aVector0.x - aVector1.x, aVector0.y - aVector1.y);
	}

	template <class T>
	inline Vector2<T> operator*(const Vector2<T>& aVector, const T& aScalar)
	{
		return Vector2<T>(aVector.x * aScalar, aVector.y * aScalar);
	}

	template <class T>
	inline Vector2<T> operator*(const T& aScalar, const Vector2<T>& aVector)
	{
		return aVector * aScalar;
	}

	template <class T>
	Vector2<T> operator/(const Vector2<T>& aVector, const T& aScalar)
	{
		assert(aScalar != 0 && "Cannot divide by zero");
		if (aScalar == static_cast<T>(0))
		{
			return aVector;
		}
		return Vector2<T>(aVector.x / aScalar, aVector.y / aScalar);
	}

	template <class T>
	inline void operator/=(Vector2<T>& aVector, const T& aScalar)
	{
		aVector = aVector / aScalar;
	}

	template <class T>
	inline Vector2<T> operator*(const Vector2<T> aVector, const Vector2<T> anotherVector)
	{
		return Vector2<T>{aVector.x* anotherVector.x, aVector.y* anotherVector.y};
	}

	template <class T>
	inline void operator*=(Vector2<T> aVector, const Vector2<T> anotherVector)
	{
		aVector = aVector * anotherVector;
	}

	template <class T>
	inline Vector2<T> operator/(const Vector2<T> aVector, const Vector2<T> anotherVector)
	{
		return Vector2<T>{aVector.x / anotherVector.x, aVector.y / anotherVector.y};
	}

	template <class T>
	inline void operator/=(Vector2<T> aVector, const Vector2<T> anotherVector)
	{
		aVector = aVector / anotherVector;
	}

	template <class T>
	inline void operator+=(Vector2<T>& aVector0, const Vector2<T>& aVector1)
	{
		aVector0 = aVector0 + aVector1;
	}

	template <class T>
	inline void operator-=(Vector2<T>& aVector0, const Vector2<T>& aVector1)
	{
		aVector0 = aVector0 - aVector1;
	}

	template <class T>
	inline void operator*=(Vector2<T>& aVector, const T& aScalar)
	{
		aVector = aVector * aScalar;
	}



	template<class T>
	inline Vector2<T>::Vector2() :
		x(0), y(0)
	{
	}


	template<class T>
	inline Vector2<T>::Vector2(const T& aX, const T& aY) :
		x(aX), y(aY)
	{
	}

	template<class T>
	inline Vector2<T> Vector2<T>::Zero() const
	{
		return Vector2<T>();
	}

	template<class T>
	inline T Vector2<T>::LengthSqr() const
	{
		return x * x + y * y;
	}

	template<class T>
	inline T Vector2<T>::Length() const
	{
		return static_cast<T>(sqrt(LengthSqr()));
	}

	template<class T>
	inline Vector2<T> Vector2<T>::GetNormalized() const
	{
		T length = Length();
		if (length == 0)
		{
			return *this;
		}

		return *this / length;
	}

	template<class T>
	inline void Vector2<T>::Normalize()
	{
		*this = GetNormalized();
	}

	template<class T>
	inline T Vector2<T>::Dot(const Vector2<T>& aVector) const
	{
		return x * aVector.x + y * aVector.y;
	}

	template<class T>
	inline void Vector2<T>::Rotate(const T anAngleInRadians)
	{
		T cosine = cos(anAngleInRadians);
		T sine = sin(anAngleInRadians);

		operator=(Vector2<T>{ cosine* x - sine * y, sine* x + cosine * y });
	}
	template<class T>
	inline bool Vector2<T>::IsWithin(const Vector2<T>& aVector, float aTolerance)
	{
		return (operator-(*this, aVector)).LengthSqr() <= aTolerance * aTolerance;
	}

	using Vector2f = CommonUtilities::Vector2<float>;
	using Vector2i = CommonUtilities::Vector2<int>;
	using Vector2ui = CommonUtilities::Vector2<unsigned>;
}
namespace CU = CommonUtilities;
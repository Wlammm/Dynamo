#pragma once
#include "Vector3.hpp"

namespace CommonUtilities
{
	template<typename T>
	class Sphere
	{
	public:
		// Default constructor: there is no sphere, the radius is zero and the position is
		// the zero vector.
		Sphere();

		// Copy constructor.
		Sphere(const Sphere<T>& aSphere);

		// Constructor that takes the center position and radius of the sphere.
		Sphere(const Vector3<T>& aCenter, T aRadius);

		// Init the sphere with a center and a radius, the same as the constructor above.
		void InitWithCenterAndRadius(const Vector3<T>& aCenter, T aRadius);

		// Returns whether a point is inside the sphere: it is inside when the point is on the
		// sphere surface or inside of the sphere.
		bool IsInside(const Vector3<T>& aPosition) const;

		const T& GetRadiusSqr() const;
		const Vector3<T>& GetCenter() const;

	private:
		Vector3<T> myPosition;
		T myRadius;

	};

	template<typename T>
	inline Sphere<T>::Sphere()
	{
		myPosition = { 0, 0, 0 };
		myRadius = 0;
	}

	template<typename T>
	inline Sphere<T>::Sphere(const Sphere<T>& aSphere)
	{
		myPosition = aSphere.myPosition;
		myRadius = aSphere.myRadius;
	}

	template<typename T>
	inline Sphere<T>::Sphere(const Vector3<T>& aCenter, T aRadius)
	{
		myPosition = aCenter;
		myRadius = aRadius;
	}

	template<typename T>
	inline void Sphere<T>::InitWithCenterAndRadius(const Vector3<T>& aCenter, T aRadius)
	{
		myPosition = aCenter;
		myRadius = aRadius;
	}

	template<typename T>
	inline bool Sphere<T>::IsInside(const Vector3<T>& aPosition) const
	{
		Vector3<T> delta = myPosition - aPosition;
		T distance = delta.Length();

		return (distance < myRadius);
	}
	template<typename T>
	inline const T& Sphere<T>::GetRadiusSqr() const
	{
		return myRadius * myRadius;
	}
	template<typename T>
	inline const Vector3<T>& Sphere<T>::GetCenter() const
	{
		return myPosition;
	}
}

namespace CU = CommonUtilities;
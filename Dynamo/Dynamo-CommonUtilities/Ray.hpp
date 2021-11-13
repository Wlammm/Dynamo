#pragma once

#include "Vector3.hpp"

namespace CommonUtilities
{
	template<typename T>
	class Ray
	{
	public:
		Ray();

		Ray(const Ray<T>& aRay);

		Ray(const Vector3<T>& aOrigin, const Vector3<T>& aDirection);

		// Init the ray with origin and a point.
		void InitWithOriginAndPoint(const Vector3<T>& aOrigin, const Vector3<T>& aPoint);

		const Vector3<T>& GetOrigin() const;
		const Vector3<T>& GetDirection() const;

	private:
		Vector3<T> myOrigin;
		Vector3<T> myDirection;

	};

	template<typename T>
	inline Ray<T>::Ray()
	{
		myOrigin = { 0, 0, 0 };
		myDirection = { 0, 0, 0 };
	}

	template<typename T>
	inline Ray<T>::Ray(const Ray<T>& aRay)
	{
		myOrigin = aRay.myOrigin;
		myDirection = aRay.myDirection;
	}

	template<typename T>
	inline Ray<T>::Ray(const Vector3<T>& aOrigin, const Vector3<T>& aDirection)
	{
		myOrigin = aOrigin;
		myDirection = aDirection;
	}

	template<typename T>
	inline void Ray<T>::InitWithOriginAndPoint(const Vector3<T>& aOrigin, const Vector3<T>& aPoint)
	{
		myOrigin = aOrigin;
		myDirection = aPoint - aOrigin;
		myDirection.Normalize();
	}
	template<typename T>
	inline const Vector3<T>& Ray<T>::GetOrigin() const
	{
		return myOrigin;
	}
	template<typename T>
	inline const Vector3<T>& Ray<T>::GetDirection() const
	{
		return myDirection;
	}
}

namespace CU = CommonUtilities;
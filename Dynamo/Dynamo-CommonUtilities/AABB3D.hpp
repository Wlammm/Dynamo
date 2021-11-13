#pragma once

#include "Vector3.hpp"

namespace CommonUtilities
{
	template<typename T>
	class AABB3D
	{
	public:
		// Default constructor: there is no AABB, both min and max points are the zero vector.
		AABB3D();

		// Copy constructor.
		AABB3D(const AABB3D<T>& aAABB3D);

		// Constructor taking the positions of the minimum and maximum corners.
		AABB3D(const Vector3<T>& aMin, const Vector3<T>& aMax);

		// Init the AABB with the positions of the minimum and maximum corners, same as
		// the constructor above.
		void InitWithMinAndMax(const Vector3<T>& aMin, const Vector3<T>& aMax);

		// Returns whether a point is inside the AABB: it is inside when the point is on any
		// of the AABB's sides or inside of the AABB.
		bool IsInside(const Vector3<T>& aPosition) const;

		Vector3<T> myMinPoint;
		Vector3<T> myMaxPoint;
	};

	template<typename T>
	inline AABB3D<T>::AABB3D()
	{
		myMinPoint = { 0, 0, 0 };
		myMaxPoint = { 0, 0, 0 };
	}

	template<typename T>
	inline AABB3D<T>::AABB3D(const AABB3D<T>& aAABB3D)
	{
		myMinPoint = aAABB3D.myMinPoint;
		myMaxPoint = aAABB3D.myMaxPoint;
	}

	template<typename T>
	inline AABB3D<T>::AABB3D(const Vector3<T>& aMin, const Vector3<T>& aMax)
	{
		myMinPoint = aMin;
		myMaxPoint = aMax;
	}

	template<typename T>
	inline void AABB3D<T>::InitWithMinAndMax(const Vector3<T>& aMin, const Vector3<T>& aMax)
	{
		myMinPoint = aMin;
		myMaxPoint = aMax;
	}

	template<typename T>
	inline bool AABB3D<T>::IsInside(const Vector3<T>& aPosition) const
	{
		return  (aPosition.x >= myMinPoint.x && aPosition.x <= myMaxPoint.x) &&
				(aPosition.y >= myMinPoint.y && aPosition.y <= myMaxPoint.y) &&
				(aPosition.z >= myMinPoint.z && aPosition.z <= myMaxPoint.z);
	}
}

namespace CU = CommonUtilities;
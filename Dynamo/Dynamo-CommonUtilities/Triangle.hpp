#pragma once

#include "Vector3.hpp"
#include "Line.hpp"

namespace CommonUtilities
{
	template<typename T>
	class Triangle
	{
	public:
		Triangle()
		{
			myPoint1 = { 1, 0, 0 };
			myPoint2 = { 0, 1, 0 };
			myPoint3 = { 0, 0, 1 };
		}

		Triangle(const Triangle& aTriangle)
		{
			myPoint1 = aTriangle.myPoint1;
			myPoint2 = aTriangle.myPoint2;
			myPoint3 = aTriangle.myPoint3;
		}

		Triangle(const Vector3<T>& point1, const Vector3<T>& point2, const Vector3<T>& point3)
		{
			myPoint1 = point1;
			myPoint2 = point2;
			myPoint3 = point3;
		}

		const Vector3<T>& GetPoint1() const { return myPoint1; }
		const Vector3<T>& GetPoint2() const { return myPoint2; }
		const Vector3<T>& GetPoint3() const { return myPoint3; }

	private:
		Vector3<T> myPoint1;
		Vector3<T> myPoint2;
		Vector3<T> myPoint3;
	};
}

namespace CU = CommonUtilities;
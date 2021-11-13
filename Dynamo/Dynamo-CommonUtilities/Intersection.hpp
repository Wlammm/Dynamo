#pragma once

#include "Plane.hpp"
#include "AABB3D.hpp"
#include "Sphere.hpp"
#include "Ray.hpp"
#include "Vector3.hpp"
#include "Triangle.hpp"
#include "Maths.hpp"
#include <array>

namespace CommonUtilities
{
	template<typename T>
	bool IntersectionPlaneRay(const Plane<T>& aPlane, const Ray<T>& aRay, Vector3<T>& aOutIntersectionPoint)
	{
		const T dnDot = aRay.GetDirection().Dot(aPlane.GetNormal());

		bool isInside = aPlane.IsInside(aRay.GetOrigin());
		if ((isInside && dnDot <= 0) || (!isInside && dnDot >= 0))
		{
			return false; // Parallel
		}

		T planeD = aPlane.GetPoint().Dot(aPlane.GetNormal());
		T length = (planeD - aRay.GetOrigin().Dot(aPlane.GetNormal())) / dnDot;

		aOutIntersectionPoint = aRay.GetOrigin() + aRay.GetDirection() * length;
		return true;
	}

	template<typename T>
	bool IntersectionAABBRay(const AABB3D<T>& aAABB, const Ray<T>& aRay)
	{
		Plane<T> planes[6];
		// Right
		planes[0] = Plane<T>(
			Vector3<T>(aAABB.myMaxPoint.x, aAABB.myMaxPoint.y, aAABB.myMaxPoint.z),
			Vector3<T>(aAABB.myMaxPoint.x, aAABB.myMinPoint.y, aAABB.myMaxPoint.z),
			Vector3<T>(aAABB.myMaxPoint.x, aAABB.myMinPoint.y, aAABB.myMinPoint.z));
		// Front
		planes[1] = Plane<T>(
			Vector3<T>(aAABB.myMaxPoint.x, aAABB.myMaxPoint.y, aAABB.myMinPoint.z),
			Vector3<T>(aAABB.myMaxPoint.x, aAABB.myMinPoint.y, aAABB.myMinPoint.z),
			Vector3<T>(aAABB.myMinPoint.x, aAABB.myMinPoint.y, aAABB.myMinPoint.z));
		// Left
		planes[2] = Plane<T>(
			Vector3<T>(aAABB.myMinPoint.x, aAABB.myMaxPoint.y, aAABB.myMinPoint.z),
			Vector3<T>(aAABB.myMinPoint.x, aAABB.myMinPoint.y, aAABB.myMinPoint.z),
			Vector3<T>(aAABB.myMinPoint.x, aAABB.myMinPoint.y, aAABB.myMaxPoint.z));
		// Back
		planes[3] = Plane<T>(
			Vector3<T>(aAABB.myMinPoint.x, aAABB.myMaxPoint.y, aAABB.myMaxPoint.z),
			Vector3<T>(aAABB.myMinPoint.x, aAABB.myMinPoint.y, aAABB.myMaxPoint.z),
			Vector3<T>(aAABB.myMaxPoint.x, aAABB.myMinPoint.y, aAABB.myMaxPoint.z));
		// Up
		planes[4] = Plane<T>(
			Vector3<T>(aAABB.myMaxPoint.x, aAABB.myMaxPoint.y, aAABB.myMaxPoint.z),
			Vector3<T>(aAABB.myMaxPoint.x, aAABB.myMaxPoint.y, aAABB.myMinPoint.z),
			Vector3<T>(aAABB.myMinPoint.x, aAABB.myMaxPoint.y, aAABB.myMinPoint.z));
		// Down
		planes[5] = Plane<T>(
			Vector3<T>(aAABB.myMaxPoint.x, aAABB.myMinPoint.y, aAABB.myMaxPoint.z),
			Vector3<T>(aAABB.myMinPoint.x, aAABB.myMinPoint.y, aAABB.myMaxPoint.z),
			Vector3<T>(aAABB.myMinPoint.x, aAABB.myMinPoint.y, aAABB.myMinPoint.z));


		for (int i = 0; i < 6; i++)
		{
			const T dnDot = aRay.GetDirection().Dot(planes[i].GetNormal());
			bool isInside = planes[i].IsInside(aRay.GetOrigin());

			if ((isInside && dnDot <= 0) || (!isInside && dnDot >= 0))
			{
				continue; // Parallel or wrong side
			}

			T planeD = planes[i].GetPoint().Dot(planes[i].GetNormal());
			T length = (planeD - aRay.GetOrigin().Dot(planes[i].GetNormal())) / dnDot;

			CU::Vector3<T> hit = aRay.GetOrigin() + aRay.GetDirection() * length;

			bool hitSucceed = true;
			for (int x = 0; x < 6; x++)
			{
				if (x != i)
				{
					if (!planes[x].IsInside(hit))
					{
						hitSucceed = false;
						break;
					}
				}
			}
			if (hitSucceed)
			{
				return true;
			}
		}

		return false;
	}

	template<typename T>
	bool IntersectionSphereRay(const Sphere<T>& aSphere, const Ray<T>& aRay)
	{
		Vector3<T> e = aSphere.GetCenter() - aRay.GetOrigin();

		T a = e.Dot(aRay.GetDirection().GetNormalized());
		T eSqr = e.LengthSqr();
		T rSqr = aSphere.GetRadiusSqr();
		T aSqr = a * a;
		T fSqr = rSqr - (eSqr + aSqr);
		if (rSqr - (eSqr - aSqr) < 0.0f)
		{
			return false; // INVALID
		}
		if (eSqr < aSqr)
		{
			return true; // INSIDE
		}
		return true;
	}

	template<typename T>
	bool IntersectionTriangleRay(const Triangle<T>& aTriangle, const Ray<T>& aRay, const CU::Vector3f& outVector)
	{
		const float EPSILON = 0.0000001f;

		Vector3f vertex0 = aTriangle.GetPoint1();
		Vector3f vertex1 = aTriangle.GetPoint2();;
		Vector3f vertex2 = aTriangle.GetPoint3();
		Vector3f edge1, edge2, h, s, q;
		float a, f, u, v;

		edge1 = vertex1 - vertex0;
		edge2 = vertex2 - vertex0;

		h = aRay.GetDirection().Cross(edge2);
		a = edge1.Dot(h);

		if (a > -EPSILON && a < EPSILON)
			return false;

		f = 1.0f / a;
		s = aRay.GetOrigin() - vertex0;
		u = f * s.Dot(h);

		if (u < 0.0f || u > 1.0f)
			return false;

		q = s.Cross(edge1);
		v = f * aRay.GetDirection().Dot(q);

		if (v < 0.0f || u + v > 1.0f)
			return false;

		float t = f * edge2.Dot(q);
		if (t > EPSILON)
		{
			outVector = aRay.GetOrigin() + aRay.GetDirection() * t;
			return true;
		}
		else
			return false;
	}

	template<typename T>
	bool IntersectionTriangleRay(const std::array<Vector3<T>, 3>& aTriangleVertices, const Ray<T>& aRay, CU::Vector3f& outVector)
	{
		const float EPSILON = 0.0000001f;

		Vector3f vertex0 = aTriangleVertices[0];
		Vector3f vertex1 = aTriangleVertices[1];
		Vector3f vertex2 = aTriangleVertices[2];
		Vector3f edge1, edge2, h, s, q;
		float a, f, u, v;

		edge1 = vertex1 - vertex0;
		edge2 = vertex2 - vertex0;

		h = aRay.GetDirection().Cross(edge2);
		a = edge1.Dot(h);

		if (a > -EPSILON && a < EPSILON)
			return false;

		f = 1.0f / a;
		s = aRay.GetOrigin() - vertex0;
		u = f * s.Dot(h);

		if (u < 0.0f || u > 1.0f)
			return false;

		q = s.Cross(edge1);
		v = f * aRay.GetDirection().Dot(q);

		if (v < 0.0f || u + v > 1.0f)
			return false;

		float t = f * edge2.Dot(q);
		if (t > EPSILON)
		{
			outVector = aRay.GetOrigin() + aRay.GetDirection() * t;
			return true;
		}
		else
			return false;
	}
}

namespace CU = CommonUtilities;
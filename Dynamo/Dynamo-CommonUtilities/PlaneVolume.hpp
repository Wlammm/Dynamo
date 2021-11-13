#pragma once
#include <vector>
#include "Plane.hpp"

namespace CommonUtilities
{
	template <class T>
	class PlaneVolume
	{
	public:
		// Default constructor: empty PlaneVolume.
		PlaneVolume() = default;
		// Constructor taking a list of Plane that makes up the PlaneVolume.
		PlaneVolume(const std::vector<Plane<T>>& aPlaneList);
		// Add a Plane to the PlaneVolume.
		void AddPlane(const Plane<T>& aPlane);
		// Returns whether a point is inside the PlaneVolume: it is inside when the point is on the
		// plane or on the side the normal is pointing away from for all the planes in thePlaneVolume.
		bool IsInside(const Vector3<T>& aPosition);
	private:
		std::vector<Plane<T>> myPlaneVolume;
	};
	template<class T>
	inline PlaneVolume<T>::PlaneVolume(const std::vector<Plane<T>>& aPlaneList)
	{
		myPlaneVolume = aPlaneList;
	}
	template<class T>
	inline void PlaneVolume<T>::AddPlane(const Plane<T>& aPlane)
	{
		myPlaneVolume.push_back(aPlane);
	}
	template<class T>
	inline bool PlaneVolume<T>::IsInside(const Vector3<T>& aPosition)
	{
		for (size_t i = 0; i < myPlaneVolume.size(); i++)
		{
			if (myPlaneVolume[i].IsInside(aPosition))
			{
				return false;
			}
		}
		return true;
	}
}
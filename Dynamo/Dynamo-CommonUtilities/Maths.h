#pragma once
#include <assert.h>

namespace CommonUtilities
{
	static const float PI = atanf(1.f) * 4.f;

	template <typename T>
	T DegreesToRadians(T someDegrees);

	template <typename T>
	T RadiansToDegrees(T someRadians);

	template<typename T>
	T DegreesToRadians(T someDegrees)
	{
		return someDegrees * (static_cast<T>(PI) / static_cast<T>(180.0));
	}
	template<typename T>
	T RadiansToDegrees(T someRadians)
	{
		return someRadians * (static_cast<T>(180.0) / static_cast<T>(PI));
	}

	template<typename T>
	T Max(const T& aFirst, const T& aSecond);

	template<typename T>
	T Min(const T& aFirst, const T& aSecond);

	template<typename T>
	T Abs(const T& aFirst);

	template<typename T>
	T Clamp(const T& aMin, const T& aMax, const T& someValue);

	template<typename T>
	T Lerp(T aFirst, const T& aSecond, const float& someTime);

	template<typename T>
	void Swap(T& aFirst, T& aSecond);

	// Definitions.
	template<typename T>
	T Max(const T& aFirst, const T& aSecond)
	{
		return aFirst > aSecond ? aFirst : aSecond;
	}

	template<typename T>
	T Min(const T& aFirst, const T& aSecond)
	{
		return aFirst < aSecond ? aFirst : aSecond;
	}

	template<typename T>
	T Abs(const T& aFirst)
	{
		return aFirst > 0 ? aFirst : -aFirst;
	}

	template<typename T>
	T Clamp(const T& aMin, const T& aMax, const T& someValue)
	{
		if (aMin > aMax)
		{
			return someValue < aMax ? aMin : someValue > aMin ? aMax : someValue;
		}

		return someValue < aMin ? aMin : someValue > aMax ? aMax : someValue;
	}

	template<typename T>
	T Lerp(T aFirst, const T& aSecond, const float& someTime)
	{
		return (1 - someTime) * aFirst + someTime * aSecond;
	}

	template<typename T>
	void Swap(T& aFirst, T& aSecond)
	{
		T first = aFirst;

		aFirst = aSecond;
		aSecond = first;
	}

	template<typename T>
	bool AreEqual(T a, T b, T aTolerance)
	{
		return Abs(a - b) <= aTolerance;
	}
}

namespace CU = CommonUtilities;
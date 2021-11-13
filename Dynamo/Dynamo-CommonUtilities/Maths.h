#pragma once
#include <assert.h>

namespace CommonUtilities
{
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
}

namespace CU = CommonUtilities;
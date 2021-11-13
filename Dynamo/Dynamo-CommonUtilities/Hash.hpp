#pragma once
#include <cstdint>
#include <string>

namespace CommonUtilities
{
	uint32_t Hash(const uint8_t* aBuffer, int aCount)
	{
		const uint32_t FNVOffsetBasis = 2166136261U;
		const uint32_t FNVPrime = 16777619U;
		uint32_t val = FNVOffsetBasis;

		for(int i = 0; i < aCount; ++i)
		{
			val ^= aBuffer[i];
			val *= FNVPrime;
		}

		return val;
	}

	uint32_t Hash(int aInt)
	{
		return Hash(reinterpret_cast<const uint8_t*>(&aInt), sizeof(int));
	}

	uint32_t Hash(const std::string& aString)
	{
		return Hash(reinterpret_cast<const uint8_t*>(aString.c_str()), aString.size());
	}
}

namespace CU = CommonUtilities;
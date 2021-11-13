#pragma once
#include <string>
#include <array>

namespace CommonUtilities
{
	bool IsSame(const std::string& aFirst, const std::string& aSecond, int aLength);
	std::array<int, 256> Preprocess(const std::string& aPattern);
	
	int Horspool(const std::string& aNeedle, const std::string& aHaystack)
	{
		if (aNeedle.length() == 0)
			return -1;

		std::array<int, 256> table = Preprocess(aNeedle);

		int skip = 0;

		while(aHaystack.length() - skip >= aNeedle.length())
		{
			std::string subString = aHaystack.substr(skip, aNeedle.length()); // substring form skip + forward
			if(IsSame(subString, aNeedle, aNeedle.length()))
			{
				return skip;
			}

			skip = skip + table[aHaystack[skip + aNeedle.length() - 1]];
		}

		return -1;
	}

	bool IsSame(const std::string& aFirst, const std::string& aSecond, int aLength)
	{
		int i = aLength - 1;

		while(aFirst[i] == aSecond[i])
		{
			if(i == 0)
			{
				return true;
			}

			i = i - 1;
		}

		return false;
	}

	std::array<int, 256> Preprocess(const std::string& aPattern)
	{
		std::array<int, 256> table;

		for(int i = 0; i < 256; ++i)
		{
			table[i] = aPattern.length();
		}

		for(int i = 0; i < aPattern.length() - 1; ++i)
		{
			table[aPattern[i]] = aPattern.length() - 1 - i;
		}

		return table;
	}
}

namespace CU = CommonUtilities;
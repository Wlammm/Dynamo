#pragma once

#include "CU.h"
#include <string>

namespace CommonUtilities
{
	namespace StringUtils
	{
		std::string ToLower(std::string s)
		{
			for (int i = 0; i < s.size(); ++i)
			{
				if (s[i] >= 'A' && s[i] <= 'Z')
				{
					s[i] += 32;
				}
			}

			return s;
		}
	}
}
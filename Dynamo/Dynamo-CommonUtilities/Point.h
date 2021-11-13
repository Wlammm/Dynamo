#pragma once 

#include <string>
#include <sstream>

namespace CommonUtilities
{
	class Point
	{
	public:
		Point(const int& someX, const int& someY);

		const int& GetX() const;
		const int& GetY() const;

		void SetX(const int& someX);
		void SetY(const int& someY);

		std::string ToString() const;

	private:
		int myX;
		int myY;
	};
}

namespace CU = CommonUtilities;
#include "pch.h"
#include "Point.h"

CommonUtilities::Point::Point(const int& someX, const int& someY)
	: myX(someX), myY(someY)
{

}

const int& CommonUtilities::Point::GetX() const
{
	return myX;
}

const int& CommonUtilities::Point::GetY() const
{
	return myY;
}

void CommonUtilities::Point::SetX(const int& someX)
{
	myX = someX;
}

void CommonUtilities::Point::SetY(const int& someY)
{
	myY = someY;
}

std::string CommonUtilities::Point::ToString() const
{
	std::stringstream ss;
	ss << "X: " << myX << " Y: " << myY;
	
	return ss.str();
}

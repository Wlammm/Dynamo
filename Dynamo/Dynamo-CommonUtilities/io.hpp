#pragma once

#include "Vector.hpp"
#include "Matrix.hpp"
#include "Quaternion.hpp"

template<typename T>
inline std::ostream& operator<<(std::ostream& os, const CU::Vector2<T>& aVector)
{
	os << "(" << aVector.x << ", " << aVector.y << ")";
	return os;
}

template<typename T>
inline std::ostream& operator<<(std::ostream& os, const CU::Vector3<T>& aVector)
{
	os << "(" << aVector.x << ", " << aVector.y << ", " << aVector.z << ")";
	return os;
}

template<typename T>
inline std::ostream& operator<<(std::ostream& os, const CU::Vector4<T>& aVector)
{
	os << "(" << aVector.x << ", " << aVector.y << ", " << aVector.z << ", " << aVector.w << ")";
	return os;
}

template<typename T>
inline std::ostream& operator<<(std::ostream& os, const CU::Matrix2x2<T>& aMatrix)
{
	os  << "(" << aMatrix(1, 1) << ", " << aMatrix(1, 2) << 
		 " | " << aMatrix(2, 1) << ", " << aMatrix(2, 2) << ")";
	return os;
}

template<typename T>
inline std::ostream& operator<<(std::ostream& os, const CU::Matrix3x3<T>& aMatrix)
{
	os << "("  << aMatrix(1, 1) << ", " << aMatrix(1, 2) << ", " << aMatrix(1, 3) << 
		 " | " << aMatrix(2, 1) << ", " << aMatrix(2, 2) << ", " << aMatrix(2, 3) <<
		 " | " << aMatrix(3, 1) << ", " << aMatrix(3, 2) << ", " << aMatrix(3, 3) << ")";
	return os;
}

template<typename T>
inline std::ostream& operator<<(std::ostream& os, const CU::Matrix4x4<T>& aMatrix)
{
	os << "("  << aMatrix(1, 1) << ", " << aMatrix(1, 2) << ", " << aMatrix(1, 3) << ", " << aMatrix(1, 4) << 
		 " | " << aMatrix(2, 1) << ", " << aMatrix(2, 2) << ", " << aMatrix(2, 3) << ", " << aMatrix(2, 4) << 
		 " | " << aMatrix(3, 1) << ", " << aMatrix(3, 2) << ", " << aMatrix(3, 3) << ", " << aMatrix(3, 4) << 
		 " | " << aMatrix(4, 1) << ", " << aMatrix(4, 2) << ", " << aMatrix(4, 3) << ", " << aMatrix(4, 4) << ")";
	return os;
}

template<typename T>
inline std::ostream& operator<<(std::ostream& os, const CU::Quaternion<T>& aQuaternion)
{
	os << "(" << aQuaternion[0] << ", " << aQuaternion[1] << ", " << aQuaternion[2] << ", " << aQuaternion[3] << ")";
	return os;
}
#pragma once
#include "Maths.h"
#include "Vector3.hpp"
#include "Matrix4x4.hpp"

namespace CommonUtilities
{
	template<typename T>
	class QuaternionT
	{
	public:
		QuaternionT<T>(); //Creates an Identity Quaternion
		QuaternionT<T>(const QuaternionT<T>& anOther); //Copy constructor
		QuaternionT<T>(const CU::Vector3<T>& aEulerAngles); //Create a to-object-space quaternion from euler angles
		QuaternionT<T>(const CU::Matrix4x4<T>& aRotationMatrix); //Create a rotation quaternion from a rotation matrix
		QuaternionT<T>(T aW, const CU::Vector3<T>& aVector); //Set raw quaternion values. Not recommended unless you know quaternions very well.

		void InitWithAxisAndRotation(const Vector3<T>& anAxis, T aRotation);
		void Normalize();
		QuaternionT<T> GetNormalized() const;

		QuaternionT<T> GetInverse() const;
		T GetMagnitude() const;
		QuaternionT<T> GetConjugate() const;
		T Dot(QuaternionT<T> anOther) const;

		CU::Matrix4x4<T> ToMatrix() const;
		CU::Vector3<T> EulerAngles() const;

		static QuaternionT<T> Slerp(const QuaternionT<T>& aFirst, QuaternionT<T> aSecond, T anInterpolation);
		inline static QuaternionT<T> Identity() { return QuaternionT<T>(); };

	public:
		T myW;
		CU::Vector3<T> myVector;
	};

	typedef QuaternionT<float> Quaternion;

	//Logically add two quaternions. If you are doing this, you are probably making your rotation quaternion invalid. Use multiplication to combine two rotations.
	template<typename T>
	QuaternionT<T> operator+(const QuaternionT<T>& aFirst, const QuaternionT<T>& aSecond);

	//Combine two rotation quaternions into a single rotation. This must be done in reverse order to achieve the desired result.
	//In other words, multiplying a vector with quaternion q1 and then q2, is equivalent to multiplying with the single quaternion q2*q1.
	template <typename T>
	QuaternionT<T> operator*(const QuaternionT<T>& aFirst, const QuaternionT<T>& aSecond);

	template<typename T>
	QuaternionT<T> operator/(const QuaternionT<T>& aFirst, const QuaternionT<T>& aSecond);

	//Rotate a vector by the rotation defined by the quaternion
	template<typename T>
	Vector3<T> operator* (const Vector3<T>& aVector, const QuaternionT<T>& aQuaternion);

	template<typename T>
	QuaternionT<T> operator*(const QuaternionT<T>& aQuaternion, T aScalar);

	template<typename T>
	QuaternionT<T> operator/(const QuaternionT<T>& aQuaternion, T aScalar);

	template<typename T>
	inline QuaternionT<T>::QuaternionT() :
		myW(1), myVector({ 0, 0, 0 })
	{
	}

	template<typename T>
	inline QuaternionT<T>::QuaternionT(const QuaternionT<T>& anOther) :
		myW(anOther.myW), myVector(anOther.myVector)
	{
	}

	template<typename T>
	inline QuaternionT<T>::QuaternionT(T aW, const CU::Vector3<T>& aVector) :
		myW(aW), myVector(aVector)
	{
	}

	template<typename T>
	inline QuaternionT<T>::QuaternionT(const CU::Vector3<T>& aEulerAngles)
	{
		T h = aEulerAngles.y * static_cast<T>(0.5);
		T p = aEulerAngles.x * static_cast<T>(0.5);
		T b = aEulerAngles.z * static_cast<T>(0.5);

		T sinH = sin(h);
		T sinP = sin(p);
		T sinB = sin(b);

		T cosH = cos(h);
		T cosP = cos(p);
		T cosB = cos(b);

		myW = cosH * cosP * cosB + sinH * sinP * sinB;
		myVector.x = -cosH * sinP * cosB - sinH * cosP * sinB;
		myVector.y = cosH * sinP * sinB - sinH * cosP * cosB;
		myVector.z = sinH * sinP * cosB - cosH * cosP * sinB;
	}

	template<typename T>
	inline QuaternionT<T>::QuaternionT(const CU::Matrix4x4<T>& aM) :
		myW(0), myVector({ 0, 0, 0 })
	{
		T wValue = aM(1, 1) + aM(2, 2) + aM(3, 3);
		T xValue = aM(1, 1) - aM(2, 2) - aM(3, 3);
		T yValue = aM(2, 2) - aM(1, 1) - aM(3, 3);
		T zValue = aM(3, 3) - aM(1, 1) - aM(2, 2);

		unsigned biggestIndex = 0;
		T biggestValue = wValue;

		if (xValue > biggestValue)
		{
			biggestValue = xValue;
			biggestIndex = 1;
		}
		if (yValue > biggestValue)
		{
			biggestValue = yValue;
			biggestIndex = 2;
		}
		if (zValue > biggestValue)
		{
			biggestValue = zValue;
			biggestIndex = 3;
		}

		biggestValue = sqrt(biggestValue + 1.0f) * 0.5f;
		T mult = 0.25f / biggestValue;

		switch (biggestIndex)
		{
		case 0:
			myW = biggestValue;
			myVector.x = (aM(2, 3) - aM(3, 2)) * mult;
			myVector.y = (aM(3, 1) - aM(1, 3)) * mult;
			myVector.z = (aM(1, 2) - aM(2, 1)) * mult;
			break;
		case 1:
			myVector.x = biggestValue;
			myW = (aM(2, 3) - aM(3, 2)) * mult;
			myVector.y = (aM(1, 2) + aM(2, 1)) * mult;
			myVector.z = (aM(3, 1) + aM(1, 3)) * mult;
			break;
		case 2:
			myVector.y = biggestValue;
			myW = (aM(3, 1) - aM(1, 3)) * mult;
			myVector.x = (aM(1, 2) + aM(2, 1)) * mult;
			myVector.z = (aM(2, 3) + aM(3, 2)) * mult;
			break;
		case 3:
			myVector.z = biggestValue;
			myW = (aM(1, 2) - aM(2, 1)) * mult;
			myVector.x = (aM(3, 1) + aM(1, 3)) * mult;
			myVector.y = (aM(2, 3) + aM(3, 2)) * mult;
			break;
		}
	}

	template<typename T>
	inline void QuaternionT<T>::InitWithAxisAndRotation(const Vector3<T>& anAxis, T aRotation)
	{
		T halfRot = aRotation * 0.5;
		myW = cos(halfRot);
		myVector = anAxis * sin(halfRot);
	}

	template<typename T>
	inline void QuaternionT<T>::Normalize()
	{
		T length = GetMagnitude();
		if (length > 0.0001)
		{
			myW = myW / length;
			myVector = myVector / length;
		}
	}

	template<typename T>
	inline QuaternionT<T> QuaternionT<T>::GetNormalized() const
	{
		QuaternionT<T> returnValue(*this);
		returnValue.Normalize();
		return returnValue;
	}

	template<typename T>
	inline QuaternionT<T> QuaternionT<T>::GetInverse() const
	{
		T vectorLenghtSqr = GetMagnitude();
		if (CU::AreEqual(vectorLenghtSqr, static_cast<T>(1.0), static_cast<T>(0.0001)))
		{
			return GetConjugate();
		}
		T denominator = sqrt(myW * myW + vectorLenghtSqr);
		return QuaternionT<T>(myW / denominator, myVector / denominator);
	}

	template<typename T>
	inline T QuaternionT<T>::GetMagnitude() const
	{
		return sqrt(myW * myW + myVector.Dot(myVector));
	}

	template<typename T>
	inline QuaternionT<T> QuaternionT<T>::GetConjugate() const
	{
		return QuaternionT<T>{myW, myVector* static_cast<T>(-1)};
	}

	template<typename T>
	inline T QuaternionT<T>::Dot(QuaternionT<T> anOther) const
	{
		T dotProduct = myW * anOther.myW + myVector.Dot(anOther.myVector);
		return dotProduct;
	}

	template<typename T>
	inline CU::Matrix4x4<T> QuaternionT<T>::ToMatrix() const
	{
		T w = myW;
		T x = myVector.x;
		T y = myVector.y;
		T z = myVector.z;

		CU::Matrix4x4<T> output;
		output(1, 1) = 1 - 2 * y * y - 2 * z * z;
		output(1, 2) = 2 * x * y + 2 * w * z;
		output(1, 3) = 2 * x * z - 2 * w * y;

		output(2, 1) = 2 * x * y - 2 * w * z;
		output(2, 2) = 1 - 2 * x * x - 2 * z * z;
		output(2, 3) = 2 * y * z + 2 * w * x;

		output(3, 1) = 2 * x * z + 2 * w * y;
		output(3, 2) = 2 * y * z - 2 * w * x;
		output(3, 3) = 1 - 2 * x * x - 2 * y * y;

		return output;
	}

	template<typename T>
	inline CU::Vector3<T> QuaternionT<T>::EulerAngles() const
	{
		CU::Vector3<float> euler;

		T sp = -2 * (myVector.y * myVector.z + myW * myVector.x);

		if (Abs(sp) > 0.9999)
		{
			euler.x = static_cast<T>(CU::PI * 0.5f) * sp;
			euler.y = atan2(-myVector.x * myVector.z - myW * myVector.y, static_cast<T>(0.5) - myVector.y * myVector.y - myVector.z * myVector.z);
			euler.z = 0;
		}
		else
		{
			euler.x = asin(sp);
			euler.y = atan2(myVector.x * myVector.z - myW * myVector.y, static_cast<T>(0.5) - myVector.x * myVector.x - myVector.y * myVector.y);
			euler.z = atan2(myVector.x * myVector.y - myW * myVector.z, static_cast<T>(0.5) - myVector.x * myVector.x - myVector.z * myVector.z);
		}
		return euler;
	}

	template<typename T>
	inline QuaternionT<T> CommonUtilities::QuaternionT<T>::Slerp(const QuaternionT<T>& aFirst, QuaternionT<T> aSecond, T anInterpolation)
	{
		QuaternionT<T> returnQuaternion;
		T cosOmega = aFirst.Dot(aSecond);
		if (cosOmega < 0.0)
		{
			aSecond.myW *= -1.0;
			aSecond.myVector = aSecond.myVector * -1.f;
			cosOmega = -cosOmega;
		}

		T k0, k1;
		if (cosOmega > static_cast<T>(0.9999))
		{
			k0 = 1 - anInterpolation;
			k1 = anInterpolation;
		}
		else
		{
			T sinOmega = sqrt(1 - cosOmega * cosOmega);
			T omega = atan2(sinOmega, cosOmega);
			T oneOverSinOmega = 1 / sinOmega;

			k0 = sin((1 - anInterpolation) * omega) * oneOverSinOmega;
			k1 = sin(anInterpolation * omega) * oneOverSinOmega;
		}

		returnQuaternion.myW = aFirst.myW * k0 + aSecond.myW * k1;
		returnQuaternion.myVector = aFirst.myVector * k0 + aSecond.myVector * k1;
		returnQuaternion.Normalize();

		return returnQuaternion;
	}

	template<typename T>
	inline QuaternionT<T> operator+(const QuaternionT<T>& aFirst, const QuaternionT<T>& aSecond)
	{
		return QuaternionT<T>(aFirst.myW + aSecond.myW, aFirst.myVector + aSecond.myVector);
	}

	template<typename T>
	inline QuaternionT<T> operator*(const QuaternionT<T>& aFirst, const QuaternionT<T>& aSecond)
	{
		return QuaternionT<T>{aFirst.myW* aSecond.myW - aFirst.myVector.Dot(aSecond.myVector), aFirst.myW* aSecond.myVector + aSecond.myW * aFirst.myVector + aFirst.myVector.Cross(aSecond.myVector)};
	}

	template<typename T>
	inline QuaternionT<T> operator/(const QuaternionT<T>& aFirst, const QuaternionT<T>& aSecond)
	{
		return aFirst * aSecond.GetInverse();
	}

	template<typename T>
	inline Vector3<T> operator*(const Vector3<T>& aVector, const QuaternionT<T>& aQuaternion)
	{
		QuaternionT<T> vectorQuaternion(0, aVector);
		return (aQuaternion * vectorQuaternion * aQuaternion.GetInverse()).myVector;
	}

	template<typename T>
	inline QuaternionT<T> operator*(const QuaternionT<T>& aQuaternion, T aScalar)
	{
		return QuaternionT<T>(aQuaternion.myW * aScalar, aQuaternion.myVector * aScalar);
	}

	template<typename T>
	QuaternionT<T> operator/(const QuaternionT<T>& aQuaternion, T aScalar)
	{
		T factor = static_cast<T>(1) / aScalar;
		return aQuaternion * factor;
	}
}

namespace CU = CommonUtilities;
#pragma once
#include <math.h>
#include "Vector3.hpp"
#include "Matrix3x3.hpp"
#include "Matrix4x4.hpp"
#include "Constants.hpp"

namespace CommonUtilities
{

    template <class T>
    class Quaternion
    {
    public:
        inline Quaternion() {}

        inline Quaternion(const Quaternion<T>& aQuaternion)
        {
            myScalar = aQuaternion.myScalar;
            myVector = aQuaternion.myVector;
        }

        inline Quaternion(T aScalar, T aVectorX, T aVectorY, T aVectorZ)
        {
            myScalar = aScalar;
            myVector = Vector3<T>(aVectorX, aVectorY, aVectorZ);
        }

        inline Quaternion(T aScalar, const Vector3<T>& aVector)
        {
            myScalar = aScalar;
            myVector = aVector;
        }

        inline const T& scalar() const { return myScalar; }

        inline void set_scalar(T aScalar) { myScalar = aScalar; }

        inline const Vector3<T>& vector() const { return myVector; }

        inline void set_vector(const Vector3<T>& aVector) { myVector = aVector; }

        inline Quaternion<T> Inverse() const { return Quaternion<T>(myScalar, -myVector); }

        inline Quaternion<T> operator+(const Quaternion<T>& aQuaternion) const
        {
            return Quaternion<T>(myScalar + aQuaternion.myScalar, myVector + aQuaternion.myVector);
        }

        inline Quaternion<T>& operator+=(const Quaternion<T>& aQuaternion)
        {
            myScalar += aQuaternion.myScalar;
            myVector += aQuaternion.myVector;
            return *this;
        }

        inline Quaternion<T> operator*(const Quaternion<T>& aQuaternion) const
        {
            return Quaternion<T>(
                myScalar * aQuaternion.myScalar - Vector3<T>::DotProduct(myVector, aQuaternion.myVector),
                myScalar * aQuaternion.myVector + aQuaternion.myScalar * myVector + Vector3<T>::CrossProduct(myVector, aQuaternion.myVector));
        }

        inline Quaternion<T> operator*(T aScalar) const
        {
            T angle;
            Vector3<T> axis;
            ToAngleAxis(&angle, &axis);
            angle *= aScalar;

            return Quaternion<T>(cos(0.5f * angle),
                axis.Normalized() * static_cast<T>(sin(0.5f * angle)));
        }

        inline Vector3<T> operator*(const Vector3<T>& aVector) const
        {
            T ss = myScalar + myScalar;
            return ss * Vector3<T>::CrossProduct(myVector, aVector) + (ss * myScalar - 1) * aVector +
                2 * Vector3<T>::DotProduct(myVector, aVector) * myVector;
        }

        inline T Normalize()
        {
            T length = sqrt(myScalar * myScalar + Vector3<T>::DotProduct(myVector, myVector));
            T scale = (1 / length);
            myScalar *= scale;
            myVector *= scale;
            return length;
        }

        inline Quaternion<T> Normalized() const
        {
            Quaternion<T> q(*this);
            q.Normalize();
            return q;
        }

        inline void ToAngleAxis(T* outAngle, Vector3<T>* outAxis) const
        {
            const Quaternion<T> q = (myScalar > 0) ? *this : Quaternion<T>(-myScalar, -myVector);
            q.ToAngleAxisFull(outAngle, outAxis);
        }

        inline void ToAngleAxisFull(T* outAngle, Vector3<T>* outAxis) const
        {
            Vector3<T> axis = myVector;
            const T axis_length = axis.Normalize();
            if (axis_length == 0)
            {
                *outAxis = Vector3<T>(1, 0, 0);
            }
            else
            {
                *outAxis = axis;
            }
            *outAngle = 2 * atan2(axis_length, myScalar);
        }

        inline Vector3<T> ToEulerAngles() const
        {
            Matrix3x3<T> m(ToMatrix());
            T cos2 = m[0] * m[0] + m[1] * m[1];
            if (cos2 < 1e-6f)
            {
                return Vector3<T>(
                    0,
                    m[2] < 0 ? static_cast<T>(0.5 * PI) : static_cast<T>(-0.5 * PI),
                    -std::atan2(m[3], m[4]));
            }
            else
            {
                return Vector3<T>(std::atan2(m[5], m[8]),
                    std::atan2(-m[2], std::sqrt(cos2)),
                    std::atan2(m[1], m[0]));
            }
        }

        inline Matrix3x3<T> ToMatrix() const
        {
            const T x2 = myVector[0] * myVector[0], y2 = myVector[1] * myVector[1], z2 = myVector[2] * myVector[2];
            const T sx = myScalar * myVector[0], sy = myScalar * myVector[1], sz = myScalar * myVector[2];
            const T xz = myVector[0] * myVector[2], yz = myVector[1] * myVector[2], xy = myVector[0] * myVector[1];
            return Matrix3x3<T>(1 - 2 * (y2 + z2), 2 * (xy + sz), 2 * (xz - sy),
                2 * (xy - sz), 1 - 2 * (x2 + z2), 2 * (sx + yz),
                2 * (sy + xz), 2 * (yz - sx), 1 - 2 * (x2 + y2));
        }

        inline Matrix4x4<T> ToMatrix4() const
        {
            const T x2 = myVector[0] * myVector[0], y2 = myVector[1] * myVector[1], z2 = myVector[2] * myVector[2];
            const T sx = myScalar * myVector[0], sy = myScalar * myVector[1], sz = myScalar * myVector[2];
            const T xz = myVector[0] * myVector[2], yz = myVector[1] * myVector[2], xy = myVector[0] * myVector[1];
            return Matrix4x4<T>(1 - 2 * (y2 + z2), 2 * (xy + sz), 2 * (xz - sy), 0.0f,
                2 * (xy - sz), 1 - 2 * (x2 + z2), 2 * (sx + yz), 0.0f,
                2 * (sy + xz), 2 * (yz - sx), 1 - 2 * (x2 + y2), 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f);
        }

        static Quaternion<T> FromAngleAxis(T angle, const Vector3<T>& axis)
        {
            const T halfAngle = static_cast<T>(0.5) * angle;
            Vector3<T> localAxis(axis);
            return Quaternion<T>(
                cos(halfAngle),
                localAxis.Normalized() * static_cast<T>(sin(halfAngle)));
        }

        static Quaternion<T> FromEulerAngles(const Vector3<T>& angles)
        {
            const Vector3<T> halfAngles(static_cast<T>(0.5) * angles[0],
                static_cast<T>(0.5) * angles[1],
                static_cast<T>(0.5) * angles[2]);
            const T sinx = std::sin(halfAngles[0]);
            const T cosx = std::cos(halfAngles[0]);
            const T siny = std::sin(halfAngles[1]);
            const T cosy = std::cos(halfAngles[1]);
            const T sinz = std::sin(halfAngles[2]);
            const T cosz = std::cos(halfAngles[2]);
            return Quaternion<T>(cosx * cosy * cosz + sinx * siny * sinz,
                sinx * cosy * cosz - cosx * siny * sinz,
                cosx * siny * cosz + sinx * cosy * sinz,
                cosx * cosy * sinz - sinx * siny * cosz);
        }

        static Quaternion<T> FromEulerAngles(T x_rotation,
            T y_rotation,
            T z_rotation)
        {
            return FromEulerAngles(Vector3<T>(x_rotation, y_rotation, z_rotation));
        }

        static Quaternion<T> FromMatrix(const Matrix3x3<T>& m)
        {
            const T trace = m(0, 0) + m(1, 1) + m(2, 2);
            if (trace > 0)
            {
                const T s = sqrt(trace + 1) * 2;
                const T oneOverS = 1 / s;
                return Quaternion<T>(static_cast<T>(0.25) * s, (m[5] - m[7]) * oneOverS,
                    (m[6] - m[2]) * oneOverS, (m[1] - m[3]) * oneOverS);
            }
            else if (m[0] > m[4] && m[0] > m[8])
            {
                const T s = sqrt(m[0] - m[4] - m[8] + 1) * 2;
                const T oneOverS = 1 / s;
                return Quaternion<T>((m[5] - m[7]) * oneOverS, static_cast<T>(0.25) * s,
                    (m[3] + m[1]) * oneOverS, (m[6] + m[2]) * oneOverS);
            }
            else if (m[4] > m[8])
            {
                const T s = sqrt(m[4] - m[0] - m[8] + 1) * 2;
                const T oneOverS = 1 / s;
                return Quaternion<T>((m[6] - m[2]) * oneOverS, (m[3] + m[1]) * oneOverS,
                    static_cast<T>(0.25) * s, (m[5] + m[7]) * oneOverS);
            }
            else
            {
                const T s = sqrt(m[8] - m[0] - m[4] + 1) * 2;
                const T oneOverS = 1 / s;
                return Quaternion<T>((m[1] - m[3]) * oneOverS, (m[6] + m[2]) * oneOverS,
                    (m[5] + m[7]) * oneOverS, static_cast<T>(0.25) * s);
            }
        }

        static Quaternion<T> FromMatrix(const Matrix4x4<T>& m)
        {
            const T trace = m(0, 0) + m(1, 1) + m(2, 2);
            if (trace > 0)
            {
                const T s = sqrt(trace + 1) * 2;
                const T oneOverS = 1 / s;
                return Quaternion<T>(static_cast<T>(0.25) * s, (m[6] - m[9]) * oneOverS,
                    (m[8] - m[2]) * oneOverS, (m[1] - m[4]) * oneOverS);
            }
            else if (m[0] > m[5] && m[0] > m[10])
            {
                const T s = sqrt(m[0] - m[5] - m[10] + 1) * 2;
                const T oneOverS = 1 / s;
                return Quaternion<T>((m[6] - m[9]) * oneOverS, static_cast<T>(0.25) * s,
                    (m[4] + m[1]) * oneOverS, (m[8] + m[2]) * oneOverS);
            }
            else if (m[5] > m[10])
            {
                const T s = sqrt(m[5] - m[0] - m[10] + 1) * 2;
                const T oneOverS = 1 / s;
                return Quaternion<T>((m[8] - m[2]) * oneOverS, (m[4] + m[1]) * oneOverS,
                    static_cast<T>(0.25) * s, (m[6] + m[9]) * oneOverS);
            }
            else
            {
                const T s = sqrt(m[10] - m[0] - m[5] + 1) * 2;
                const T oneOverS = 1 / s;
                return Quaternion<T>((m[1] - m[4]) * oneOverS, (m[8] + m[2]) * oneOverS,
                    (m[6] + m[9]) * oneOverS, static_cast<T>(0.25) * s);
            }
        }

        static inline T DotProduct(const Quaternion<T>& q1, const Quaternion<T>& q2)
        {
            return q1.myScalar * q2.myScalar + Vector3<T>::DotProduct(q1.myVector, q2.myVector);
        }

        static inline Quaternion<T> Slerp(const Quaternion<T>& q1,
            const Quaternion<T>& q2, T s1)
        {
            if (q1.myScalar * q2.myScalar + Vector3<T>::DotProduct(q1.myVector, q2.myVector) > 0.9999f)
            {
                return Quaternion<T>(q1.myScalar * (1 - s1) + q2.myScalar * s1,
                    q1.myVector * (1 - s1) + q2.myVector * s1)
                    .Normalized();
            }
            return q1 * ((q1.Inverse() * q2) * s1);
        }

        inline T operator[](const int i) const { return i == 0 ? myScalar : myVector[i - 1]; }

        static inline Vector3<T> PerpendicularVector(const Vector3<T>& v)
        {
            Vector3<T> axis = Vector3<T>::CrossProduct(
                Vector3<T>(static_cast<T>(1), static_cast<T>(0), static_cast<T>(0)),
                v);

            if (axis.LengthSquared() < static_cast<T>(0.05))
            {
                axis = Vector3<T>::CrossProduct(
                    Vector3<T>(static_cast<T>(0), static_cast<T>(1), static_cast<T>(0)),
                    v);
            }
            return axis;
        }

        static inline Quaternion<T> RotateFromToWithAxis(
            const Vector3<T>& v1, const Vector3<T>& v2,
            const Vector3<T>& preferred_axis)
        {
            Vector3<T> start = v1.Normalized();
            Vector3<T> end = v2.Normalized();

            T dot_product = Vector3<T>::DotProduct(start, end);

            if (dot_product >= static_cast<T>(0.99999847691))
            {
                return Quaternion<T>::Identity;
            }

            if (dot_product <= static_cast<T>(-0.99999847691))
            {
                return Quaternion<T>(static_cast<T>(0), preferred_axis);
            }

            Vector3<T> cross_product = Vector3<T>::CrossProduct(start, end);

            return Quaternion<T>(static_cast<T>(1.0) + dot_product, cross_product)
                .Normalized();
        }

        static inline Quaternion<T> RotateFromTo(const Vector3<T>& v1,
            const Vector3<T>& v2)
        {
            Vector3<T> start = v1.Normalized();
            Vector3<T> end = v2.Normalized();

            T dot_product = Vector3<T>::DotProduct(start, end);

            if (dot_product >= static_cast<T>(0.99999847691))
            {
                return Quaternion<T>::Identity;
            }

            if (dot_product <= static_cast<T>(-0.99999847691))
            {
                return Quaternion<T>(0, PerpendicularVector(start));
            }

            Vector3<T> cross_product = Vector3<T>::CrossProduct(start, end);

            return Quaternion<T>(static_cast<T>(1.0) + dot_product, cross_product)
                .Normalized();
        }

        static inline Quaternion<T> LookAt(const Vector3<T>& forward,
            const Vector3<T>& up)
        {
            return FromMatrix(
                Matrix3x3<T>::LookAt(forward, Vector3<T>(static_cast<T>(0)), up));
        }

        static Quaternion<T> Identity;

    private:
        Vector3<T> myVector;
        T myScalar;
    };

    template <typename T>
    Quaternion<T> Quaternion<T>::Identity = Quaternion<T>(1, 0, 0, 0);

    template <class T>
    inline Quaternion<T> operator*(T s, const Quaternion<T>& q)
    {
        return q * s;
    }
}  

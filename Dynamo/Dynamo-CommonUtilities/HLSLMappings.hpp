#pragma once

#include "Vector.hpp"
#include "Matrix.hpp"

typedef CU::Vector2<float> float2;
typedef CU::Vector3<float> float3;
typedef CU::Vector4<float> float4;

typedef CU::Vector2<int> int2;
typedef CU::Vector3<int> int3;
typedef CU::Vector4<int> int4;

typedef CU::Vector2<unsigned int> uint2;
typedef CU::Vector3<unsigned int> uint3;
typedef CU::Vector4<unsigned int> uint4;

typedef CU::Matrix2x2<float> float2x2;
typedef CU::Matrix3x3<float> float3x3;
typedef CU::Matrix4x4<float> float4x4;

typedef CU::Matrix2x2<double> double2x2;
typedef CU::Matrix3x3<double> double3x3;
typedef CU::Matrix4x4<double> double4x4;

typedef CU::Matrix2x2<int> int2x2;
typedef CU::Matrix3x3<int> int2x3;
typedef CU::Matrix4x4<int> int4x4;

typedef CU::Matrix2x2<unsigned int> uint2x2;
typedef CU::Matrix3x3<unsigned int> uint3x3;
typedef CU::Matrix4x4<unsigned int> uint4x4;
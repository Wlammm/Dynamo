#pragma once

namespace Dynamo {};

using uint = unsigned int;
namespace Dyn = Dynamo;

namespace CommonUtilities
{
	using Vector2f = Vector2<float>;
	using Vector2ui = Vector2<unsigned int>;
	using Vector2i = Vector2<int>;

	using Vector3f = Vector3<float>;
	using Vector3ui = Vector3<unsigned int>;
	using Vector3i = Vector3<int>;

	using Vector4f = Vector4<float>;
	using Vector4ui = Vector4<unsigned int>;
	using Vector4i = Vector4<int>;
}

using Vec2f = CU::Vector2<float>;
using Vec2ui = CU::Vector2<unsigned int>;
using Vec2i = CU::Vector2<int>;
using Vec2d = CU::Vector2<double>;

using Vec3f = CU::Vector3<float>;
using Vec3ui = CU::Vector3<unsigned int>;
using Vec3i = CU::Vector3<int>;

using Vec4f = CU::Vector4<float>;
using Vec4ui = CU::Vector4<unsigned int>;
using Vec4i = CU::Vector4<int>;
using Vec4d = CU::Vector4<double>;

using Color = CU::Color;

using Mat4f = CU::Matrix4x4<float>;

using Matrix4x4f = CU::Matrix4x4f;
using Mat4f = CU::Matrix4x4f;
using Quat = CU::Quaternion;

constexpr float Rad2Deg = 57.2957795f;
constexpr float Deg2Rad = 0.0174532925f;
constexpr float PI = 3.14159265359f;
constexpr float HALF_PI = 3.14159265359f * .5f;

#define DYNAMO_VERSION " V0.0"
#pragma once

#include "Shaders/SharedDefines.hpp"

#define FRAME_BUFFER_SLOT 0
#define OBJECT_BUFFER_SLOT 1
#define SHADER_BUFFER_SLOT 2
#define BONE_BUFFER_SLOT 3
#define DIRECTIONAL_LIGHT_BUFFER_SLOT 4
#define AMBIENT_LIGHT_BUFFER_SLOT 5
#define POINT_LIGHT_BUFFER_SLOT 6
#define SPOT_LIGHT_BUFFER_SLOT 7
#define CUSTOM_BUFFER_SLOT 8
#define EMISSIVE_BUFFER_SLOT 9
#define MATERIAL_BUFFER_SLOT 10

#define CUBEMAP_TEXTURE_SLOT 0
#define ALBEDO_TEXTURE_SLOT 1
#define NORMAL_TEXTURE_SLOT 2
#define MATERIAL_TEXTURE_SLOT 3

#define FS_TEXTURE_SLOT1 0
#define FS_TEXTURE_SLOT2 1
#define FS_TEXTURE_SLOT3 2
#define FS_TEXTURE_SLOT4 3

#define DEFAULT_SAMPLER_SLOT 0
#define WRAP_SAMPLER_SLOT 1
#define CLAMP_SAMPLER_SLOT 2
#define POINT_SAMPLER_SLOT 3

namespace Dynamo
{
	struct FrameBuffer
	{
		Mat4f myToCamera{};
		Mat4f myToProjection{};
		Vec4f myCameraPosition{};
	};

	struct ObjectBuffer
	{
		Mat4f myToWorld{};
		Color myColor{};
		Color myAdditiveColor{};
		Vec2f myUVScale{};
		int myIsAnimated = 0;

		float padding = 0;
	};

	struct BoneBuffer
	{
		std::array<Mat4f, 128> myBoneTransforms;
	};

	struct DirectionalLightBuffer
	{
		Vec4f myToLight{};
		Color myColor{};
		float myIntensity = 0;

		Vec3f padding{};
	};

	struct EmissiveBuffer
	{
		float myIntensity = 0;

		Vec3f padding{};
	};

	struct AmbientLightBuffer
	{
		float myIntensity = 0;

		Vec3f padding{};
	};

	struct ForwardPointLightBuffer
	{
		struct Buffer
		{
			Vec4f myPosition{};
			Color myColor{};
			float myIntensity = 0;
			float myRange = 0;

			Vec2f padding{};
		};
		std::array<Buffer, FORWARD_POINT_LIGHT_COUNT> myPointLights;

		uint myNumPointLights = 0;
		Vec3ui padding{};
	};

	struct ForwardSpotLightBuffer
	{
		struct Buffer
		{
			Vec4f myPosition{};
			Vec4f myDirection{};
			Color myColor{};

			float myIntensity = 0;
			float myRange = 0;
			float myInnerAngle = 0;
			float myOuterAngle = 0;
		};

		std::array<Buffer, FORWARD_SPOT_LIGHT_COUNT> myLights;

		uint myNumSpotLights = 0;
		Vec3ui padding{};
	};

	struct DeferredPointLightBuffer
	{
		struct Buffer
		{
			Vec4f myPosition{};
			Color myColor{};
			float myIntensity = 0;
			float myRange = 0;
			Vec2f padding{};
		};
		std::array<Buffer, DEFERRED_POINT_LIGHT_COUNT> myLights;

		uint myLightCount;
		Vec3ui padding;
	};

	struct DeferredSpotLightBuffer
	{
		struct Buffer
		{
			Vec4f myPosition{};
			Vec4f myDirection{};
			Color myColor{};
			float myIntensity = 0;
			float myRange = 0;
			float myInnerAngle = 0;
			float myOuterAngle = 0;
		};
		std::array<Buffer, DEFERRED_SPOT_LIGHT_COUNT> myLights;

		uint myLightCount = 0;
		Vec3ui padding{};
	};

	struct MaterialBuffer
	{
		float myRoughnessConstant = 0;
		float myRoughnessInterp = 1;
		float myMetalnessConstant = 0;
		float myMetalnessInterp = 1;

		float myCustomValue1;
		float myCustomValue2;
		float myCustomValue3;
		float myCustomValue4;
	};
}
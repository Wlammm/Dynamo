#pragma once

#define FRAME_BUFFER_SLOT 0
#define OBJECT_BUFFER_SLOT 1
#define ALBEDO_TEXTURE_SLOT 1
#define NORMAL_TEXTURE_SLOT 2
#define MATERIAL_TEXTURE_SLOT 3

#define DEFAULT_SAMPLER_SLOT 0
#define WRAP_SAMPLER_SLOT 1
#define CLAMP_SAMPLER_SLOT 2
#define POINT_SAMPLER_SLOT 3

namespace Dynamo
{
	struct FrameBuffer
	{
		struct Buffer
		{
			Mat4f myToCamera;
			Mat4f myToProjection;
			Vec4f myCameraPosition;
		} buffer;
	};

	struct ObjectBuffer
	{
		struct Buffer
		{
			Mat4f myToWorld;
			Color myColor;
			Vec2f myUVScale;

			Vec2f padding;
		} buffer;
	};
}
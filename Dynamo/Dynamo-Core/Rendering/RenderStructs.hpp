#pragma once

#include "pch.h"

#define FRAME_BUFFER_SLOT 0
#define OBJECT_BUFFER_SLOT 1

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
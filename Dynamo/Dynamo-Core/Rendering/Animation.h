#pragma once

#include "pch.h"

namespace Dynamo
{
	struct Animation
	{
		std::unordered_map<uint, std::vector<Mat4f>> myBoneTransforms;
		uint myFrameCount;
		std::filesystem::path myPath = "";
	};
}
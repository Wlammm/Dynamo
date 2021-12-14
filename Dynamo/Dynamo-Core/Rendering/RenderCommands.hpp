#pragma once

#include "pch.h"

namespace Dynamo
{
	struct Mesh;

	struct MeshCommand
	{
		Mat4f myMatrix{};
		Color myColor{};
		Color myAdditiveColor{};
		Mesh const* myMesh = nullptr;
		Material* myMaterial = nullptr;
	};

	struct PointLightCommand
	{
		Color myColor{};
		Vec3f myPosition{};
		float myRange = 0;
		float myIntensity = 0;
	};

	struct SpotLightCommand
	{
		Color myColor{};
		Vec3f myDirection{};
		Vec3f myPosition{};
		float myInnerAngle = 0;
		float myOuterAngle = 0;
		float myIntensity = 0;
		float myRange = 0;
	};

	struct DirectionalLightCommand
	{
		Color myColor{};
		Vec3f myDirection{};
		float myIntensity = 0;
	};

	struct AmbientLightCommand
	{
		float myIntensity = 0;
		SRV* myCubeMap = nullptr;
	};
}

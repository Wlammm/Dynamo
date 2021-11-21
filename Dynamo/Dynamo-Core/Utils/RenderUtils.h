#pragma once

namespace Dynamo
{
	class RenderUtils
	{
	public:
		enum BlendState
		{
			BLENDSTATE_DISABLE,
			BLENDSTATE_ALPHABLEND,
			BLENDSTATE_FOLLIAGE,
			BLENDSTATE_ADDITIVE,
			BLENDSTATE_COUNT
		};

		enum SamplerState
		{
			SAMPLERSTATE_POINT,
			SAMPLERSTATE_TRILINEARWRAP,
			SAMPLERSTATE_TRILINEARCLAMP,
			SAMPLERSTATE_TRILINEAR,
			SAMPLERSTATE_COUNT
		};

	public:
		static void Create();
		static void Destroy();

		static void SetSamplerState(SamplerState aState, uint aSlot);
		static void SetBlendState(BlendState aState);

	private:
		static void CreateBlendStates();
		static void CreateSamplerStates();

	private:
		static std::array<ID3D11BlendState*, BLENDSTATE_COUNT> myBlendStates;
		static std::array<ID3D11SamplerState*, SAMPLERSTATE_COUNT> mySamplerStates;
	};
}
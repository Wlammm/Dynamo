#include "pch.h"
#include "HDREffect.h"

namespace Dynamo
{
	HDREffect::HDREffect()
	{
		ReInit();
		myHDRShader = ShaderFactory::GetShader("Assets/Shaders/FullscreenPS-HDR.cso", ShaderType::PixelShader);
	}

	void HDREffect::Render(FullscreenRenderer& aFullscreenRenderer, Texture& aFinalTarget)
	{
		myIntermediateTexture.ClearTexture();

		myIntermediateTexture.SetAsActiveTarget();
		aFinalTarget.SetAsResourceOnSlot(FS_TEXTURE_SLOT1);
		aFullscreenRenderer.Render(myHDRShader);

		aFinalTarget.SetAsActiveTarget();
		myIntermediateTexture.SetAsResourceOnSlot(FS_TEXTURE_SLOT1);
		aFullscreenRenderer.RenderCopy();
	}
	void HDREffect::Release()
	{
		myIntermediateTexture.Release();
	}

	void HDREffect::ReInit()
	{
		myIntermediateTexture = TextureFactory::CreateTexture(Main::GetWindowResolution(), DXGI_FORMAT_R32G32B32A32_FLOAT);
	}
}

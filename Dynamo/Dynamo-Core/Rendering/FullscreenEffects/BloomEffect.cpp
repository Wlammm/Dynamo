#include "pch.h"
#include "BloomEffect.h"

namespace Dynamo
{
	BloomEffect::BloomEffect()
	{
		myLuminanceShader = ShaderFactory::GetShader("Shaders/FullscreenPS-Luminance.cso", ShaderType::PixelShader);
		myGaussianHShader = ShaderFactory::GetShader("Shaders/FullscreenPS-GaussianH.cso", ShaderType::PixelShader);
		myGaussianVShader = ShaderFactory::GetShader("Shaders/FullscreenPS-GaussianV.cso", ShaderType::PixelShader);
		myBloomShader = ShaderFactory::GetShader("Shaders/FullscreenPS-Bloom.cso", ShaderType::PixelShader);

		const Vec2ui& resolution = Main::GetWindowResolution();
		Vec2ui halfRes = (resolution.Cast<float>() * 0.5f).Cast<uint>();
		Vec2ui quarterRes = (resolution.Cast<float>() * 0.25f).Cast<uint>();

		myLuminanceTexture = TextureFactory::CreateTexture(resolution, DXGI_FORMAT_R8G8B8A8_UNORM);
		myHalfSizeTexture = TextureFactory::CreateTexture(halfRes, DXGI_FORMAT_R8G8B8A8_UNORM);
		myQuarterSizeTexture = TextureFactory::CreateTexture(quarterRes, DXGI_FORMAT_R8G8B8A8_UNORM);
		myBlurTexture1 = TextureFactory::CreateTexture(resolution, DXGI_FORMAT_R8G8B8A8_UNORM);
		myBlurTexture2 = TextureFactory::CreateTexture(resolution, DXGI_FORMAT_R8G8B8A8_UNORM);
		myIntermediateTexture = TextureFactory::CreateTexture(resolution, DXGI_FORMAT_R8G8B8A8_UNORM);
	}

	void BloomEffect::Render(FullscreenRenderer& aFullscreenRenderer, Texture& aFinalTarget)
	{
		myLuminanceTexture.SetAsActiveTarget();
		aFinalTarget.SetAsResourceOnSlot(FS_TEXTURE_SLOT1);
		aFullscreenRenderer.Render(myLuminanceShader);

		myHalfSizeTexture.SetAsActiveTarget();
		myLuminanceTexture.SetAsResourceOnSlot(FS_TEXTURE_SLOT1);
		aFullscreenRenderer.RenderCopy();

		myQuarterSizeTexture.SetAsActiveTarget();
		myHalfSizeTexture.SetAsResourceOnSlot(FS_TEXTURE_SLOT1);
		aFullscreenRenderer.RenderCopy();

		myBlurTexture1.SetAsActiveTarget();
		myQuarterSizeTexture.SetAsResourceOnSlot(FS_TEXTURE_SLOT1);
		aFullscreenRenderer.RenderCopy();

		myBlurTexture2.SetAsActiveTarget();
		myBlurTexture1.SetAsResourceOnSlot(FS_TEXTURE_SLOT1);
		aFullscreenRenderer.Render(myGaussianHShader);

		myBlurTexture1.SetAsActiveTarget();
		myBlurTexture2.SetAsResourceOnSlot(FS_TEXTURE_SLOT1);
		aFullscreenRenderer.Render(myGaussianVShader);

		myBlurTexture2.SetAsActiveTarget();
		myBlurTexture1.SetAsResourceOnSlot(FS_TEXTURE_SLOT1);
		aFullscreenRenderer.Render(myGaussianHShader);

		myBlurTexture1.SetAsActiveTarget();
		myBlurTexture2.SetAsResourceOnSlot(FS_TEXTURE_SLOT1);
		aFullscreenRenderer.Render(myGaussianVShader);

		myQuarterSizeTexture.SetAsActiveTarget();
		myBlurTexture1.SetAsResourceOnSlot(FS_TEXTURE_SLOT1);
		aFullscreenRenderer.RenderCopy();

		myHalfSizeTexture.SetAsActiveTarget();
		myQuarterSizeTexture.SetAsResourceOnSlot(FS_TEXTURE_SLOT1);
		aFullscreenRenderer.RenderCopy();

		myIntermediateTexture.SetAsActiveTarget();
		aFinalTarget.SetAsResourceOnSlot(FS_TEXTURE_SLOT1);
		myHalfSizeTexture.SetAsResourceOnSlot(FS_TEXTURE_SLOT2);
		aFullscreenRenderer.Render(myBloomShader);

		aFinalTarget.SetAsResourceOnSlot(FS_TEXTURE_SLOT1);
		myIntermediateTexture.SetAsResourceOnSlot(FS_TEXTURE_SLOT1);
		aFullscreenRenderer.RenderCopy();
	}
}

#include "pch.h"
#include "BloomEffect.h"
#include "Utils/RenderUtils.h"

namespace Dynamo
{
	BloomEffect::BloomEffect()
	{
		myLuminanceShader = ShaderFactory::GetShader("Shaders/FullscreenPS-Luminance.cso", ShaderType::PixelShader);
		myGaussianHShader = ShaderFactory::GetShader("Shaders/FullscreenPS-GaussianH.cso", ShaderType::PixelShader);
		myGaussianVShader = ShaderFactory::GetShader("Shaders/FullscreenPS-GaussianV.cso", ShaderType::PixelShader);
		myBloomShader = ShaderFactory::GetShader("Shaders/FullscreenPS-Bloom.cso", ShaderType::PixelShader);

		ReInit();

		RenderUtils::CreateBuffer<LuminanceBuffer>(myLuminanceBuffer);
	}

	void BloomEffect::Render(FullscreenRenderer& aFullscreenRenderer, Texture& aFinalTarget)
	{
		ClearTextures();

		myLuminanceBufferData.myCutoff = myCutoff;
		RenderUtils::MapBuffer<LuminanceBuffer>(myLuminanceBufferData, myLuminanceBuffer);
		Main::GetContext()->PSSetConstantBuffers(CUSTOM_BUFFER_SLOT, 1, &myLuminanceBuffer);

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

		//myIntermediateTexture.SetAsActiveTarget();
		//myLuminanceTexture.SetAsResourceOnSlot(FS_TEXTURE_SLOT1);
		//aFullscreenRenderer.RenderCopy();
		//
		aFinalTarget.SetAsActiveTarget(FS_TEXTURE_SLOT1);
		myIntermediateTexture.SetAsResourceOnSlot(FS_TEXTURE_SLOT1);
		aFullscreenRenderer.RenderCopy();
	}

	void BloomEffect::Release()
	{
		myLuminanceTexture.Release();
		myHalfSizeTexture.Release();
		myQuarterSizeTexture.Release();
		myBlurTexture1.Release();
		myBlurTexture2.Release();
		myIntermediateTexture.Release();
	}

	void BloomEffect::ReInit()
	{
		const Vec2ui& resolution = Main::GetWindowResolution();
		Vec2ui halfRes = (resolution.Cast<float>() * 0.5f).Cast<uint>();
		Vec2ui quarterRes = (resolution.Cast<float>() * 0.25f).Cast<uint>();

		myLuminanceTexture = TextureFactory::CreateTexture(resolution, DXGI_FORMAT_R32G32B32A32_FLOAT);
		myHalfSizeTexture = TextureFactory::CreateTexture(halfRes, DXGI_FORMAT_R32G32B32A32_FLOAT);
		myQuarterSizeTexture = TextureFactory::CreateTexture(quarterRes, DXGI_FORMAT_R32G32B32A32_FLOAT);
		myBlurTexture1 = TextureFactory::CreateTexture(resolution, DXGI_FORMAT_R32G32B32A32_FLOAT);
		myBlurTexture2 = TextureFactory::CreateTexture(resolution, DXGI_FORMAT_R32G32B32A32_FLOAT);
		myIntermediateTexture = TextureFactory::CreateTexture(resolution, DXGI_FORMAT_R32G32B32A32_FLOAT);
	}

	void BloomEffect::SetCutoff(const float aCutoff)
	{
		myCutoff = aCutoff;
	}

	float BloomEffect::GetCutoff() const
	{
		return myCutoff;
	}

	void BloomEffect::ClearTextures()
	{
		myLuminanceTexture.ClearTexture();
		myHalfSizeTexture.ClearTexture();
		myQuarterSizeTexture.ClearTexture();
		myBlurTexture1.ClearTexture();
		myBlurTexture2.ClearTexture();
		myIntermediateTexture.ClearTexture();
	}
}
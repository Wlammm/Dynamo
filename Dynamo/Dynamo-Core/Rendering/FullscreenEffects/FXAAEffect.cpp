#include "pch.h"
#include "FXAAEffect.h"
#include "Utils/RenderUtils.h"

namespace Dynamo
{
	FXAAEffect::FXAAEffect()
	{
		RenderUtils::CreateBuffer<FXAABuffer>(myBuffer);
		ReInit();
		myShader = ShaderFactory::GetShader("Shaders/FullscreenPS-FXAA.cso", ShaderType::PixelShader);
	}
	
	FXAAEffect::~FXAAEffect()
	{
		myBuffer->Release();
	}

	void FXAAEffect::Render(FullscreenRenderer& aFullscreenRenderer, Texture& aFinalTarget)
	{
		myIntermediateTexture.ClearTexture();
		myBufferData.myResolution = Main::GetWindowResolution().Cast<float>();
		RenderUtils::MapBuffer<FXAABuffer>(myBufferData, myBuffer);

		Main::GetContext()->PSSetConstantBuffers(CUSTOM_BUFFER_SLOT, 1, &myBuffer);

		myIntermediateTexture.SetAsActiveTarget();
		aFinalTarget.SetAsResourceOnSlot(FS_TEXTURE_SLOT1);
		aFullscreenRenderer.Render(myShader);

		aFinalTarget.SetAsActiveTarget();
		myIntermediateTexture.SetAsResourceOnSlot(FS_TEXTURE_SLOT1);
		aFullscreenRenderer.RenderCopy();
	}
	void FXAAEffect::Release()
	{
		myIntermediateTexture.Release();
	}

	void FXAAEffect::ReInit()
	{
		myIntermediateTexture = TextureFactory::CreateTexture(Main::GetWindowResolution(), DXGI_FORMAT_R32G32B32A32_FLOAT);
	}
}

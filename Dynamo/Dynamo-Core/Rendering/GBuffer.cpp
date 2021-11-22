#include "pch.h"
#include "GBuffer.h"

namespace Dynamo
{
	void GBuffer::ClearTextures()
	{
		const float clearColor[4] = { 0, 0, 0, 0 };

		for (int i = 0; i < GBufferTexture::COUNT; ++i)
		{
			Main::GetContext()->ClearRenderTargetView(myRTVs[(GBufferTexture)i], &clearColor[0]);
		}
	}

	void GBuffer::SetAsActiveTarget(Texture* aDepth)
	{
		Main::GetContext()->OMSetRenderTargets(GBufferTexture::COUNT, &myRTVs[0], (aDepth != nullptr) ? aDepth->GetDepth() : nullptr);
		Main::GetContext()->RSSetViewports(1, myViewport);
	}

	void GBuffer::SetAsResourceOnSlot(GBufferTexture aTexture, uint aSlot) const
	{
		Main::GetContext()->PSSetShaderResources(aSlot, 1, &mySRVs[aTexture]);
	}

	void GBuffer::SetAllAsResources()
	{
		Main::GetContext()->PSSetShaderResources(1, GBufferTexture::COUNT, &mySRVs[0]);
	}
}

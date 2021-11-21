#include "pch.h"
#include "Texture.h"

namespace Dynamo
{
	Texture::~Texture()
	{
	}

	void Texture::SetAsActiveDepth()
	{
		Main::GetContext()->OMSetRenderTargets(0, nullptr, myDepth);
		Main::GetContext()->RSSetViewports(1, myViewport);
	}

	void Texture::SetAsActiveTarget(Texture* aDepth)
	{
		if (aDepth)
		{
			Main::GetContext()->OMSetRenderTargets(1, &myRenderTarget, aDepth->myDepth);
		}
		else
		{
			Main::GetContext()->OMSetRenderTargets(1, &myRenderTarget, nullptr);
		}

		Main::GetContext()->RSSetViewports(1, myViewport);
	}

	void Texture::SetAsResourceOnSlot(const uint aSlot) const
	{
		assert(mySRV && "Texture not initialized.");
		Main::GetContext()->PSSetShaderResources(aSlot, 1, &mySRV);
		Main::GetContext()->GSSetShaderResources(aSlot, 1, &mySRV);
		Main::GetContext()->VSSetShaderResources(aSlot, 1, &mySRV);
	}

	void Texture::ClearTexture()
	{
		float clearColor[4] = { 0, 0, 0, 0 };
		Main::GetContext()->ClearRenderTargetView(myRenderTarget, &clearColor[0]);
	}

	void Texture::ClearDepth(float aClearDepthValue, uint aClearStencilValue)
	{
		Main::GetContext()->ClearDepthStencilView(myDepth, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, aClearDepthValue, aClearStencilValue);
	}
}
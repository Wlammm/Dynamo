#pragma once

namespace Dynamo
{
	class DirectXFramework
	{
	public:
		DirectXFramework();
		~DirectXFramework();

		void BeginFrame();
		void RenderFrame();
		void EndFrame();

		void GenerateTargets();

		ID3D11RenderTargetView* GetBackBuffer() const;
		IDXGISwapChain* GetSwapChain() const;
		ID3D11DepthStencilView* GetDepthBuffer() const;

	private:
		IDXGISwapChain* mySwapChain = nullptr;
		ID3D11Device* myDevice = nullptr;
		ID3D11DeviceContext* myContext = nullptr;

		ID3D11DepthStencilView* myDepthBuffer = nullptr;
		ID3D11RenderTargetView* myBackBuffer = nullptr;
	};
}
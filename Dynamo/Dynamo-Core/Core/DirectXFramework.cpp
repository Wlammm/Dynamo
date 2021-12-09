#include "pch.h"
#include "DirectXFramework.h"
#include "Rendering/RenderManager.h"
#include "Utils/ImGuiManager.h"

#pragma comment(lib, "d3d11.lib")

namespace Dynamo
{
	DirectXFramework::DirectXFramework()
	{
		HRESULT result;
		DXGI_SWAP_CHAIN_DESC swapchainDesc = {};
		swapchainDesc.BufferCount = 1;
		swapchainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapchainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapchainDesc.OutputWindow = Main::GetHWND();
		swapchainDesc.SampleDesc.Count = 1;
		swapchainDesc.Windowed = true;

		UINT flags = 0;
#ifdef false
		flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		result = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, flags, nullptr, 0, D3D11_SDK_VERSION, &swapchainDesc,
			&mySwapChain, &myDevice, nullptr, &myContext);

		assert(SUCCEEDED(result) && "Failed to create devices & swapchain.");

		GenerateTargets();

		Main::ourInstance->myContext = myContext;
		Main::ourInstance->myDevice = myDevice;
	}

	DirectXFramework::~DirectXFramework()
	{
		mySwapChain->Release();
		mySwapChain = nullptr;

		myDevice->Release();
		myDevice = nullptr;

		myContext->Release();
		myContext = nullptr;

		myBackBuffer->Release();
		myBackBuffer = nullptr;

		myDepthBuffer->Release();
		myDepthBuffer = nullptr;
	}

	void DirectXFramework::BeginFrame()
	{
		ImGuiManager::BeginFrame();

		myContext->OMSetRenderTargets(1, &myBackBuffer, myDepthBuffer);

		const float clearColor[4] = { 0, 0, 0, 0 };
		myContext->ClearRenderTargetView(myBackBuffer, clearColor);
		myContext->ClearDepthStencilView(myDepthBuffer, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}

	void DirectXFramework::RenderFrame()
	{
		Main::GetRenderManager().Render();
		ImGuiManager::Render();
	}

	void DirectXFramework::EndFrame()
	{
		mySwapChain->Present(1, 0);
		Input::GetManager().EndFrame();
	}

	void DirectXFramework::GenerateTargets()
	{
		HRESULT result;
		ID3D11Texture2D* backBufferTexture;
		result = mySwapChain->GetBuffer(0, _uuidof(ID3D11Texture2D), (void**)&backBufferTexture);
		assert(SUCCEEDED(result));

		result = myDevice->CreateRenderTargetView(backBufferTexture, nullptr, &myBackBuffer);
		assert(SUCCEEDED(result));

		result = backBufferTexture->Release();
		assert(SUCCEEDED(result));

		// Do I even need this?
		ID3D11Texture2D* depthBufferTexture;
		D3D11_TEXTURE2D_DESC depthBufferDesc = { 0 };

		depthBufferDesc.Width = (uint)Main::GetWindowSize().x;
		depthBufferDesc.Height = (uint)Main::GetWindowSize().y;
		depthBufferDesc.ArraySize = 1;
		depthBufferDesc.Format = DXGI_FORMAT_D32_FLOAT;
		depthBufferDesc.SampleDesc.Count = 1;
		depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

		result = myDevice->CreateTexture2D(&depthBufferDesc, nullptr, &depthBufferTexture);
		assert(SUCCEEDED(result));

		result = myDevice->CreateDepthStencilView(depthBufferTexture, nullptr, &myDepthBuffer);
		assert(SUCCEEDED(result));

		D3D11_VIEWPORT viewport = { 0 };
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;
		viewport.Width = (float)Main::GetWindowSize().x;
		viewport.Height = (float)Main::GetWindowSize().y;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		myContext->RSSetViewports(1, &viewport);
	}

	ID3D11RenderTargetView* DirectXFramework::GetBackBuffer() const
	{
		return myBackBuffer;
	}

	IDXGISwapChain* DirectXFramework::GetSwapChain() const
	{
		return mySwapChain;
	}
	ID3D11DepthStencilView* DirectXFramework::GetDepthBuffer() const
	{
		return myDepthBuffer;
	}
}
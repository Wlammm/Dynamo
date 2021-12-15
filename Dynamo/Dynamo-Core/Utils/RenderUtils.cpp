#include "pch.h"
#include "RenderUtils.h"

namespace Dynamo
{
	void RenderUtils::Create()
	{
		CreateBlendStates();
		CreateSamplerStates();
		CreateDepthStates();
	}

	void RenderUtils::Destroy()
	{
		//for (int i = 0; i < SAMPLERSTATE_COUNT; ++i)
		//{
		//	mySamplerStates[i]->Release();
		//}
		
		//for (int i = 0; i < BLENDSTATE_COUNT; ++i)
		//{
		//	myBlendStates[i]->Release();
		//}
	}

	void RenderUtils::SetSamplerState(SamplerState aState, uint aSlot)
	{
		Main::GetContext()->VSSetSamplers(aSlot, 1, &mySamplerStates[aState]);
		Main::GetContext()->PSSetSamplers(aSlot, 1, &mySamplerStates[aState]);
		Main::GetContext()->GSSetSamplers(aSlot, 1, &mySamplerStates[aState]);
	}

	void RenderUtils::SetBlendState(BlendState aState)
	{
		// TODO: Should this be 0.5 maybe?
		float blendFactor[4] = { 0.5f, 0.5f, 0.5f, 0.5f };
		UINT sampleMask = 0xffffffff;
		Main::GetContext()->OMSetBlendState(myBlendStates[aState], blendFactor, sampleMask);
	}

	void RenderUtils::SetDepthState(DepthState aState)
	{
		Main::GetContext()->OMSetDepthStencilState(myDepthStates[aState], 1);
	}

	void RenderUtils::CreateBlendStates()
	{
		HRESULT result = 0;

		D3D11_BLEND_DESC alphaBlendDesc = {};
		alphaBlendDesc.RenderTarget[0].BlendEnable = true;
		alphaBlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		alphaBlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		alphaBlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		alphaBlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		alphaBlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
		alphaBlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_MAX;
		alphaBlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		alphaBlendDesc.AlphaToCoverageEnable = false;
		result = Main::GetDevice()->CreateBlendState(&alphaBlendDesc, &myBlendStates[BLENDSTATE_ALPHABLEND]);
		assert(SUCCEEDED(result));

		alphaBlendDesc.AlphaToCoverageEnable = true;
		result = Main::GetDevice()->CreateBlendState(&alphaBlendDesc, &myBlendStates[BLENDSTATE_FOLLIAGE]);
		assert(SUCCEEDED(result));

		D3D11_BLEND_DESC additiveBlendDesc = {};
		additiveBlendDesc.RenderTarget[0].BlendEnable = true;
		additiveBlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		additiveBlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		additiveBlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		additiveBlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		additiveBlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
		additiveBlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_MAX;
		additiveBlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		result = Main::GetDevice()->CreateBlendState(&additiveBlendDesc, &myBlendStates[BLENDSTATE_ADDITIVE]);
		assert(SUCCEEDED(result));

		myBlendStates[BLENDSTATE_DISABLE] = nullptr;
	}
	
	void RenderUtils::CreateSamplerStates()
	{
		HRESULT result;

		D3D11_SAMPLER_DESC pointSampleDesc = {};
		pointSampleDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		pointSampleDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		pointSampleDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		pointSampleDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		pointSampleDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		pointSampleDesc.MinLOD = -FLT_MAX;
		pointSampleDesc.MaxLOD = FLT_MAX;
		result = Main::GetDevice()->CreateSamplerState(&pointSampleDesc, &mySamplerStates[SAMPLERSTATE_POINT]);
		assert(SUCCEEDED(result));

		D3D11_SAMPLER_DESC trilWrapSampleDesc = {};
		trilWrapSampleDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		trilWrapSampleDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		trilWrapSampleDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		trilWrapSampleDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		trilWrapSampleDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		trilWrapSampleDesc.MinLOD = -FLT_MAX;
		trilWrapSampleDesc.MaxLOD = FLT_MAX;
		result = Main::GetDevice()->CreateSamplerState(&trilWrapSampleDesc, &mySamplerStates[SAMPLERSTATE_TRILINEARWRAP]);
		assert(SUCCEEDED(result));

		D3D11_SAMPLER_DESC trilClampSampleDesc = {};
		trilClampSampleDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		trilClampSampleDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		trilClampSampleDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		trilClampSampleDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		trilClampSampleDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		trilClampSampleDesc.MinLOD = -FLT_MAX;
		trilClampSampleDesc.MaxLOD = FLT_MAX;

		result = Main::GetDevice()->CreateSamplerState(&trilClampSampleDesc, &mySamplerStates[SAMPLERSTATE_TRILINEARCLAMP]);
		assert(SUCCEEDED(result));

		mySamplerStates[SAMPLERSTATE_TRILINEAR] = nullptr;
	}

	void RenderUtils::CreateDepthStates()
	{
		HRESULT result;

		D3D11_DEPTH_STENCIL_DESC readonlyDesc = {};
		readonlyDesc.DepthEnable = true;
		readonlyDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		readonlyDesc.DepthFunc = D3D11_COMPARISON_LESS;
		readonlyDesc.StencilEnable = false;

		result = Main::GetDevice()->CreateDepthStencilState(&readonlyDesc, &myDepthStates[DEPTHSTATE_READONLY]);
		assert(SUCCEEDED(result));

		myDepthStates[DEPTHSTATE_DEFUALT] = nullptr;
	}
}

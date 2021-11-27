#include "pch.h"
#include "RenderManager.h"
#include "Utils/RenderUtils.h"
#include "FullscreenEffects/FullscreenEffect.h"
#include "Core/DirectXFramework.h"

namespace Dynamo
{
	RenderManager::RenderManager()
	{
		RenderUtils::Create();

		CreateTextures();

		myGammaCorrectionShader = ShaderFactory::GetShader("Shaders/FullscreenPS-GammaCorrection.cso", ShaderType::PixelShader);
	}
	
	RenderManager::~RenderManager()
	{
		RenderUtils::Destroy();
	}

	void RenderManager::AddMesh(MeshRenderer* aMeshRenderer)
	{
		myModels.Add(aMeshRenderer);
	}

	void RenderManager::RemoveMesh(MeshRenderer* aMeshRenderer)
	{
		myModels.TryRemove(aMeshRenderer);
	}

	void RenderManager::AddDirectionalLight(DirectionalLight* aDirLight)
	{
		myDirLights.Add(aDirLight);
	}

	void RenderManager::RemoveDirectionalLight(DirectionalLight* aDirLight)
	{
		myDirLights.TryRemove(aDirLight);
	}

	void RenderManager::AddAmbientLight(AmbientLight* anAmbLight)
	{
		myAmbLights.Add(anAmbLight);
	}

	void RenderManager::RemoveAmbientLight(AmbientLight* anAmbLight)
	{
		myAmbLights.TryRemove(anAmbLight);
	}

	void RenderManager::AddPointLight(PointLight* aPointLight)
	{
		myPointLights.Add(aPointLight);
	}

	void RenderManager::RemovePointLight(PointLight* aPointLight)
	{
		myPointLights.TryRemove(aPointLight);
	}

	void RenderManager::AddSpotLight(SpotLight* aSpotLight)
	{
		mySpotLights.Add(aSpotLight);
	}

	void RenderManager::RemoveSpotLight(SpotLight* aSpotLight)
	{
		mySpotLights.TryRemove(aSpotLight);
	}

	void RenderManager::AddFullscreenEffect(FullscreenEffect* anEffect, const int aLayer)
	{
		myFullscreenEffects[aLayer].push_back(anEffect);
	}

	void RenderManager::RemoveFullscreenEffect(FullscreenEffect* anEffect)
	{
		for (auto it : myFullscreenEffects)
		{
			for(int i = 0; i < it.second.size(); ++i)
			{
				if (it.second[i] = anEffect)
				{
					it.second.erase(it.second.begin() + i);
					return;
				}
			}
		}
	}

	void RenderManager::Update()
	{
		if (Input::IsKeyDown(KeyCode::F6)) 
		{
			myRenderPass++;

			if (myRenderPass > 6)
			{
				myRenderPass = 0;
			}
		}
	}

	void RenderManager::Render()
	{
		ImGuiRender();

		ClearTextures();
		SetSamplers();

		if (myRenderDeferred)
		{
			RenderDeferred();
			GammaCorrection();
		}
		else
		{
			RenderForward();
		}

		if(myRenderEffects)
			RenderFullscreenEffects();

		RenderDeferredPass();
		RenderToBackBuffer();
	}

	FullscreenRenderer& RenderManager::GetFullscreenRenderer()
	{
		return myFullscreenRenderer;
	}

	void RenderManager::ImGuiRender()
	{
		Debug::ImGui("RenderManager", [this]()
			{
				ImGui::Checkbox("Deferred", &myRenderDeferred);
				ImGui::Checkbox("Fullscreen Effects", &myRenderEffects);
			});
	}

	void RenderManager::SetSamplers()
	{
		RenderUtils::SetSamplerState(RenderUtils::SamplerState::SAMPLERSTATE_TRILINEAR, DEFAULT_SAMPLER_SLOT);
		RenderUtils::SetSamplerState(RenderUtils::SamplerState::SAMPLERSTATE_POINT, POINT_SAMPLER_SLOT);
		RenderUtils::SetSamplerState(RenderUtils::SamplerState::SAMPLERSTATE_TRILINEARCLAMP, CLAMP_SAMPLER_SLOT);
		RenderUtils::SetSamplerState(RenderUtils::SamplerState::SAMPLERSTATE_TRILINEARWRAP, WRAP_SAMPLER_SLOT);
	}

	void RenderManager::RenderDeferred()
	{
		RenderUtils::SetBlendState(RenderUtils::BlendState::BLENDSTATE_DISABLE);
		
		myGBuffer.SetAsActiveTarget(&myRenderDepth);
		myDeferredRenderer.GenerateGBuffer(myModels);
		
		RenderUtils::SetBlendState(RenderUtils::BlendState::BLENDSTATE_ADDITIVE);
		myRenderTexture.SetAsActiveTarget();
		myGBuffer.SetAllAsResources();
		myDeferredRenderer.Render(myDirLights, myAmbLights, myPointLights, mySpotLights);
	}

	void RenderManager::RenderForward()
	{
		RenderUtils::SetBlendState(RenderUtils::BlendState::BLENDSTATE_DISABLE);

		myRenderTexture.SetAsActiveTarget(&myRenderDepth);
		myForwardRenderer.Render(myModels, myDirLights, myAmbLights, myPointLights, mySpotLights);
	}

	void RenderManager::RenderFullscreenEffects()
	{
		for (auto& it : myFullscreenEffects)
		{
			for (auto& effect : it.second)
			{
				effect->Render(myFullscreenRenderer, myRenderTexture);
			}
		}
	}

	void RenderManager::RenderDeferredPass()
	{
		RenderUtils::SetBlendState(RenderUtils::BLENDSTATE_DISABLE);
		if (myRenderPass != 0)
		{
			myRenderTexture.SetAsActiveTarget();
			myGBuffer.SetAsResourceOnSlot(GBuffer::GBufferTexture::ALBEDO, 0);
			myGBuffer.SetAsResourceOnSlot(GBuffer::GBufferTexture::MATERIAL, 1);
			myGBuffer.SetAsResourceOnSlot(GBuffer::GBufferTexture::NORMAL, 2);
			myGBuffer.SetAsResourceOnSlot(GBuffer::GBufferTexture::AMBIENTOCCLUSION, 3);
			myDeferredRenderer.DrawRenderPass(myRenderPass);
		}
	}

	void RenderManager::RenderToBackBuffer()
	{
		myBackBuffer.SetAsActiveTarget();
		myRenderTexture.SetAsResourceOnSlot(FS_TEXTURE_SLOT1);
		myFullscreenRenderer.RenderCopy();
	}

	void RenderManager::GammaCorrection()
	{
		myIntermediateTexture.SetAsActiveTarget();
		myRenderTexture.SetAsResourceOnSlot(FS_TEXTURE_SLOT1);
		myFullscreenRenderer.Render(myGammaCorrectionShader);

		myRenderTexture.SetAsActiveTarget();
		myIntermediateTexture.SetAsResourceOnSlot(FS_TEXTURE_SLOT1);
		myFullscreenRenderer.RenderCopy();
	}

	void RenderManager::CreateTextures()
	{
		ID3D11Resource* backBufferResource = nullptr;
		Main::GetFramework().GetBackBuffer()->GetResource(&backBufferResource);
		ID3D11Texture2D* backBufferTexture = (ID3D11Texture2D*)backBufferResource;
		assert(backBufferTexture);

		myBackBuffer = TextureFactory::CreateTexture(backBufferTexture);
		myRenderTexture = TextureFactory::CreateTexture(Main::GetWindowResolution(), DXGI_FORMAT_R8G8B8A8_UNORM);
		myIntermediateTexture = TextureFactory::CreateTexture(Main::GetWindowResolution(), DXGI_FORMAT_R8G8B8A8_UNORM);
		myRenderDepth = TextureFactory::CreateDepth(Main::GetWindowResolution(), DXGI_FORMAT_D32_FLOAT);

		myGBuffer = TextureFactory::CreateGBuffer(Main::GetWindowResolution());
	}

	void RenderManager::ClearTextures()
	{
		myRenderTexture.ClearTexture();
		myRenderDepth.ClearDepth();
		myBackBuffer.ClearTexture();
		myGBuffer.ClearTextures();
		myIntermediateTexture.ClearTexture();
	}
}
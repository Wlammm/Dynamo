#include "pch.h"
#include "RenderManager.h"
#include "Utils/RenderUtils.h"
#include "FullscreenEffects/FullscreenEffect.h"
#include "Core/DirectXFramework.h"

#include "Material.h"

#include "FullscreenEffects/HDREffect.h"
#include "FullscreenEffects/BloomEffect.h"
#include "FullscreenEffects/FXAAEffect.h"

namespace Dynamo
{
	RenderManager::RenderManager()
	{
		RenderUtils::Create();

		CreateTextures(true);

		myGammaCorrectionShader = ShaderFactory::GetShader("Shaders/FullscreenPS-GammaCorrection.cso", ShaderType::PixelShader);

		myDebugRenderer.Init(Main::GetFramework());

		AddFullscreenEffect(new Dyn::FXAAEffect(), 1);
		AddFullscreenEffect(new Dyn::HDREffect(), 10);

		myDeferredMeshes.Reserve(1000);
		myForwardMeshes.Reserve(1000);
		myNonDepthTestedMeshes.Reserve(200);
	}
	
	RenderManager::~RenderManager()
	{
		RenderUtils::Destroy();
	}

	void RenderManager::AddMesh(const MeshCommand& aMesh)
	{
		if (!aMesh.myMaterial->myIsDepthTested)
		{
			myNonDepthTestedMeshes.push_back(aMesh);
			return;
		}

		if (aMesh.myMaterial->mySurfaceType == SurfaceType::Opaque)
		{
			myDeferredMeshes.push_back(aMesh);
			return;
		}

		myForwardMeshes.push_back(aMesh);
	}

	void RenderManager::AddDirectionalLight(const DirectionalLightCommand& aDirLight)
	{
		myDirLights.Add(aDirLight);
	}

	void RenderManager::AddAmbientLight(const AmbientLightCommand& anAmbLight)
	{
		myAmbLights.Add(anAmbLight);
	}

	void RenderManager::AddPointLight(const PointLightCommand& aPointLight)
	{
		myPointLights.Add(aPointLight);
	}

	void RenderManager::AddSpotLight(const SpotLightCommand& aSpotLight)
	{
		mySpotLights.Add(aSpotLight);
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

			if (myRenderPass > 7)
			{
				myRenderPass = 0;
			}
		}
	}

	void RenderManager::Render()
	{
		SetSamplers();
		ImGuiRender();

		ClearTextures();
		SetSamplers();

		RenderDeferred();
		GammaCorrection();
		RenderForward();

		if(myRenderEffects)
			RenderFullscreenEffects();

		RenderDebug();

		if(myRenderDeferred)
			RenderDeferredPass();
		RenderToBackBuffer();

		ClearCommands();
	}

	FullscreenRenderer& RenderManager::GetFullscreenRenderer()
	{
		return myFullscreenRenderer;
	}

	DebugRenderer& RenderManager::GetDebugRenderer()
	{
		return myDebugRenderer;
	}

	const Texture& RenderManager::GetDepthTexture() const
	{
		return myRenderDepth;
	}

	ForwardRenderer& RenderManager::GetForwardRenderer()
	{
		return myForwardRenderer;
	}

	Texture& RenderManager::GetMainRenderTarget()
	{
		return myRenderTexture;
	}

	void RenderManager::ReleaseAllTextures()
	{
		myRenderTexture.Release();
		myRenderDepth.Release();
		myBackBuffer.Release();
		myIntermediateTexture.Release();
		myGBuffer.Release();

		for (auto entry : myFullscreenEffects)
		{
			for (auto val : entry.second)
			{
				val->Release();
			}
		}
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
		myDeferredRenderer.GenerateGBuffer(myDeferredMeshes);
		
		RenderUtils::SetBlendState(RenderUtils::BlendState::BLENDSTATE_ADDITIVE);
		myRenderTexture.SetAsActiveTarget();
		myGBuffer.SetAllAsResources();
		myDeferredRenderer.Render(myDirLights, myAmbLights, myPointLights, mySpotLights);
	}

	void RenderManager::RenderForward()
	{
		RenderUtils::SetBlendState(RenderUtils::BlendState::BLENDSTATE_ALPHABLEND);

		myRenderTexture.SetAsActiveTarget(&myRenderDepth);
		myForwardRenderer.Render(myForwardMeshes, myDirLights, myAmbLights, myPointLights, mySpotLights);

		RenderUtils::SetDepthState(RenderUtils::DEPTHSTATE_READONLY);
		myForwardRenderer.Render(myNonDepthTestedMeshes, myDirLights, myAmbLights, myPointLights, mySpotLights);
		RenderUtils::SetDepthState(RenderUtils::DEPTHSTATE_DEFUALT);
	}

	void RenderManager::RenderDebug()
	{
		myRenderTexture.SetAsActiveTarget(&myRenderDepth);
		RenderUtils::SetBlendState(RenderUtils::BLENDSTATE_DISABLE);

		Camera* cam = Main::GetMainCamera();
		if (!cam)
			return;

		myDebugRenderer.RenderDepthTested(*cam);
		myRenderTexture.SetAsActiveTarget();
		myDebugRenderer.Render(*cam);
	}

	void RenderManager::RenderFullscreenEffects()
	{
		RenderUtils::SetBlendState(RenderUtils::BLENDSTATE_DISABLE);
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
			myGBuffer.SetAsResourceOnSlot(GBuffer::GBufferTexture::NORMALTEXTURE, 4);
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
		RenderUtils::SetBlendState(RenderUtils::BLENDSTATE_DISABLE);
		myIntermediateTexture.SetAsActiveTarget();
		myRenderTexture.SetAsResourceOnSlot(FS_TEXTURE_SLOT1);
		myFullscreenRenderer.Render(myGammaCorrectionShader);

		myRenderTexture.SetAsActiveTarget();
		myIntermediateTexture.SetAsResourceOnSlot(FS_TEXTURE_SLOT1);
		myFullscreenRenderer.RenderCopy();
	}

	void RenderManager::CreateTextures(bool isFirstTime)
	{
		ID3D11Resource* backBufferResource = nullptr;
		Main::GetFramework()->GetBackBuffer()->GetResource(&backBufferResource);
		ID3D11Texture2D* backBufferTexture = (ID3D11Texture2D*)backBufferResource;
		assert(backBufferTexture);

		myBackBuffer = TextureFactory::CreateTexture(backBufferTexture);
		myRenderTexture = TextureFactory::CreateTexture(Main::GetWindowResolution(), DXGI_FORMAT_R32G32B32A32_FLOAT);
		myIntermediateTexture = TextureFactory::CreateTexture(Main::GetWindowResolution(), DXGI_FORMAT_R32G32B32A32_FLOAT);
		myRenderDepth = TextureFactory::CreateDepth(Main::GetWindowResolution(), DXGI_FORMAT_D32_FLOAT);

		myGBuffer = TextureFactory::CreateGBuffer(Main::GetWindowResolution());

		if (isFirstTime)
			return;

		for (auto entry : myFullscreenEffects)
		{
			for (auto val : entry.second)
			{
				val->ReInit();
			}
		}
	}

	void RenderManager::ClearTextures()
	{
		myRenderTexture.ClearTexture();
		myRenderDepth.ClearDepth();
		myBackBuffer.ClearTexture();
		myGBuffer.ClearTextures();
		myIntermediateTexture.ClearTexture();
	}

	void RenderManager::ClearCommands()
	{
		myDeferredMeshes.clear();
		myForwardMeshes.clear();
		myNonDepthTestedMeshes.clear();
		myDirLights.clear();
		myAmbLights.clear();
		myPointLights.clear();
		mySpotLights.clear();
	}
}
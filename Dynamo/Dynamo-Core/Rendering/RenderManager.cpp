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

	void RenderManager::Render()
	{
		myRenderTexture.SetAsActiveTarget(&myRenderDepth);

		myForwardRenderer.Render(myModels, myDirLights, myAmbLights, myPointLights, mySpotLights);

		RenderFullscreenEffects();

		RenderToBackBuffer();
	}

	void RenderManager::RenderFullscreenEffects()
	{
		for (auto it : myFullscreenEffects)
		{
			for (auto effect : it.second)
			{
				effect->Render(myFullscreenRenderer, myRenderTexture);
			}
		}
	}

	void RenderManager::RenderToBackBuffer()
	{
		myRenderTexture.SetAsActiveTarget(FS_TEXTURE_SLOT1);
		myBackBuffer.SetAsActiveTarget();
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

		myRenderDepth = TextureFactory::CreateDepth(Main::GetWindowResolution(), DXGI_FORMAT_D32_FLOAT);
	}
}

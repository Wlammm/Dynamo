#pragma once

#include "ForwardRenderer.h"
#include "FullscreenRenderer.h"
#include "DebugRenderer.h"
#include "DeferredRenderer.h"
#include "SelectionRenderer.h"
#include "GBuffer.h"
#include "RenderCommands.hpp"


constexpr float globalPointLightIntensityMultiplier = 10000;
constexpr float globalSpotLightIntensityMultiplier = 10000;

namespace Dynamo
{
	class MeshRenderer;
	class DirectionalLight;
	class AmbientLight;
	class PointLight;
	class SpotLight;
	class FullscreenEffect;

	class RenderManager
	{
	public:
		RenderManager();
		~RenderManager();

		void AddMesh(const MeshCommand& aMesh);
		void AddDirectionalLight(const DirectionalLightCommand& aDirLight);
		void AddAmbientLight(const AmbientLightCommand& anAmbLight);
		void AddPointLight(const PointLightCommand& aPointLight);
		void AddSpotLight(const SpotLightCommand& aSpotLight);

		void AddFullscreenEffect(FullscreenEffect* anEffect, const int aLayer = 0);
		void RemoveFullscreenEffect(FullscreenEffect* anEffect);
		template<typename T>
		T* GetFullscreenEffect();

		void Update();
		void Render();

		FullscreenRenderer& GetFullscreenRenderer();
		DebugRenderer& GetDebugRenderer();
		ForwardRenderer& GetForwardRenderer();

		Texture& GetMainRenderTarget();
		const Texture& GetDepthTexture() const;

		Texture& GetSelectionTexture();

		void ReleaseAllTextures();
		void CreateTextures(bool isFirstTime);

		// Will update selection texture and callback when its updated.
		void SetSelectionCallback(std::function<void(void)> aCallback);

	private:
		void ImGuiRender();
		void SetSamplers();

		void RenderDeferred();
		void RenderForward();
		void RenderDebug();
		void RenderFullscreenEffects();
		void RenderDeferredPass();
		void RenderToBackBuffer();
		void RenderSelection();

		void GammaCorrection();

		void ClearTextures();
		void ClearCommands();

	private:
		bool myRenderDeferred = true;
		bool myRenderEffects = true;

		int myRenderPass = 0;

	private:
		CU::DArray<MeshCommand> myDeferredMeshes;
		CU::DArray<MeshCommand> myForwardMeshes;
		CU::DArray<MeshCommand> myNonDepthTestedMeshes;

		CU::DArray<DirectionalLightCommand> myDirLights;
		CU::DArray<AmbientLightCommand> myAmbLights;
		CU::DArray<PointLightCommand> myPointLights;
		CU::DArray<SpotLightCommand> mySpotLights;

		std::map<int, std::vector<FullscreenEffect*>> myFullscreenEffects;

		ForwardRenderer myForwardRenderer;
		FullscreenRenderer myFullscreenRenderer;
		DeferredRenderer myDeferredRenderer;
		DebugRenderer myDebugRenderer;
		SelectionRenderer mySelectionRenderer;

		Texture myRenderTexture;
		Texture myRenderDepth;
		Texture myBackBuffer;
		Texture myIntermediateTexture;

		Texture mySelectionTexture;

		GBuffer myGBuffer = {};

		Shader* myGammaCorrectionShader = nullptr;

		std::function<void(void)> mySelectionCallback = nullptr;
	};

	template<typename T>
	inline T* RenderManager::GetFullscreenEffect()
	{
		for (auto it : myFullscreenEffects)
		{
			for (int i = 0; i < it.second.size(); ++i)
			{
				if (T* ptr = dynamic_cast<T*>(it.second[i]))
				{
					return ptr;
				}
			}
		}
		return nullptr;
	}
}
#pragma once

#include "ForwardRenderer.h"
#include "FullscreenRenderer.h"
#include "DebugRenderer.h"
#include "DeferredRenderer.h"
#include "GBuffer.h"

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

		void AddMesh(MeshRenderer* aMeshRenderer);
		void RemoveMesh(MeshRenderer* aMeshRenderer);

		void AddDirectionalLight(DirectionalLight* aDirLight);
		void RemoveDirectionalLight(DirectionalLight* aDirLight);

		void AddAmbientLight(AmbientLight* anAmbLight);
		void RemoveAmbientLight(AmbientLight* anAmbLight);

		void AddPointLight(PointLight* aPointLight);
		void RemovePointLight(PointLight* aPointLight);

		void AddSpotLight(SpotLight* aSpotLight);
		void RemoveSpotLight(SpotLight* aSpotLight);

		void AddFullscreenEffect(FullscreenEffect* anEffect, const int aLayer = 0);
		void RemoveFullscreenEffect(FullscreenEffect* anEffect);

		void Update();
		void Render();

		FullscreenRenderer& GetFullscreenRenderer();
		DebugRenderer& GetDebugRenderer();

		Texture& GetMainRenderTarget();
		Texture& GetNonEffectRenderTarget();

	private:
		void ImGuiRender();
		void SetSamplers();

		void RenderDeferred();
		void RenderForward();
		void RenderDebug();
		void RenderFullscreenEffects();
		void RenderNonEffectTexture();
		void RenderDeferredPass();
		void RenderToBackBuffer();

		void GammaCorrection();

		void CreateTextures();
		void ClearTextures();

		// TEMP
	private:
		bool myRenderDeferred = true;
		bool myRenderEffects = true;

		int myRenderPass = 0;

	private:
		CU::DArray<MeshRenderer*> myModels;
		CU::DArray<DirectionalLight*> myDirLights;
		CU::DArray<AmbientLight*> myAmbLights;
		CU::DArray<PointLight*> myPointLights;
		CU::DArray<SpotLight*> mySpotLights;

		std::map<int, std::vector<FullscreenEffect*>> myFullscreenEffects;

		ForwardRenderer myForwardRenderer;
		FullscreenRenderer myFullscreenRenderer;
		DeferredRenderer myDeferredRenderer;
		DebugRenderer myDebugRenderer;

		Texture myRenderTexture;
		Texture myRenderDepth;
		Texture myNonEffectTexture;
		Texture myBackBuffer;

		Texture myIntermediateTexture;

		GBuffer myGBuffer = {};

		Shader* myGammaCorrectionShader = nullptr;
	};
}
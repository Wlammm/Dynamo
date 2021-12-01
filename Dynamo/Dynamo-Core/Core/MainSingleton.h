#pragma once

#include "Shaders/SharedDefines.hpp"

namespace Dynamo
{
	class WindowHandler;
	class DirectXFramework;
	class Scene;
	class Camera;
	class RenderManager;

	class MainSingleton
	{
	public:
		static void Init();
		static void Destroy();
		static void Update();

		// Returns full window size. Including top-bar.
		static const Vec2ui& GetWindowSize();
		static const Vec2ui& GetWindowOffset();

		// Returns screensize in window.
		static const Vec2ui& GetWindowResolution();

		static const Color& GetClearColor();
		static const std::wstring& GetWindowTitle();

		static const HWND& GetHWND();

		static const bool& GetEngineRunning();
		static void SetEngineRunning(const bool& aValue);

		static ID3D11Device* GetDevice();
		static ID3D11DeviceContext* GetContext();

		static WindowHandler& GetWindowHandler();
		static DirectXFramework& GetFramework();

		static Scene* GetScene();
		static void SetScene(Scene* aScene);

		// Overrides the main camera. nullptr to disable.
		static void SetOverrideCamera(Camera* aCamera);

		static Camera* GetMainCamera();

		static RenderManager& GetRenderManager();

 	private:
		const Vec2ui myWindowSize = { WINDOW_RESOLUTION_X, WINDOW_RESOLUTION_Y };
		const Vec2ui myWindowOffset = { 100, 100 };
		const Color myClearColor = { 0, 0, 0, 1 };
		const std::wstring myWindowTitle = L"Dynamo";

		// Being set by WindowHandler::WndProc.
		Vec2ui myWindowResolution = { 0, 0 };
		HWND myWindowHandle;

		bool myEngineRunning = true;

		ID3D11Device* myDevice = nullptr;
		ID3D11DeviceContext* myContext = nullptr;

		WindowHandler* myWindowHandler;
		DirectXFramework* myFramework;
		RenderManager* myRenderManager;

		Scene* myScene = nullptr;
		Camera* myOverrideCamera = nullptr;
		Camera* myMainCamera = nullptr;

	private:
		friend WindowHandler;
		friend class DirectXFramework;

		static MainSingleton* ourInstance;
	};

	using Main = MainSingleton;
}

#pragma once

namespace Dynamo
{
	class WindowHandler;
	class DirectXFramework;
	class Scene;

	class MainSingleton
	{
	public:
		static void Init();

		static const Vec2ui& GetWindowSize();
		static const Vec2ui& GetWindowOffset();
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


	private:
		const Vec2ui myWindowSize = { 1920, 1080 };
		const Vec2ui myWindowOffset = { 100, 100 };
		const Color myClearColor = { 0, 0, 0, 1 };
		const std::wstring myWindowTitle = L"Dynamo";

		// Being set by WindowHandler::WndProc.
		Vec2ui myWindowResolution = { 0, 0 };
		HWND myWindowHandle;

		bool myEngineRunning = true;

		ID3D11Device* myDevice = nullptr;
		ID3D11DeviceContext* myContext = nullptr;

		std::unique_ptr<WindowHandler> myWindowHandler;
		std::unique_ptr<DirectXFramework> myFramework;

		Scene* myScene = nullptr;

	private:
		friend WindowHandler;
		friend class DirectXFramework;

		static MainSingleton* ourInstance;
	};

	using Main = MainSingleton;
}

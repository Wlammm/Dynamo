#pragma once
#include "InputManager.h"

namespace Dynamo
{
	class Input
	{
	public:
		static void Create();
		static void Destroy();

		// The frame the user pressed the button.
		static bool IsKeyDown(const int aKeyCode);
		// The frame the user pressed the button.
		static bool IsKeyDown(const KeyCode aKeyCode);
		// The frame the user pressed the button.
		static bool IsKeyDown(const MouseButton aMouseButton);
		// The frame the user pressed the button.
		static bool IsMouseDown(const MouseButton aMouseButton);

		// The frame the user released the button.
		static bool IsKeyUp(const int aKeyCode);
		// The frame the user released the button.
		static bool IsKeyUp(const KeyCode aKeyCode);
		// The frame the user released the button.
		static bool IsMouseUp(const MouseButton aMouseButton);
		// The frame the user released the button.
		static bool IsKeyUp(const MouseButton aMouseButton);

		// The frames the user is pressing the button.
		static bool IsKeyPressed(const int aKeyCode);
		// The frames the user is pressing the button.
		static bool IsKeyPressed(const KeyCode aKeyCode);
		// The frames the user is pressing the button.
		static bool IsKeyPressed(const MouseButton aMouseButton);
		// The frames the user is pressing the button.
		static bool IsMousePressed(const MouseButton aMouseButton);

		// Mouse position relative to the entire computer screen
		static CU::Vector2ui GetMousePosition();
		// Mouse position relative to the client screen
		static CU::Vector2ui GetMousePositionClientRelative();
		// Normalized mouse position relative to client screen.
		static CU::Vector2f GetMousePositionClientNormalized();

		static CU::InputManager& GetManager();

	private:
		void CreateInternal();

	private:
		static Input* myInstance;

		std::unique_ptr<CU::InputManager> myInputManager;
	};
}


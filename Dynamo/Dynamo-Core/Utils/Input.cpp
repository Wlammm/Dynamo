#include "pch.h"
#include "Input.h"

Dynamo::Input* Dynamo::Input::myInstance = nullptr;

void Dynamo::Input::Create()
{
	myInstance = new Input;
	myInstance->CreateInternal();
}

void Dynamo::Input::CreateInternal()
{
	myInputManager = std::make_unique<CU::InputManager>();
}

void Dynamo::Input::Destroy()
{
	delete myInstance;
	myInstance = nullptr;
}

bool Dynamo::Input::IsKeyDown(const int aKeyCode)
{
	return myInstance->myInputManager->IsKeyDown(aKeyCode);
}

bool Dynamo::Input::IsKeyDown(const KeyCode aKeyCode)
{
	return myInstance->IsKeyDown(static_cast<int>(aKeyCode));
}

bool Dynamo::Input::IsKeyDown(const MouseButton aMouseButton)
{
	return myInstance->IsKeyDown(static_cast<int>(aMouseButton));
}

bool Dynamo::Input::IsMouseDown(const MouseButton aMouseButton)
{
	return IsKeyDown(aMouseButton);
}

bool Dynamo::Input::IsKeyUp(const int aKeyCode)
{
	return myInstance->myInputManager->IsKeyUp(aKeyCode);
}

bool Dynamo::Input::IsKeyUp(const KeyCode aKeyCode)
{
	return myInstance->IsKeyUp(static_cast<int>(aKeyCode));
}

bool Dynamo::Input::IsKeyUp(const MouseButton aMouseButton)
{
	return myInstance->IsKeyUp(static_cast<int>(aMouseButton));
}

bool Dynamo::Input::IsMouseUp(const MouseButton aMouseButton)
{
	return IsKeyUp(aMouseButton);
}

bool Dynamo::Input::IsKeyPressed(const int aKeyCode)
{
	return myInstance->myInputManager->IsKeyPressed(aKeyCode);
}

bool Dynamo::Input::IsKeyPressed(const KeyCode aKeyCode)
{
	return myInstance->IsKeyPressed(static_cast<int>(aKeyCode));
}

bool Dynamo::Input::IsKeyPressed(const MouseButton aMouseButton)
{
	return myInstance->IsKeyPressed(static_cast<int>(aMouseButton));
}

bool Dynamo::Input::IsMousePressed(const MouseButton aMouseButton)
{
	return IsKeyPressed(aMouseButton);
}

CU::Vector2ui Dynamo::Input::GetMousePosition()
{
	return CU::Vector2ui{
		static_cast<unsigned int>(myInstance->myInputManager->GetMousePosition().x),
		static_cast<unsigned int>(myInstance->myInputManager->GetMousePosition().y)
	};
}

CU::Vector2ui Dynamo::Input::GetMousePositionClientRelative()
{
	return CU::Vector2ui{
		static_cast<unsigned int>(myInstance->myInputManager->GetMousePositionClientRelative().x),
		static_cast<unsigned int>(myInstance->myInputManager->GetMousePositionClientRelative().y)
	};
}

CU::Vector2f Dynamo::Input::GetMousePositionClientNormalized()
{
	CU::Vector2f pixelPos = {
		static_cast<float>(GetMousePositionClientRelative().x),
		static_cast<float>(GetMousePositionClientRelative().y) };

	CU::Vector2f floatResolution = {
		static_cast<float>(Main::GetWindowResolution().x),
		static_cast<float>(Main::GetWindowResolution().y) };

	return (pixelPos / floatResolution);
}

CU::InputManager& Dynamo::Input::GetManager()
{
	return *(myInstance->myInputManager);
}

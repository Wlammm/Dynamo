#include "Component.h"

void Component::SetActive(const bool aState)
{
	if (!aState && myIsActive)
	{
		OnDisable();
	}

	if (aState && !myIsActive)
	{
		OnEnable();
	}

	myIsActive = aState;
}

const bool Component::IsActive() const
{
	return myIsActive;
}

GameObject* Component::GetGameObject() const
{
	return myGameObject;
}

Dynamo::Transform& Component::GetTransform()
{
	return myGameObject->GetTransform();
}

void Component::Update() { }

void Component::LateUpdate() { }

void Component::OnCreate() { }

void Component::OnDestroy() { }

void Component::OnEnable() { }

void Component::OnDisable() { }

void Component::OnCollision(GameObject* anOther) { }

void Component::OnTriggerEnter(GameObject* anOther) { }

void Component::OnTriggerExit(GameObject* anOther) { }

void Component::OnTrigger(GameObject* anOther) { }
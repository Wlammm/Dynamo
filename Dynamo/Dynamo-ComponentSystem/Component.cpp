#include "Component.h"

Component::Component(const std::string& aCategory)
{
	myCategory = aCategory;
}

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

void Component::EditorUpdate() { }

void Component::OnCreate() { }

void Component::OnDestroy() { }

void Component::OnEnable() { }

void Component::OnDisable() { }

void Component::ExposeValues() { }

nlohmann::json Component::Save()
{
	nlohmann::json json;
	json["INFO"] = "Component::Save() not overriden.";
	return json;
}

void Component::Load(nlohmann::json& aJson) { }

ComponentName Component::GetName()
{
	return std::string();
}

TypeID Component::GetTypeID()
{
	return std::string();
}

int Component::GetTypeSize()
{
	return 0;
}

const std::string& Component::GetCategory() const
{
	return myCategory;
}

void Component::OnCollision(GameObject* anOther) 
{ 
	anOther;
}

void Component::OnTriggerEnter(GameObject* anOther) 
{ 
	anOther;
}

void Component::OnTriggerExit(GameObject* anOther) 
{ 
	anOther;
}

void Component::OnTrigger(GameObject* anOther) 
{ 
	anOther;
}

void Component::RegisterToAdmin(ComponentAdmin& anAdmin)
{
	// Component not defines. Use COMP(className) in component header.
	throw std::exception();
}
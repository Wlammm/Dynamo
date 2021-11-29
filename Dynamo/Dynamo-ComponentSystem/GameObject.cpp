#include "GameObject.h"
#include "Component.h"
#include "ComponentAdmin.h"

const bool GameObject::IsActive() const
{
	return myIsActive;
}

const std::string& GameObject::GetName() const
{
	return myName;
}

void GameObject::SetTag(const Tag aTag)
{
	myTag = aTag;
}

const Tag GameObject::GetTag() const
{
	return myTag;
}

const size_t& GameObject::GetGameObjectID() const
{
	return myID;
}

void GameObject::Reset()
{
	myIsActive = true;
	myName = "Unnamed GameObject";
	myTag = Tag::Untagged;
	myTransform = nullptr;
}

void GameObject::SetActive(bool isActive)
{
	myIsActive = isActive;
	myAdmin->SetActive(this, isActive);
}

void GameObject::SetName(const std::string& aName)
{
	myName = aName;
}

Dynamo::Transform& GameObject::GetTransform()
{
	assert(myTransform != nullptr && "GameObject not initialized.");
	return *myTransform;
}
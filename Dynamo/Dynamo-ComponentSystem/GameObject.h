#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include "Types.h"
#include "Tag.hpp"
#include "ComponentAdmin.h"

class Component;
class GameObject;

namespace Dynamo
{
	class Transform;
	class Scene;
}

class GameObject
{
public:
	static void Destroy(GameObject* anObject, const float aTime = 0);

public:
	GameObject() = default;
	~GameObject() = default;

	void operator delete(void*) = delete;

	void SetActive(bool isActive);
	const bool IsActive() const;

	void SetName(const std::string& aName);
	const std::string& GetName() const;

	void SetTag(const Tag aTag);
	const Tag GetTag() const;

	const GameObjectID& GetGameObjectID() const;

	template<typename T, typename... Args>
	T* AddComponent(Args&&... params)
	{
		return myAdmin->AddComponent<T>(myID, std::forward<Args>(params)...);
	}

	template<typename T>
	T* GetComponent()
	{
		return myAdmin->GetComponent<T>(myID);
	}

	template<typename T>
	void RemoveComponent()
	{
		myAdmin->RemoveComponent<T>(myID);
	}

	Dynamo::Transform& GetTransform();

	std::vector<Component*> GetAllComponents();

	ComponentAdmin* GetAdmin();

private:
	// Used to set private variables.
	friend class ComponentAdmin;
	friend Dynamo::Scene;

	void Reset();

	GameObjectID myID = 0;
	std::string myName = "Unnamed GameObject";
	Tag myTag = Tag::Untagged;
	Dynamo::Transform* myTransform = nullptr;

	bool myIsActive = true;

	ComponentAdmin* myAdmin = nullptr;
};
#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include "Types.h"
#include "Tag.hpp"
#include "ComponentAdmin.h"

class Component;
class GameObject;


class GameObject
{
public:
	GameObject() = default;
	~GameObject() = default;

	void operator delete(void*) = delete;
	
	static GameObject* Instantiate()
	{
		return ComponentAdmin::GetInstance()->CreateGameObject();
	}
	
	static void Destroy(GameObject* anObject, const float aTime = 0)
	{
		ComponentAdmin::GetInstance()->RemoveGameObject(anObject, aTime);
	}

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
		return ComponentAdmin::GetInstance()->AddComponent<T>(myID, std::forward<Args>(params)...);
	}

	template<typename T>
	T* GetComponent()
	{
		return ComponentAdmin::GetInstance()->GetComponent<T>(myID);
	}

	template<typename T>
	void RemoveComponent()
	{
		ComponentAdmin::GetInstance()->RemoveComponent<T>(myID);
	}

private:
	// Used to set private variables.
	friend class ComponentAdmin;
	void Reset();

	GameObjectID myID = -1;
	std::string myName = "";
	Tag myTag = Tag::Untagged;

	bool myIsActive = true;
};
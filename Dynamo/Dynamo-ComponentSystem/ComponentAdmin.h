#pragma once

#include <string>
#include <unordered_map>
#include <map>
#include "Types.h"
#include "ComponentManager.hpp"
#include <queue>
#include <unordered_set>

class GameObject;
class Component;

class ComponentAdmin
{
public:
	ComponentAdmin() = default;
	~ComponentAdmin();

	void Init();

	void Update(const float aDeltaTime);
	void UpdateComponents();
	void LateUpdateComponents();
	void EditorUpdateComponents();

	void SetActive(GameObject* ob, const bool aState);

	template<typename T, typename... Args>
	T* AddComponent(const GameObjectID aID, Args&&... params)
	{
		GameObject* toLookFor = &myBase[aID];
		assert(std::find(myActiveGameObjects.begin(), myActiveGameObjects.end(), toLookFor) != myActiveGameObjects.end() && "Trying to add component to non active gameobject.");

		T* comp =  myComponentManager.AddComponent<T>(aID, std::forward<Args>(params)...);
		comp->myGameObject = &myBase[aID];
		comp->myIsActive = true;
		comp->myAdmin = this;
		comp->OnCreate();
		return comp;
	}

	template<typename T>
	T* GetComponent(const GameObjectID aID)
	{
		assert(myActiveIDS.count(aID) != 0 && "GameObject is not currently active.");

		return myComponentManager.GetComponent<T>(aID);
	}

	template<typename T>
	void RemoveComponent(const GameObjectID aID)
	{
		std::string typeName = typeid(T).name();
		myComponentsToRemove.push_back({ typeName, aID });
	}

	GameObject* CreateGameObject();
	void RemoveGameObject(GameObject* anObject, const float aTime);

	const std::vector<GameObject*>& GetAllGameObjects() const;

	template<typename T>
	const std::vector<GameObject*> GetGameObjectsWithComponent()
	{
		std::vector<GameObjectID> gameObjectIDSWithComponent = myComponentManager.GetGameObjectsWithComponent<T>();

		std::vector<GameObject*> gameObjects;
		for (int i = 0; i < gameObjectIDSWithComponent.size(); ++i)
		{
			gameObjects.push_back(&myBase[gameObjectIDSWithComponent[i]]);
		}

		return gameObjects;
	}

	std::vector<Component*> GetAllComponentsOnGameObject(GameObjectID aGO);

	ComponentManager& GetComponentManager();

	const std::string& GetComponentName(const std::string& aTypeID);

private:
	std::unordered_set<GameObjectID> myActiveIDS;

	GameObject* myBase;
	std::queue<GameObject*> myGameObjects;
	std::vector<GameObject*> myActiveGameObjects;

	std::map<GameObject*, float> myGameObjectsToBeDeleted;

	std::vector<std::pair<std::string, GameObjectID>> myComponentsToRemove;

	ComponentManager myComponentManager;
};
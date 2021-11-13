#pragma once

#include "ComponentArray.hpp"
#include "Types.h"
#include <any>
#include <memory>
#include <unordered_map>
#include <string>

class ComponentManager
{
public:
	template<typename T, typename... Args>
	T* AddComponent(const GameObjectID anEntity, Args&&... params)
	{
		std::string typeName = typeid(T).name();
		if (myComponentTypes.find(typeName) == myComponentTypes.end())
		{
			RegisterComponent<T>();
		}

		return GetComponentArray<T>()->InsertData(anEntity, std::forward<Args>(params)...);
	}

	void RemoveComponent(const std::string& aTypeName, const GameObjectID aID)
	{
		myComponentArrays[aTypeName]->RemoveData(aID);
	}

	template<typename T>
	const bool HasComponent(const GameObjectID anEntity)
	{
		std::string typeName = typeid(T).name();
		if (myComponentTypes.find(typeName) == myComponentTypes.end())
		{
			RegisterComponent<T>();
		}

		return GetComponentArray<T>()->HasData(anEntity);
	}

	template<typename T>
	T* GetComponent(const GameObjectID anEntity)
	{
		return GetComponentArray<T>()->GetData(anEntity);
	}

	template<typename T>
	std::vector<GameObjectID> GetGameObjectsWithComponent()
	{
		return GetComponentArray<T>()->GetGameObjects();
	}

	void EntityDestroyed(const GameObjectID anEntity)
	{
		for (auto const& pair : myComponentArrays)
		{
			auto const& component = pair.second;

			component->GameObjectDestroyed(anEntity);
		}
	}

	void Update()
	{
		for (auto& comp : myComponentArrays)
		{
			comp.second->Update();
		}
	}

	void LateUpdate()
	{
		for (auto& comp : myComponentArrays)
		{
			comp.second->LateUpdate();
		}
	}

	void SetActive(GameObjectID anEntity, const bool aState)
	{
		for (auto& comp : myComponentArrays)
		{
			comp.second->SetActive(anEntity, aState);
		}
	}

private:
	std::unordered_map<std::string, ComponentType> myComponentTypes{};
	std::unordered_map<std::string, std::shared_ptr<IComponentArray>> myComponentArrays{};
	ComponentType myNextComponentType{ 1 };

	template<typename T>
	std::shared_ptr<ComponentArray<T>> GetComponentArray()
	{
		std::string typeName = typeid(T).name();

		assert(myComponentTypes.find(typeName) != myComponentTypes.end() && "Component not registered.");

		return std::static_pointer_cast<ComponentArray<T>>(myComponentArrays[typeName]);
	}

	template<typename T>
	void RegisterComponent()
	{
		std::string typeName = typeid(T).name();

		assert(myComponentTypes.find(typeName) == myComponentTypes.end() && "Component type already registered.");

		myComponentTypes.insert({ typeName, myNextComponentType });
		myComponentArrays.insert({ typeName, std::make_shared<ComponentArray<T>>() });

		++myNextComponentType;
	}
};

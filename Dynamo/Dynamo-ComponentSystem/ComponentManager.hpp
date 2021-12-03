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
		return GetComponentArray<T>()->InsertData(anEntity, std::forward<Args>(params)...);
	}

	void* AddComponentWithTypeID(const TypeID& aComponentType, const GameObjectID anEntity)
	{
		if (myComponentArrays.find(aComponentType) != myComponentArrays.end())
		{
			return myComponentArrays[aComponentType]->InsertDataWithTypeID(anEntity);
		}

		// Unregistered component.
		throw std::exception();
		return nullptr;
	}

	void RemoveComponent(const std::string& aTypeName, const GameObjectID aID)
	{
		myComponentArrays[aTypeName]->RemoveData(aID);
	}

	template<typename T>
	const bool HasComponent(const GameObjectID anEntity)
	{
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

	void EditorUpdate()
	{
		for (auto& comp : myComponentArrays)
		{
			comp.second->EditorUpdate();
		}
	}

	void SetActive(GameObjectID anEntity, const bool aState)
	{
		for (auto& comp : myComponentArrays)
		{
			comp.second->SetActive(anEntity, aState);
		}
	}

	template<typename T>
	void RegisterComponent(const std::string& aName)
	{
		std::string typeID = typeid(T).name();

		if (myComponentArrays.find(typeID) == myComponentArrays.end())
		{
			myComponentArrays.insert({ typeID, std::make_shared<ComponentArray<T>>() });
		}
	}

	std::vector<void*> GetAllComponentsOnGameObject(GameObjectID aGo)
	{
		std::vector<void*> components;
		for (const auto& val : myComponentArrays)
		{
			void* comp = val.second->GetComponentOnGameObject(aGo);

			if (comp)
			{
				components.push_back(comp);
			}
		}
		return components;
	}

private:
	std::unordered_map<std::string, std::shared_ptr<IComponentArray>> myComponentArrays{};

	template<typename T>
	std::shared_ptr<ComponentArray<T>> GetComponentArray()
	{
		std::string typeName = typeid(T).name();

		if (myComponentArrays.find(typeName) == myComponentArrays.end())
		{
			myComponentArrays.insert({ typeName, std::make_shared<ComponentArray<T>>() });
		}

		return std::static_pointer_cast<ComponentArray<T>>(myComponentArrays[typeName]);
	}
};

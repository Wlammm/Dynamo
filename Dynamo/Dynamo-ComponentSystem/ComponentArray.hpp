#pragma once
#include <assert.h>
#include <array>
#include <unordered_map>
#include "Types.h"

class IComponentArray
{
public:
	virtual ~IComponentArray() = default;
	virtual void GameObjectDestroyed(GameObjectID anEntity) = 0;
	virtual void Update() = 0;
	virtual void LateUpdate() = 0;
	virtual void SetActive(GameObjectID anEntity, const bool aState) = 0;
	virtual void RemoveData(GameObjectID anEntity) = 0;
};

template<typename T>
class ComponentArray : public IComponentArray
{
public:
	template<typename... Args>
	T* InsertData(GameObjectID anEntity, Args&&... params)
	{
		assert(myEntityToIndexMap.find(anEntity) == myEntityToIndexMap.end() && "Component already exists on gameobject.");

		T aComponent = T(std::forward<Args>(params)...);

		int newIndex = mySize;
		myEntityToIndexMap[anEntity] = newIndex;
		myIndexToEntityMap[newIndex] = anEntity;
		myComponentArray[newIndex] = aComponent;
		mySize++;

		myComponentArray[newIndex].OnCreate();

		return &myComponentArray[newIndex];
	}

	void RemoveData(GameObjectID anEntity) override
	{
		assert(myEntityToIndexMap.find(anEntity) != myEntityToIndexMap.end() && "GameObjectID does not exists.");

		int indexOfRemovedGameObjectID  = myEntityToIndexMap[anEntity];
		int indexOfLastElement = mySize - 1;

		myComponentArray[indexOfRemovedGameObjectID].OnDestroy();

		myComponentArray[indexOfRemovedGameObjectID] = myComponentArray[indexOfLastElement];

		GameObjectID entityOfLastElement = myIndexToEntityMap[indexOfLastElement];
		myEntityToIndexMap[entityOfLastElement] = indexOfRemovedGameObjectID;
		myIndexToEntityMap[indexOfRemovedGameObjectID] = entityOfLastElement;

		myEntityToIndexMap.erase(anEntity);
		myIndexToEntityMap.erase(indexOfLastElement);
	}

	T* GetData(GameObjectID anEntity)
	{
		assert(myEntityToIndexMap.find(anEntity) != myEntityToIndexMap.end() && "Component does not exist on gameobject.");

		return &myComponentArray[myEntityToIndexMap[anEntity]];
	}
	
	const bool HasData(GameObjectID anEntity)
	{
		return (myEntityToIndexMap.find(anEntity) != myEntityToIndexMap.end());
	}

	void GameObjectDestroyed(GameObjectID anEntity) override
	{
		if (myEntityToIndexMap.find(anEntity) != myEntityToIndexMap.end())
		{
			RemoveData(anEntity);
		}
	}

	void Update() override
	{
		for (auto& e : myEntityToIndexMap)
		{
			if (myComponentArray[e.second].IsActive())
			{
				myComponentArray[e.second].Update();
			}
		}
	}

	void LateUpdate() override
	{
		for (auto& e : myEntityToIndexMap)
		{
			if (myComponentArray[e.second].IsActive())
			{
				myComponentArray[e.second].LateUpdate();
			}
		}
	}

	void SetActive(GameObjectID anEntity, const bool aState)
	{
		if (myEntityToIndexMap.find(anEntity) != myEntityToIndexMap.end())
		{
			myComponentArray[myEntityToIndexMap[anEntity]].SetActive(aState);
		}
	}

	std::vector<GameObjectID> GetGameObjects()
	{
		std::vector<GameObjectID> returnVec;

		for (auto& e : myEntityToIndexMap)
		{
			returnVec.push_back(e.first);
		}

		return returnVec;
	}

private:
	std::array<T, MAX_GAMEOBJECTS> myComponentArray{};
	std::unordered_map<GameObjectID, int> myEntityToIndexMap{};
	std::unordered_map<int, GameObjectID> myIndexToEntityMap{};
	int mySize{};
};
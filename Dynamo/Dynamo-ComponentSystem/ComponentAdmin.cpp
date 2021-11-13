#include "ComponentAdmin.h"
#include "GameObject.h"
#include "Component.h"

ComponentAdmin* ComponentAdmin::ourInstance = nullptr;

ComponentAdmin::ComponentAdmin()
{
	ourInstance = this;
}

ComponentAdmin::~ComponentAdmin()
{
	ourInstance = nullptr;

	delete[] myBase;
	myBase = nullptr;
}

void ComponentAdmin::Init()
{
	myBase = new GameObject[MAX_GAMEOBJECTS];

	for (int i = 0; i < MAX_GAMEOBJECTS; ++i)
	{
		myGameObjects.push(&myBase[i]);
		myBase[i].myID = i;
	}
}

void ComponentAdmin::Update()
{
	for (const auto& comp : myComponentsToRemove)
	{
		myComponentManager.RemoveComponent(comp.first, comp.second);
	}
	myComponentsToRemove.clear();

	std::vector<GameObject*> removedKeys;
	for (auto& val : myGameObjectsToBeDeleted)
	{
		val.second -= 0; // REPLACE WITH DELTATIME

		if (val.second <= 0)
		{
			myGameObjects.push(val.first);
			myActiveGameObjects.erase(std::remove(myActiveGameObjects.begin(), myActiveGameObjects.end(), val.first));
			myComponentManager.EntityDestroyed(val.first->GetGameObjectID());
			removedKeys.push_back(val.first);
		}
	}
	for (int i = 0; i < removedKeys.size(); ++i)
	{
		myGameObjectsToBeDeleted.erase(removedKeys[i]);
	}

	myComponentsToRemove.clear();

	myComponentManager.Update();
}

void ComponentAdmin::LateUpdate()
{
	myComponentManager.LateUpdate();
}

void ComponentAdmin::SetActive(GameObject* ob, const bool aState)
{
	myComponentManager.SetActive(ob->GetGameObjectID(), aState);
}

GameObject* ComponentAdmin::CreateGameObject()
{
	assert(myGameObjects.size() > 0 && "All gameobjects in use. Try increasing MAX_GAMEOBJECTS in types.h");

	GameObject* object = myGameObjects.front();
	myGameObjects.pop();

	object->Reset();
	myActiveGameObjects.push_back(object);
	return object;
}

void ComponentAdmin::RemoveGameObject(GameObject* anObject, const float aTime)
{
	assert(std::find(myActiveGameObjects.begin(), myActiveGameObjects.end(), anObject) != myActiveGameObjects.end() && "Gameobject is not currently active.");

	myGameObjectsToBeDeleted[anObject] = aTime;
}

const std::vector<GameObject*>& ComponentAdmin::GetAllGameObjects() const
{
	return myActiveGameObjects;
}

ComponentAdmin* ComponentAdmin::GetInstance()
{
	return ourInstance;
}
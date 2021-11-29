#include "ComponentAdmin.h"
#include "GameObject.h"
#include "Component.h"

ComponentAdmin::~ComponentAdmin()
{
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
		myBase[i].myAdmin = this;
	}
}

void ComponentAdmin::Update(const float aDeltaTime)
{
	for (const auto& comp : myComponentsToRemove)
	{
		myComponentManager.RemoveComponent(comp.first, comp.second);
	}
	myComponentsToRemove.clear();

	std::vector<GameObject*> removedKeys;
	for (auto& val : myGameObjectsToBeDeleted)
	{
		val.second -= aDeltaTime;

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
}

void ComponentAdmin::UpdateComponents()
{
	myComponentManager.Update();
}

void ComponentAdmin::LateUpdateComponents()
{
	myComponentManager.LateUpdate();
}

void ComponentAdmin::EditorUpdateComponents()
{
	myComponentManager.EditorUpdate();
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
	object->myAdmin = this;

	myActiveIDS.insert(object->GetGameObjectID());
	return object;
}

void ComponentAdmin::RemoveGameObject(GameObject* anObject, const float aTime)
{
	assert(std::find(myActiveGameObjects.begin(), myActiveGameObjects.end(), anObject) != myActiveGameObjects.end() && "Gameobject is not currently active.");
	myActiveIDS.erase(anObject->GetGameObjectID());
	myGameObjectsToBeDeleted[anObject] = aTime;
}

const std::vector<GameObject*>& ComponentAdmin::GetAllGameObjects() const
{
	return myActiveGameObjects;
}
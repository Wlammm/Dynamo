#include "pch.h"
#include "Scene.h"
#include <ComponentAdmin.h>
#include "Components/Transform.h"

namespace Dynamo
{
    GameObject* Scene::CreateGameObject()
    {
        GameObject* go = myComponentAdmin->CreateGameObject();
        Transform* trans = go->AddComponent<Transform>();
        go->myTransform = trans;
        go->myAdmin = myComponentAdmin.get();
        return go;
    }

    void Scene::RemoveGameObject(GameObject* aGameObject, const float aTime)
    {
        myComponentAdmin->RemoveGameObject(aGameObject, aTime);
    }

    ComponentAdmin& Dynamo::Scene::GetComponentAdmin()
    {
        assert(myComponentAdmin != nullptr && "ComponentAdmin is not initialized in scene.");
        return *myComponentAdmin;
    }

    std::vector<GameObject*> Scene::GetAllGameObjects()
    {
        return myComponentAdmin->GetAllGameObjects();
    }

    void Scene::InternalInit()
    {
        myComponentAdmin = std::make_unique<ComponentAdmin>();
        myComponentAdmin->Init();
    }
}

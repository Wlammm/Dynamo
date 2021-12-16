#include "pch.h"
#include "Scene.h"
#include <ComponentAdmin.h>
#include "Components/Transform.h"
#include "Rendering/FullscreenEffects/BloomEffect.h"

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
        DYN_ASSERT(myComponentAdmin != nullptr && "ComponentAdmin is not initialized in scene.");
        return *myComponentAdmin;
    }

    const std::vector<GameObject*>& Scene::GetAllGameObjects()
    {
        return myComponentAdmin->GetAllGameObjects();
    }

    void Scene::SetEmissiveIntensity(const float aValue)
    {
        myEmissiveIntensity = aValue;
    }

    float Scene::GetEmissiveIntensity() const
    {
        return myEmissiveIntensity;
    }

    const std::string& Scene::GetSavePath() const
    {
        return mySavePath;
    }

    void Scene::SetSavePath(const std::string& aPath)
    {
        mySavePath = aPath;
    }

    void Scene::InternalInit()
    {
        myComponentAdmin = std::make_unique<ComponentAdmin>();
        myComponentAdmin->Init();
    }
}

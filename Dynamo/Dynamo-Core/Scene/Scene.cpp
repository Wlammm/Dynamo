#include "pch.h"
#include "Scene.h"
#include <ComponentAdmin.h>

namespace Dynamo
{
    ComponentAdmin& Dynamo::Scene::GetComponentAdmin()
    {
        assert(myComponentAdmin != nullptr && "ComponentAdmin is not initialized in scene.");
        return *myComponentAdmin;
    }

    void Scene::InternalInit()
    {
        myComponentAdmin = std::make_unique<ComponentAdmin>();
        myComponentAdmin->Init();
    }
}

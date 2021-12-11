#include "pch.h"
#include "Skybox.h"
#include "Components/MeshRenderer.h"
#include "Rendering/Material.h"
#include "Components/Camera.h"

namespace Dynamo
{
	Skybox::Skybox()
	{
		GameObject* ob = Main::GetScene()->CreateGameObject();
		mySphere = ob->AddComponent<MeshRenderer>();
		mySphere->SetModel("Assets/Fbx/SkyboxSphere.fbx");
		mySphere->AddMaterial(MaterialFactory::GetMaterial("Assets/MaterialS/Skybox.dynmaterial"));
		myTransform = &ob->GetTransform();
	}

	Skybox::~Skybox()
	{

	}

	void Skybox::Update()
	{
		Camera* cam = Main::GetMainCamera();
		if (!cam)
			return;

		float farPlane = cam->GetFarPlane();
		myTransform->SetScale(farPlane, farPlane, farPlane);
	}
}

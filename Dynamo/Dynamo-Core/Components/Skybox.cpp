#include "pch.h"
#include "Skybox.h"
#include "Components/MeshRenderer.h"
#include "Rendering/Material.h"
#include "Components/Camera.h"

namespace Dynamo
{
	Skybox::Skybox()
	{

	}

	Skybox::~Skybox()
	{

	}

	void Skybox::OnCreate()
	{
		mySphere = AddComponent<MeshRenderer>();
		mySphere->SetModel("Assets/Fbx/Primitives/SkyboxSphere.fbx");
		mySphere->SetMaterialOnAllMeshes(MaterialFactory::GetMaterial("Assets/Materials/Skybox.dynmaterial"));
		myTransform = &GetGameObject()->GetTransform();
	}

	void Skybox::Update()
	{
		Camera* cam = Main::GetMainCamera();
		if (!cam)
			return;

		float farPlane = cam->GetFarPlane();
		myTransform->SetScale(farPlane * 0.495f, farPlane * 0.495f, farPlane * 0.495f);
		myTransform->SetPosition(cam->GetTransform()->GetPosition());
	}

	void Skybox::EditorUpdate()
	{
		Update();
	}
}

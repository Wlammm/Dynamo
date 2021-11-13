#include <iostream>
#include "ComponentAdmin.h"
#include "PrinterComponent.h"
#include "GameObject.h"
#include "TestComponent.h"

int main()
{
	ComponentAdmin* admin = new ComponentAdmin();
	admin->Init();

	GameObject* gameObject = GameObject::Instantiate();
	gameObject->SetName("tst");
	gameObject->SetTag(Tag::Player);


	GameObject::Destroy(gameObject);
	gameObject = GameObject::Instantiate();

	while (true)
	{
		admin->Update();
		admin->LateUpdate();
	}

	return 0;
}
#pragma once

#include "ComponentAdmin.h"
#include "GameObject.h"

class Component
{
public:
	Component() = default;
	virtual ~Component() = default;

	void SetActive(const bool aState);
	const bool IsActive() const;

	GameObject* GetGameObject() const;

	void operator delete(void*) = delete;

protected:
	template<typename T, typename... Args>
	T* AddComponent(Args&&... params)
	{
		return ComponentAdmin::GetInstance()->AddComponent<T>(myGameObject->GetGameObjectID(), params);
	}

	template<typename T>
	T* GetComponent()
	{
		return ComponentAdmin::GetInstance()->GetComponent<T>(myGameObject->GetGameObjectID());
	}

	template<typename T>
	void RemoveComponent()
	{
		ComponentAdmin::GetInstance()->RemoveComponent<T>(myGameObject->GetGameObjectID());
	}

	// Runs once per frame.
	virtual void Update();
	// Runs once per frame after update.
	virtual void LateUpdate();

	// Runs when the component is created.
	virtual void OnCreate();
	// Runs when the component or gameobject is destroyed.
	virtual void OnDestroy();

	// Runs when the component or gameobject is re-enabled.
	virtual void OnEnable();
	// Runs when the componnet or gmaeobject is disabled.
	virtual void OnDisable();

	// Runs once a collision with this object has happened.
	virtual void OnCollision(GameObject* anOther);
	// Runs the frame this object enters a trigger.
	virtual void OnTriggerEnter(GameObject* anOther);
	// Runs the frame this object leaves a trigger.
	virtual void OnTriggerExit(GameObject* anOther);
	// Runs every frame this object is inside a trigger.
	virtual void OnTrigger(GameObject* anOther);

private:
	template<typename T>
	friend class ComponentArray;
	friend class ComponentAdmin;
	friend class GameObject;

	GameObject* myGameObject = nullptr;
	bool myIsActive = true;
};
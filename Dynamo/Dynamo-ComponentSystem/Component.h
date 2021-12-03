#pragma once

#include "ComponentAdmin.h"
#include "GameObject.h"

class Component
{
public:
	Component() = default;
	Component(const std::string& aCategory);
	virtual ~Component() = default;

	void SetActive(const bool aState);
	const bool IsActive() const;

	GameObject* GetGameObject() const;

	Dynamo::Transform& GetTransform();

	virtual void ExposeValues();

	virtual ComponentName GetName();
	virtual TypeID GetTypeID();
	const std::string& GetCategory() const;

protected:
	template<typename T, typename... Args>
	T* AddComponent(Args&&... params)
	{
		return myAdmin->AddComponent<T>(myGameObject->GetGameObjectID(), params);
	}

	template<typename T>
	T* GetComponent()
	{
		return myAdmin->GetComponent<T>(myGameObject->GetGameObjectID());
	}

	template<typename T>
	void RemoveComponent()
	{
		myAdmin->RemoveComponent<T>(myGameObject->GetGameObjectID());
	}

	// Runs once per frame.
	virtual void Update();
	// Runs once per frame after update.
	virtual void LateUpdate();
	// Runs once per frame in editor.
	virtual void EditorUpdate();

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

	virtual void RegisterToAdmin(class ComponentAdmin& anAdmin);

private:
	template<typename T>
	friend class ComponentArray;
	friend class ComponentAdmin;
	friend class GameObject;
	friend class ComponentRegistry;

	std::string myCategory = "Uncategorized";

	GameObject* myGameObject = nullptr;
	bool myIsActive = true;

	ComponentAdmin* myAdmin = nullptr;
};
#pragma once

class ComponentAdmin;
class GameObject;

namespace Dynamo
{
	/*
	* Dont forget to call Internal init!
	*/
	class Scene
	{
	public:
		// Call Update, LateUpdate however you want.
		virtual void Update() = 0;

		GameObject* CreateGameObject();
		void RemoveGameObject(GameObject* aGameObject, const float aTime = 0);

		ComponentAdmin& GetComponentAdmin();

		const std::vector<GameObject*>& GetAllGameObjects();

		template<typename T>
		std::vector<GameObject*> GetAllGameObjectsWithComponent()
		{
			return myComponentAdmin->GetGameObjectsWithComponent<T>();
		}
		
	protected:
		void InternalInit();

	private:
		CU::DArray<GameObject*> myGameObjects;
		std::unique_ptr<ComponentAdmin> myComponentAdmin;
	};
}
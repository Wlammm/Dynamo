#pragma once

class ComponentAdmin;
class GameObject;

namespace Dynamo
{
	class SceneSerializer;

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
		
		void SetEmissiveIntensity(const float aValue);
		float GetEmissiveIntensity() const;

		const std::string& GetSavePath() const;
		void SetSavePath(const std::string& aPath);

	protected:
		void InternalInit();

	private:
		friend SceneSerializer;
		CU::DArray<GameObject*> myGameObjects;
		std::unique_ptr<ComponentAdmin> myComponentAdmin;

		float myEmissiveIntensity = 1;
		std::string mySavePath = "";
	};
}
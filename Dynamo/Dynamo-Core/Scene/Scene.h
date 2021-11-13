#pragma once

class ComponentAdmin;
class GameObject;

namespace Dynamo
{
	class Scene
	{
	public:
		// Call Update, LateUpdate however you want.
		virtual void Update() = 0;

		ComponentAdmin& GetComponentAdmin();
		
	protected:
		void InternalInit();

	private:
		std::unique_ptr<ComponentAdmin> myComponentAdmin;
	};
}
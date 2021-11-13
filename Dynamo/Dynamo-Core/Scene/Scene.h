#pragma once

class ComponentAdmin;

namespace Dynamo
{

	class Scene
	{
	public:
		virtual void Update() = 0;

		ComponentAdmin& GetComponentAdmin();
		
	protected:
		void InternalInit();

	private:
		std::unique_ptr<ComponentAdmin> myComponentAdmin;
	};
}
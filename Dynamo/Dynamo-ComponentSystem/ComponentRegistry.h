#pragma once

#include <vector>
#include <unordered_map>

class Component;
class ComponentAdmin;

class ComponentRegistry
{
public:
	template<typename T>
	static void RegisterComponent()
	{
		for (int i = 0; i < myComponentTypes.size(); ++i)
		{
			if (dynamic_cast<T*>(myComponentTypes[i]))
				return;
		}
		T* ptr = new T;
		myComponentTypes.push_back(ptr);
	}

	static void RegisterToAdmin(ComponentAdmin& anAdmin);

	static const std::vector<Component*>& GetComponentTypes();

private:
	inline static std::vector<Component*> myComponentTypes;
};
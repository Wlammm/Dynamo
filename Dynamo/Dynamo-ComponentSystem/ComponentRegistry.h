#pragma once

#include <vector>
#include <unordered_map>

using TypeID = std::string;

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
		myComponents[typeid(T).name()] = ptr;
	}

	static void RegisterToAdmin(ComponentAdmin& anAdmin);

	static const std::vector<Component*>& GetComponentTypes();

	static Component* GetComponentFromTypeID(const TypeID& aTypeID);

private:
	inline static std::vector<Component*> myComponentTypes;
	inline static std::unordered_map<TypeID, Component*> myComponents;
};
#include "ComponentRegistry.h"
#include "Component.h"

void ComponentRegistry::RegisterToAdmin(ComponentAdmin& anAdmin)
{
	for (int i = 0; i < myComponentTypes.size(); ++i)
	{
		myComponentTypes[i]->RegisterToAdmin(anAdmin);
	}
}

const std::vector<Component*>& ComponentRegistry::GetComponentTypes()
{
	return myComponentTypes;
}

#include <iostream>
#include "PrinterComponent.h"

void PrinterComponent::Update()
{
	std::cout << "Update";
	RemoveComponent<PrinterComponent>();
}

void PrinterComponent::OnEnable()
{
	std::cout << "enabled";
}

void PrinterComponent::OnDisable()
{
	std::cout << "disabled.";
}
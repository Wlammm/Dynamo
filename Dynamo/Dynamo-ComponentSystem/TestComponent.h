#pragma once

#include "Component.h"
#include <string>

class TestComponent : public Component
{
public:
	TestComponent() = default;
	TestComponent(std::string aWrite);

private:

};
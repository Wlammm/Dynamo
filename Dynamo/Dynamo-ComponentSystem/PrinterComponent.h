#pragma once

#include "Component.h"

class PrinterComponent : public Component
{
public:
	void Update() override;

	void OnEnable() override;
	void OnDisable() override;

private:
};
#pragma once

#include <memory>
#include "ComponentAdmin.h"

#define TO_STRING(x) #x

#define COMP(className)																			  \
class Creator											   							 			  \
{														   							 			  \
public:													   							 			  \
	Creator()											   							 			  \
	{													   							 			  \
		ComponentRegistry::RegisterComponent<className>(); 							 			  \
	}													   							 			  \
private:												   							 			  \
	inline static std::unique_ptr<Creator> myCreator = std::make_unique<Creator>();	 			  \
};																					 			  \
																								  \
protected:																						  \
	void RegisterToAdmin(ComponentAdmin& anAdmin) override										  \
	{																							  \
		anAdmin.GetComponentManager().RegisterComponent<className>(TO_STRING(className));		  \
	}																							  \
																								  \
public:																							  \
	std::string GetName() override																  \
	{																							  \
		return TO_STRING(className);															  \
	}																							  \
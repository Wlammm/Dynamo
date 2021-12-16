#include "Assert.h"
#include "ConsoleUtils.h"
#include "../Dynamo-ImGui/ImGui_Wrapper.h"

void AssertRelease(const char* aFile, const int aLine)
{
	ImGui::NotifyError("Assert", "Assertion in file: %s on line: %i", __FILE__, (int)__LINE__);
	Dynamo::Console::ErrorLog("Assertion in file: %s on line: %i", __FILE__, (int)__LINE__);
}
#pragma once

#include <string>

void AssertRelease(const char* aFile, const int aLine);

#if _DEBUG
#define DYN_ASSERT(expression) assert(expression)

#elif _RELEASE
#define DYN_ASSERT(expression) if(!(expression)) { ImGui::NotifyError("Assert", "Assertion in file: %s on line: %i", __FILE__, (int)__LINE__); Dynamo::Console::ErrorLog("Assertion in file: %s on line: %i", __FILE__, (int)__LINE__); }

#else
#define DYN_ASSERT(expression) ((void)0)

#endif
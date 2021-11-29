#include "EditorPch.h"
#include <iostream>
#include "Entry.h"

int main()
{
	Dynamo::Entry::Init(Editor::MainSingleton::Update);
	Editor::MainSingleton::Init();
	Dynamo::Entry::Run();

	return 0;
}
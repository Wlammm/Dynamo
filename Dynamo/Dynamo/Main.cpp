#include <iostream>
#include <Entry.h>
#include <Game.h>

int main()
{
	Dynamo::Entry::Init(nullptr);
	Game* game = new Game();

	Dynamo::Entry::Run();

	delete game;
	return 0;
}
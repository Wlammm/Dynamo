#pragma once

class GameScene;

class Game
{
public:
	Game();
	~Game();

private:
	std::unique_ptr<GameScene> myScene;

};
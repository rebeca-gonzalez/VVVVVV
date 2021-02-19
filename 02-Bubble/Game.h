#ifndef _GAME_INCLUDE
#define _GAME_INCLUDE

#include "Scene.h"


#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480


// Game is a singleton (a class with a single instance) that represents our whole application


class Game
{

public:
	Game() {}
	
	
	static Game &instance()
	{
		static Game G;
	
		return G;
	}

	void init();
	bool update(int deltaTime);
	void render();
	
	// Input callback methods
	void keyPressed(int key);
	void keyReleased(int key);
	void specialKeyPressed(int key);
	void specialKeyReleased(int key);
	void mouseMove(int x, int y);
	void mousePress(int button);
	void mouseRelease(int button);
	void win();
	
	bool getKey(int key) const;
	bool getSpecialKey(int key) const;

	bool get_up_key();
	void block_up_key();

private:
	bool bPlay;                       // Continue to play game?
	Scene scene;                      // Scene to render
	bool keys[256], specialKeys[256]; // Store key states so that 
	                                  // we can have access at any time
	bool up_key;

	ISoundEngine* MusicEngine;
	SceneState::State state;
};


#endif // _GAME_INCLUDE



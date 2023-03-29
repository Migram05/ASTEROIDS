#pragma once

#include <SDL.h>
#include <iostream>
#include <fstream>
#include <string>
#include "../utils/Vector2D.h"
#include "../sdlutils/Texture.h"
#include "../ecs/GameStateMachine.h"
#include "../ecs/PlayState.h"
#include "../ecs/MainMenuState.h"
#include "../ecs/PauseState.h"
#include "../sdlutils/SDLUtils.h"
#include <list>
#include <unordered_set>

using namespace std;
class Game
{
public:
	Game();
	~Game();
	void Run();
	double WIN_WIDTH = 600; //Para cambiar el ancho y alto, hacerlo desde el archivo "GameData.txt" en la carpeta del juego
	double WIN_HEIGHT = 700;

	Texture* getTexture(int n);
	void exitGame();
	void pauseGame();
	void resumeGame(bool l);
	void playerLoses();

private:

	SDL_Window* window;
	SDL_Renderer* renderer;
	unsigned int FRAME_RATE = 3;
	unsigned int frameTime, startTime;
	bool exit = false;
	void gameSettings();
	const static unsigned int NUM_TEXTURES = 8;
	GameStateMachine* gameStateMachine;
	Texture* textures[NUM_TEXTURES]; //Array de texturas del juego
	enum ValoresArray { GrayAsteroid, GoldAsteroid, Fighter1, Fire, Heart, OnePlayer, Multiplayer, Exit};
	
	typedef struct {
		string filename;
		unsigned int hframes, wframes;
	}TextureDescription;
	const TextureDescription TEXT_DESCRIPT[NUM_TEXTURES]{ //Array con la información de los objetos (dirección, filas y cols)
		{"resources/images/asteroid.png",5,6},{"resources/images/asteroid_gold.png",5,6},{"resources/images/fighter.png",1,1},{"resources/images/fire.png",1,1},{"resources/images/heart.png",1,1},{"resources/images/single.png",1,1},{"resources/images/multi.png",1,1},{"resources/images/exit.png",1,1},
	};
	void InitGameObjects();
	void LoadTextures(SDL_Renderer* renderer);
};


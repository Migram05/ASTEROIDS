#include "Game.h"
using uint = unsigned int;

Game::Game() { //Constructora del juego, con la carga de texturas incluida

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); // Check Memory Leaks
	window = nullptr;
	renderer = nullptr;
	gameSettings(); //Carga el archivo de ajustes de juego, si encuentra el archivo
	SDL_Init(SDL_INIT_EVERYTHING);
	window = SDL_CreateWindow("ASTEROIDS", SDL_WINDOWPOS_CENTERED, //Creación de la ventana
		SDL_WINDOWPOS_CENTERED, WIN_WIDTH, WIN_HEIGHT, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED); //Creación del render
	//SDL_SetRenderDrawColor(renderer, 70,130,191, 255);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	exit = false;
	LoadTextures(renderer); //Se canrgan las texturas en el array
	InitGameObjects();//Se crean los objetos del juego (paddle, ball, wall...)
}
void Game::gameSettings() { //Carga los datos del juego desde un archivo, si lo encuentra

	ifstream readFile("gameData.txt");
	if (readFile.is_open()) {
		readFile >> WIN_WIDTH;
		readFile >> WIN_HEIGHT;
		readFile >> FRAME_RATE;
		readFile.close();
	}
	else {
		ofstream saveFile("gameData.txt"); //Si no encuentra el archivo, lo crea
		saveFile << WIN_WIDTH << endl;
		saveFile << WIN_HEIGHT << endl;
		saveFile << FRAME_RATE << endl;
		saveFile.close();
	}
	
}
void Game::Run() { //Bucle principal de juego
	if (window == nullptr || renderer == nullptr) //Comprobación de ventana y render
		throw exception("Error fatal SDL");
	startTime = SDL_GetTicks();
	while (!exit) { //Bucle de juego con condiciones de detención
		frameTime = SDL_GetTicks() - startTime;
		if (frameTime >= FRAME_RATE) { //Se comprueba el tiempo transcurrido entre el último update
			gameStateMachine->currentState()->update();
			startTime = SDL_GetTicks();
		}
		SDL_RenderClear(renderer);
		gameStateMachine->render();
		SDL_RenderPresent(renderer);
		gameStateMachine->clearList(); //Se borran los estados que no se estén usando
	}
}
void Game::InitGameObjects() { //Creación de la máquina de estados
	gameStateMachine = new GameStateMachine();
	gameStateMachine->changeState(new PlayState(this, WIN_WIDTH, WIN_HEIGHT));
}
Game::~Game() { //Destructora de la memoria dinámica creada
	for (Texture* t : textures) delete t; //Se borran las texturas del juego
	delete gameStateMachine; //Se borra la máquina
	SDL_DestroyRenderer(renderer); //Se destruyen el renderer y la ventana
	SDL_DestroyWindow(window);
	SDL_Quit();
}
void Game::LoadTextures(SDL_Renderer* renderer) { //Cada valor del array de texturas recibe su información del array de descripciones
	textures[GrayAsteroid] = new Texture(renderer, TEXT_DESCRIPT[GrayAsteroid].filename, 5, 6);
	textures[GoldAsteroid] = new Texture(renderer, TEXT_DESCRIPT[GoldAsteroid].filename, 5, 6);
	textures[Fighter1] = new Texture(renderer, TEXT_DESCRIPT[Fighter1].filename);
	textures[Fighter2] = new Texture(renderer, TEXT_DESCRIPT[Fighter2].filename);
	textures[Fire] = new Texture(renderer, TEXT_DESCRIPT[Fire].filename);
	textures[Heart] = new Texture(renderer, TEXT_DESCRIPT[Heart].filename);
}
Texture* Game::getTexture(int x) { //Devuelve una textura del vector
	return textures[x];
}
/*void Game::exitGame() { //Activa la salida
	exit = true;
}
void Game::startGameState() { //Pasa al estado de juego
	gameStateMachine->changeState(new PlayState(this, false, WIN_WIDTH, WIN_HEIGHT));
}

void Game::pauseGame() { //Pausa el juego
	gameStateMachine->pushState(new PauseState(this, WIN_WIDTH, WIN_HEIGHT));
}
void Game::resumeGame() { //Reanuda el juego
	gameStateMachine->popState();
}
void Game::mainMenu() { //Vuelve al menu principal
	gameStateMachine->popState();
	gameStateMachine->changeState(new MainMenuState(this, WIN_WIDTH, WIN_HEIGHT));
}
void Game::playerWins() { //Cambia al estado final tras pasar todos los niveles
	gameStateMachine->changeState(new EndState(this, WIN_WIDTH, WIN_HEIGHT, true));
}
void Game::playerLoses() {
	gameStateMachine->changeState(new EndState(this, WIN_WIDTH, WIN_HEIGHT, false));
}*/

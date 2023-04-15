#include "Game.h"
using uint = unsigned int;

Game::Game() { //Constructora del juego, con la carga de texturas incluida

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); // Check Memory Leaks
	window = nullptr;
	renderer = nullptr;
	gameSettings(); //Carga el archivo de ajustes de juego, si encuentra el archivo
	SDLUtils::init("ASTEROIDS", WIN_WIDTH, WIN_HEIGHT, "../TPV2/resources/config/asteroids.resources.json"); //Inicializa SDLUtils
	auto& sdl = *SDLUtils::instance();
	window = sdl.window(); //Ventana y render
	renderer = sdl.renderer();	
	exit = false;
	LoadTextures(renderer); //Se canrgan las texturas en el array
	InitGameObjects();//Se crean los estados del juego
	SDL_ShowCursor(1);//Se muestra el cursor
}
void Game::gameSettings() { //Carga los datos del juego desde un archivo, si lo encuentra

	ifstream readFile("../TPV2/src/game/gameData.txt");
	if (readFile.is_open()) {
		readFile >> WIN_WIDTH;
		readFile >> WIN_HEIGHT;
		readFile >> FRAME_RATE;
		readFile.close();
	}
	else {
		ofstream saveFile("../TPV2/src/game/gameData.txt"); //Si no encuentra el archivo, lo crea
		saveFile << WIN_WIDTH << endl;
		saveFile << WIN_HEIGHT << endl;
		saveFile << FRAME_RATE << endl;
		saveFile.close();
	}
	
}
void Game::Run() { //Bucle principal de juego
	if (window == nullptr || renderer == nullptr) //Comprobación de ventana y render
		throw exception("Error fatal SDL");
	auto& sdl = *SDLUtils::instance();
	startTime = sdl.currRealTime(); //Control del numero de frames/seg
	while (!exit) { //Bucle de juego con condiciones de detención
		frameTime = sdl.currRealTime() - startTime;
		if (frameTime >= FRAME_RATE) { //Se comprueba el tiempo transcurrido entre el último update
			gameStateMachine->update(); //Update controlado
			startTime = SDL_GetTicks();
		}
		sdl.clearRenderer();
		gameStateMachine->render(); //Render
		sdl.presentRenderer();
		gameStateMachine->clearList(); //Se borran los estados que no se estén usando
	}
}
void Game::InitGameObjects() { //Creación de la máquina de estados
	gameStateMachine = new GameStateMachine();
	gameStateMachine->changeState(new MainMenuState(this, WIN_WIDTH, WIN_HEIGHT));
}

void Game::LoadTextures(SDL_Renderer* renderer) { //Cada valor del array de texturas recibe su información del array de descripciones
	textures[GrayAsteroid] = new Texture(renderer, TEXT_DESCRIPT[GrayAsteroid].filename, 5, 6);
	textures[GoldAsteroid] = new Texture(renderer, TEXT_DESCRIPT[GoldAsteroid].filename, 5, 6);
	textures[Fighter1] = new Texture(renderer, TEXT_DESCRIPT[Fighter1].filename);
	textures[Fire] = new Texture(renderer, TEXT_DESCRIPT[Fire].filename);
	textures[Heart] = new Texture(renderer, TEXT_DESCRIPT[Heart].filename);
	textures[OnePlayer] = new Texture(renderer, TEXT_DESCRIPT[OnePlayer].filename);
	textures[Multiplayer] = new Texture(renderer, TEXT_DESCRIPT[Multiplayer].filename);
	textures[Exit] = new Texture(renderer, TEXT_DESCRIPT[Exit].filename);
	textures[Host] = new Texture(renderer, TEXT_DESCRIPT[Host].filename);
	textures[Join] = new Texture(renderer, TEXT_DESCRIPT[Join].filename);
	textures[TextBox] = new Texture(renderer, TEXT_DESCRIPT[TextBox].filename);
}
Texture* Game::getTexture(int x) { //Devuelve una textura del vector
	return textures[x];
}
void Game::exitGame() { //Activa la salida
	exit = true;
}
void Game::exitToMenu()
{
	gameStateMachine->changeState(new MainMenuState(this, WIN_WIDTH, WIN_HEIGHT));
}
void Game::pauseGame() { //Pausa el juego
	gameStateMachine->pushState(new PauseState(this, WIN_WIDTH, WIN_HEIGHT, false));
}
void Game::resumeGame(bool lost) { //Reanuda el juego
	gameStateMachine->popState();
	if (PlayState* p = dynamic_cast<PlayState*>(gameStateMachine->currentState())) {
		if(lost) p->resetGame();
	}
}
void Game::playerLoses() {
	gameStateMachine->pushState(new PauseState(this, WIN_WIDTH, WIN_HEIGHT, true));
}

void Game::playSingleplayer()
{
	gameStateMachine->changeState(new PlayState(this, WIN_WIDTH, WIN_HEIGHT));
}

void Game::playMultiplayer(bool client, string dir)
{
	gameStateMachine->changeState(new MultiplayerState(this, WIN_WIDTH, WIN_HEIGHT, client, dir));
}


Game::~Game() { //Destructora de la memoria dinámica creada
	for (Texture* t : textures) delete t; //Se borran las texturas del juego
	delete gameStateMachine; //Se borra la máquina
	auto& sdl = *SDLUtils::instance(); //Se borra la instancia de SDLUtils
	sdl.close();
}

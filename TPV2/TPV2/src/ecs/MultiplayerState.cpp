#include "MultiplayerState.h"
#include "Manager.h"
#include "../systems/BulletsSystem.h"
#include "../systems/CollisionsSystem.h"
#include "../systems/FighterSystem.h"
#include "../systems/GameCtrlSystem.h"
#include "../systems/RenderSystem.h"


MultiplayerState::MultiplayerState(Game* g, double w, double h, bool c) : GameState(w, h), isClient(c) // Constructora
{
	game = g;
}
const std::string MultiplayerState::s_playID = "MULTIPLAY";//ID del estado

void MultiplayerState::update()
{
	if (!isClient) {
		if (SDLNet_CheckSockets(socketSet, 0) > 0) {
			// Espera una conexión entrante
			client = SDLNet_TCP_Accept(master_socket);
			if (client) {
				cout << "cliente conectado" << endl;
				SDLNet_TCP_AddSocket(socketSet, client);
			}
		}
	}
	else {
		// Mensaje a enviar al servidor
		const char* message = "Hola, servidor!";
		int result = SDLNet_TCP_Send(client, message, strlen(message) + 1);
		if (result < strlen(message) + 1) {
			std::cerr << "Error al enviar el mensaje al servidor: " << SDLNet_GetError() << std::endl;
		}
	}
	if (client != NULL && SDLNet_CheckSockets(socketSet, 0) > 0) {
		// Buffer para almacenar los datos recibidos
		const int BUFFER_SIZE = 1024;
		char buffer[BUFFER_SIZE];
		memset(buffer, 0, BUFFER_SIZE);
		int result = SDLNet_TCP_Recv(client, buffer, BUFFER_SIZE);
		if (result > 0) {
			cout << buffer << endl;
		}
		else cout << "error al recibir mensaje" << endl;
	}

#ifdef COMPS

	manager_->update(); //Llamada al manager
	asteroidsManager_->addAsteroidFrequency(); //Se chequea el tiempo para generar o no un asteroide
	checkCollisions(); //Colisiones
#endif // COMPS
#ifndef COMPS

	gameCtrlSys_->update();
	fighterSys_->update();
	bulletSys_->update();
	collisionSys_->update();

	manager_->flushMessages();
	//El refresh es un método propio de Game State
#endif // !COMPS
}

void MultiplayerState::render()
{
#ifdef COMPS
	manager_->render();
#endif // COMPS
#ifndef COMPS
	renderSys_->update();
#endif // !COMPS
}

bool MultiplayerState::onEnter()
{
	if (SDLNet_Init() < 0) { 
		game->exitGame();
		cout << "Conection error" << endl;
	}
	/*if (SDLNet_Init() < 0) {
		game->exitGame();
		cout << "Conection error" << endl;
	}
	IPaddress ip; //Struct con la ip y el puerto
	Uint16 port = 0; char* host = nullptr;
	if (SDLNet_ResolveHost(&ip, host, port) < 0) { //Le asigna los valores de "host" y "port" a la ip del destinatario
		cout << "Host/Port error" << endl;
	}
	UDPsocket sock = SDLNet_UDP_Open(port);
	if (!sock) {
		cout << "Error de socket" << endl;
	}
	UDPpacket* p = SDLNet_AllocPacket(512); // hasta 512
	if (!p) { cout << "Error de paquete" << endl; }
	p->address = ip;
	Uint8* lol = new Uint8 (10);
	p->data = lol;
	if (SDLNet_UDP_Send(sock, -1, p) <= 0) {
		cout << "Error al enviar el datapack" << endl;
	}*/

	if (!isClient) {
		//Si es el servidor

		// Crea una dirección IP
		if (SDLNet_ResolveHost(&ip, NULL, port) < 0) {
			throw("Error resolviendo host servidor");
		}
		// Crea un socket para escuchar las conexiones entrantes
		master_socket = SDLNet_TCP_Open(&ip);
		if (!master_socket) {
			throw("Error creando el socket maestro");
		}
		
		SDLNet_TCP_AddSocket(socketSet, master_socket);
	}
	else {
		//Cliente
		if (SDLNet_ResolveHost(&ip, "localhost", port) < 0) {
			throw("Error descifrando IP en el cliente");
		}

		// Crea un socket para conectarse al servidor
		client = SDLNet_TCP_Open(&ip);
		if (!client) {
			throw("Error conectandose al servidor");
		}
		SDLNet_TCP_AddSocket(socketSet, client);
	}

	manager_ = new Manager(game);

#ifdef COMPS
	cout << "Using components" << endl;
	manager_->createPlayer();
#endif // COMPS

#ifndef COMPS
	cout << "Using systems" << endl;
	gameCtrlSys_ = manager_->addSystem<GameCtrlSystem>();

	bulletSys_ = manager_->addSystem<BulletsSystem>();

	collisionSys_ = manager_->addSystem<CollisionsSystem>();

	fighterSys_ = manager_->addSystem<FighterSystem>();

	renderSys_ = manager_->addSystem<RenderSystem>();
#endif // !COMPS
	auto& sdl = *SDLUtils::instance();
	Music::setMusicVolume(8); //Musica de fondo y volumen
	sdl.musics().at("theme").play();
	SoundEffect::setChannelVolume(25);
	return true;
}

void MultiplayerState::refresh()
{
	manager_->refresh();
}

void MultiplayerState::resetGame()
{
	//TO DO
}

void MultiplayerState::checkCollisions()
{
	//TO DO
}

MultiplayerState::~MultiplayerState()
{
	Music::haltMusic();
	delete manager_;

	// Cerrar sockets
	if(client) SDLNet_TCP_Close(client);
	if (!isClient) {
		SDLNet_TCP_Close(master_socket);
		SDLNet_FreeSocketSet(socketSet);
	}
	// Cierra SDL_net
	SDLNet_Quit();
}


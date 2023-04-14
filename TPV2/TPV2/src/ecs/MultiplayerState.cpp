#include "MultiplayerState.h"
#include "Manager.h"
#include "../systems/BulletsSystem.h"
#include "../systems/CollisionsSystem.h"
#include "../systems/FighterSystemOnline.h"
#include "../systems/GameCtrlSystem.h"
#include "../systems/RenderSystem.h"


MultiplayerState::MultiplayerState(Game* g, double w, double h, bool c, string ipDirection) : GameState(w, h), isClient(c), ipDir(ipDirection) // Constructora
{
	game = g;
}
const std::string MultiplayerState::s_playID = "MULTIPLAYER";//ID del estado

void MultiplayerState::update()
{
	if (!isClient) { //Comprueba si hay mensajes
		if (SDLNet_CheckSockets(socketSet, 0) > 0) {
			if (SDLNet_SocketReady(master_socket)) {
				// Espera una conexión entrante
				client = SDLNet_TCP_Accept(master_socket);
				if (client) {
					playerIndex++;
					cout << "cliente conectado" << endl;
					SDLNet_TCP_AddSocket(socketSet, client);

					// Mensaje a enviar al servidor
					string str = "IndxSet" + to_string(playerIndex);
					const char* message = str.c_str();
					int result = SDLNet_TCP_Send(client, message, strlen(message) + 1);
					if (result < strlen(message) + 1) {
						std::cerr << "Error al enviar el mensaje al cliente: " << SDLNet_GetError() << std::endl;
					}
				}
			}
			if (client && SDLNet_SocketReady(client)) {
				// Buffer para almacenar los datos recibidos
				const int BUFFER_SIZE = 1024;
				char buffer[BUFFER_SIZE];
				memset(buffer, 0, BUFFER_SIZE);
				int result = SDLNet_TCP_Recv(client, buffer, BUFFER_SIZE);
				if (result > 0) {
					onRecieveMessage(buffer);
				}
				else client = nullptr;
			}
			
		}
	}
	else {
		if (client != NULL && SDLNet_CheckSockets(socketSet, 0) > 0) {
			if (SDLNet_SocketReady(client)) {
				// Buffer para almacenar los datos recibidos
				const int BUFFER_SIZE = 1024;
				char buffer[BUFFER_SIZE];
				memset(buffer, 0, BUFFER_SIZE);
				int result = SDLNet_TCP_Recv(client, buffer, BUFFER_SIZE);
				if (result > 0) {
					onRecieveMessage(buffer);
				}
				else manager_->exitGame();
			}

		}
	}
	if (!client) return; //Si no hay otro jugador, no se comienza la partida

#ifdef COMPS
	manager_->update(); //Llamada al manager
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

void MultiplayerState::onRecieveMessage(char* m)
{
	cout << m << endl;
	cout << sizeof(m) << endl;
	if (strncmp(m, "IndxSet", 7) == 0) {
		// Los contenidos de las cadenas son iguales
		cout << "cambio de index" << endl;
		cout << m[7] - 48 << endl;
		manager_->setPlayerIndex(m[7]-48); //Calculo para obtener el índice a partir de un char
		Message msg; msg.id = _m_CHANGEINDEX;
		manager_->send(msg, true);
	}
	else if (strncmp(m, "Move", 4) == 0) {
		cout << "la nave se mueve" << endl;
		Message msg; msg.id = _m_MOVESHIP; msg.moveShip_data.indx = (m[4] - 48);
		manager_->send(msg, true);
	}
	else if (strncmp(m, "Rotate", 6) == 0) {
		Message msg; msg.id = _m_ROTATESHIP; msg.rotateShip_data.indx = (m[7] - 48);
		if (m[6] == 'I') msg.rotateShip_data.proportion = -1;
		else msg.rotateShip_data.proportion = 1;
		manager_->send(msg, true);
	}
	else if (strncmp(m, "Shoot", 5) == 0) {
		Message msg; msg.id = _m_SHIPSHOOT; msg.shipShoot_data.indx = (m[5] - 48);
		manager_->send(msg, true);
	}
	else {
		// Los contenidos de las cadenas son diferentes
		cout << "mensaje desconocido" << endl;
	}

}
//Envia mensajes al otro jugador
void MultiplayerState::sendMessage(string m)
{
	const char* message = m.c_str();
	int result = SDLNet_TCP_Send(client, message, strlen(message) + 1);
	if (result < strlen(message) + 1) {
		std::cerr << "Error al enviar el mensaje al cliente: " << SDLNet_GetError() << std::endl;
	}
}

void MultiplayerState::render()
{
#ifdef COMPS
	manager_->render();
#endif // COMPS
#ifndef COMPS
	renderSys_->update();
#endif // !COMPS
	if (!client) {
		auto& sdl = *SDLUtils::instance();
		Texture lostText(sdl.renderer(), "ESPERANDO JUGADORES", sdl.fonts().at("CAPTURE50"), build_sdlcolor(0x112233ff));
		lostText.render(sdl.width() / 2 - 270, sdl.height() / 2 - 50);
	}
}

bool MultiplayerState::onEnter()
{

	if (SDLNet_Init() < 0) { 
		game->exitGame();
		cout << "Conection error" << endl;
	}

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
		if (SDLNet_ResolveHost(&ip, ipDir.c_str(), port) < 0) {
			throw("Error descifrando IP en el cliente");
		}

		// Crea un socket para conectarse al servidor
		client = SDLNet_TCP_Open(&ip);
		if (!client) {
			cout << "Error conectandose al servidor" << endl;
			game->exitToMenu(); //Si no hay una partida hosteada, devuelve al cliente al menú
		}
		SDLNet_TCP_AddSocket(socketSet, client);
	}

	manager_ = new Manager(game);
#ifdef COMPS
	cout << "Using components" << endl;
	manager_->createPlayer(2);
#endif // COMPS

#ifndef COMPS
	cout << "Using systems" << endl;
	gameCtrlSys_ = manager_->addSystem<GameCtrlSystem>();

	bulletSys_ = manager_->addSystem<BulletsSystem>();

	fighterSys_ = manager_->addSystem<FighterSystemOnline>();

	collisionSys_ = manager_->addSystem<CollisionsSystem>();

	renderSys_ = manager_->addSystem<RenderSystem>();
#endif // !COMPS
	auto& sdl = *SDLUtils::instance();
	Music::setMusicVolume(8); //Musica de fondo y volumen (8)
	sdl.musics().at("theme").play();
	SoundEffect::setChannelVolume(25); //(25)
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


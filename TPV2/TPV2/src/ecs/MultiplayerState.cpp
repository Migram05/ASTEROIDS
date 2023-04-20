#include "MultiplayerState.h"
#include "Manager.h"
#include "../systems/BulletsSystem.h"
#include "../systems/CollisionsSystem.h"
#include "../systems/FighterSystemOnline.h"
#include "../systems/GameCtrlSystem.h"
#include "../systems/RenderSystem.h"


MultiplayerState::MultiplayerState(Game* g, double w, double h, bool c, string name, string ipDirection) : GameState(w, h), isClient(c), localName(name), ipDir(ipDirection)// Constructora
{
	game = g;
}
const std::string MultiplayerState::s_playID = "MULTIPLAYER";//ID del estado


bool MultiplayerState::onEnter()
{
#ifdef COMPS
	game->exitToMenu("MULTIJUGADOR SOLO CON SISTEMAS"); //El multijugador solo funciona con sistemas
	return false;
#endif // COMPS
#ifndef COMPS
	if (SDLNet_Init() < 0) {
		cout << "Conection error" << endl;
	}
	if (!isClient) {
		//Si es el servidor
		// Crea una dirección IP
		if (SDLNet_ResolveHost(&ip, NULL, port) < 0) {
			game->exitToMenu("Error resolviendo host servidor");
		}
		// Crea un socket para escuchar las conexiones entrantes
		master_socket = SDLNet_TCP_Open(&ip);
		if (!master_socket) {
			game->exitToMenu("Error creando el socket maestro");
		}
		SDLNet_TCP_AddSocket(socketSet, master_socket);
		cout << system("ipconfig") << endl;
	}
	else {
		//Cliente
		if (SDLNet_ResolveHost(&ip, ipDir.c_str(), port) < 0) {
			game->exitToMenu("Connexion error"); //Si no hay una partida hosteada, devuelve al cliente al menú
		}
		// Crea un socket para conectarse al servidor
		client = SDLNet_TCP_Open(&ip);
		if (!client) {
			game->exitToMenu("Connexion error"); //Si no hay una partida hosteada, devuelve al cliente al menú
		}
		SDLNet_TCP_AddSocket(socketSet, client);
		// Mensaje para darle el nombre al servidor
		string str = "Name" + localName;
		const char* message = str.c_str();
		int result = SDLNet_TCP_Send(client, message, strlen(message) + 1);
		if (result < strlen(message) + 1) {
			std::cerr << "Error al enviar el mensaje al servidor: " << SDLNet_GetError() << std::endl;
		}
	}
	manager_ = new Manager(game);
	manager_->setPlayerName(localName);
	gameCtrlSys_ = manager_->addSystem<GameCtrlSystem>();
	bulletSys_ = manager_->addSystem<BulletsSystem>();
	fighterSys_ = manager_->addSystem<FighterSystemOnline>();
	collisionSys_ = manager_->addSystem<CollisionsSystem>();
	renderSys_ = manager_->addSystem<RenderSystem>();

	auto& sdl = *SDLUtils::instance();
	Music::setMusicVolume(8); //Musica de fondo y volumen (8)
	sdl.musics().at("theme").play();
	SoundEffect::setChannelVolume(25); //(25)
	return true;
#endif // !COMPS
}

void MultiplayerState::update()
{
	if (!isClient) { //Comprueba si hay mensajes
		if (SDLNet_CheckSockets(socketSet, 0) > 0) {
			if (SDLNet_SocketReady(master_socket)) {
				// Espera una conexión entrante
				client = SDLNet_TCP_Accept(master_socket);

				if (client) {
					playerIndex = 1;
					cout << "cliente conectado" << endl;
					SDLNet_TCP_AddSocket(socketSet, client);
					// Mensaje a enviar al servidor
					string str = "IndxSet" + to_string(playerIndex) + localName;
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
				else manager_->exitGame("");
			}

		}
	}
	if (!client) {
		checkExit();
		return;
	}
	
	gameCtrlSys_->update();
	fighterSys_->update();
	bulletSys_->update();
	collisionSys_->update();

	manager_->flushMessages();
	//El refresh es un método propio de Game State
}

void MultiplayerState::onRecieveMessage(char* m)
{
	//cout << m << endl;
	//cout << sizeof(m) << endl;
	if (strncmp(m, "IndxSet", 7) == 0) {
		// Los contenidos de las cadenas son iguales
		cout << "cambio de index" << endl;
		cout << m[7] - 48 << endl;
		manager_->setPlayerIndex(m[7]-48); //Calculo para obtener el índice a partir de un char
		Message msg; msg.id = _m_CHANGEINDEX;
		manager_->send(msg, true);

		//Con el cambio de indice tambien recibe el nombre del host
		string otherName = m;
		otherName = otherName.substr(8, otherName.size() - 8);
		cout << "Nombre del otro jugador: " << otherName << endl;
		manager_->setEnemyName(otherName);
	}
	else if (strncmp(m, "Move", 4) == 0) {
		cout << "la nave se mueve "<< (m[4] - 48) << endl;
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
	if (strncmp(m, "Reset", 5) == 0) {
		client = NULL;
		Message msg; msg.id = _m_RESETPLAYERS;
		cout << "reset " << endl;
		manager_->send(msg);
	}
	else if (strncmp(m, "Name", 4) == 0) {
		string otherName = m;
		otherName = otherName.substr(4, otherName.size() - 4);
		cout << "Nombre del otro jugador: " <<  otherName << endl;
		manager_->setEnemyName(otherName);
	}
	else if (strncmp(m, "M", 1) == 0) {
		int index = (m[1] - 48);
		string msgS = m;
		string sPX = msgS.substr(2, 4); int posX = stoi(sPX);
		string sPY = msgS.substr(6, 4); int posY = stoi(sPY);
		string sR = msgS.substr(10, 4); 
		int Rot;
		if (sR[1] == '-' || sR[2] == '-') {
			if (sR[1] == '-') sR = sR.substr(2, 2);
			else sR = sR.substr(3, 1);
			Rot = -stoi(sR);
		}
		else Rot = stoi(sR);
		string sS = msgS.substr(14, 1); int Shoot = stoi(sS);
		
		Message msg; msg.id = _m_SHIPSTATE; 
		msg.shipData.idx = index; msg.shipData.pX = posX; msg.shipData.pY = posY; msg.shipData.R = Rot; msg.shipData.S = Shoot;
		manager_->send(msg);
	}
	else {
		// Los contenidos de las cadenas son diferentes
		cout << "mensaje desconocido" << endl;
	}

}
void MultiplayerState::checkExit() //Comprueba que no se quiera salir de la partida
{
	SDL_Event event;
	while (SDL_PollEvent(&event)) { //Controla la entrada
		if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) { //Devuelve al host a menú principal
			manager_->exitGame("");
		}
	}
}
//Envia mensajes al otro jugador
void MultiplayerState::sendMessage(string m)
{
	const char* message = m.c_str();
	int result = SDLNet_TCP_Send(client, message, strlen(message) + 1);
	if (result < strlen(message) + 1) {
		std::cerr << "Error al enviar el mensaje al cliente: " << SDLNet_GetError() << std::endl;
		SDLNet_TCP_DelSocket(socketSet, client);
		client = NULL;
		Message msg; msg.id = _m_RESETPLAYERS;
		cout << "reset " << endl;
		manager_->send(msg);
	}
}

void MultiplayerState::render()
{
	renderSys_->update();

	if (!client) {
		auto& sdl = *SDLUtils::instance();
		Texture lostText(sdl.renderer(), "WAITING FOR PLAYERS", sdl.fonts().at("CAPTURE50"), build_sdlcolor(0x112233ff));
		lostText.render(sdl.width() / 2 - 270, sdl.height() / 2 - 50);
	}
}

void MultiplayerState::refresh()
{
	manager_->refresh();
}

MultiplayerState::~MultiplayerState()
{
	Music::haltMusic();
	if(manager_) delete manager_;
	//Cierre de sockets
	SDLNet_TCP_Close(client);
	SDLNet_FreeSocketSet(socketSet);
	if (!isClient && master_socket) SDLNet_TCP_Close(master_socket);
	// Cierra SDL_net
	SDLNet_Quit();
}


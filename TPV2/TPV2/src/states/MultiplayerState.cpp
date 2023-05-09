#include "MultiplayerState.h"
#include "../ecs/Manager.h"
#include "../systems/BulletsSystem.h"
#include "../systems/CollisionsSystem.h"
#include "../systems/FighterSystemOnline.h"
#include "../systems/GameCtrlSystem.h"
#include "../systems/RenderSystem.h"
#include "../ecs/Component.h"

MultiplayerState::MultiplayerState(Game* g, double w, double h, bool c, string name, string ipDirection) : GameState(w, h), isClient(c), localName(name), ipDir(ipDirection)// Constructora
{
	game = g;
}
const std::string MultiplayerState::s_playID = "MULTIPLAYER";//ID del estado


bool MultiplayerState::onEnter()
{
#ifdef COMPS //Desactiva los componentes
#undef COMPS
#endif // COMPS

#ifndef COMPS
	manager_ = new Manager(game); //Contruye los objetos
	manager_->setPlayerName(localName);
	gameCtrlSys_ = manager_->addSystem<GameCtrlSystem>();
	bulletSys_ = manager_->addSystem<BulletsSystem>();
	fighterSys_ = manager_->addSystem<FighterSystemOnline>();
	collisionSys_ = manager_->addSystem<CollisionsSystem>();
	renderSys_ = manager_->addSystem<RenderSystem>();

	Message msg; msg.id = _m_EXIT;  //Salida

	if (SDLNet_Init() < 0) { //Inicia SDL_Net
		msg.mainMenuInfo.menuInfoData = SDLNet_GetError();
		manager_->send(msg);
	}
	if (!isClient) { //Si es el servidor
		if (SDLNet_ResolveHost(&ip, NULL, port) < 0) {// Crea una dirección IP
			msg.mainMenuInfo.menuInfoData = SDLNet_GetError();
			manager_->send(msg);
		}
		// Crea un socket para escuchar las conexiones entrantes
		master_socket = SDLNet_TCP_Open(&ip);
		if (!master_socket) {
			msg.mainMenuInfo.menuInfoData = SDLNet_GetError();
			manager_->send(msg);
		}
		SDLNet_TCP_AddSocket(socketSet, master_socket);
		cout << system("ipconfig") << endl; //Muestra la IP por consola
	}
	else {
		//Cliente
		if (SDLNet_ResolveHost(&ip, ipDir.c_str(), port) < 0) {
			msg.mainMenuInfo.menuInfoData = SDLNet_GetError();
			manager_->send(msg);
			return false;
		}
		// Crea un socket para conectarse al servidor
		client = SDLNet_TCP_Open(&ip);
		if (!client) {
			msg.mainMenuInfo.menuInfoData = SDLNet_GetError();
			manager_->send(msg);
			return false;
		}
		SDLNet_TCP_AddSocket(socketSet, client);
		// Mensaje para darle el nombre al servidor
		string str = "Name" + localName;
		const char* message = str.c_str();
		int result = SDLNet_TCP_Send(client, message, strlen(message) + 1);
		if (result < strlen(message) + 1) {
			msg.mainMenuInfo.menuInfoData = SDLNet_GetError();
			manager_->send(msg);
		}
	}
	
	auto& sdl = *SDLUtils::instance();
	Music::setMusicVolume(8); //Musica de fondo y volumen (8)
	sdl.musics().at("theme").play();
	SoundEffect::setChannelVolume(25); //(25)
	SDL_ShowCursor(0);//Se oculta el cursor
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

				if (client) { //Si conecta con el cliente
					
					cout << "cliente conectado" << endl;
					SDLNet_TCP_AddSocket(socketSet, client);
					// Mensaje a enviar al servidor
					string str = "IndxSet1" + localName; //Le indica al cliente su índice y le pasa el nombre del host
					const char* message = str.c_str();
					int result = SDLNet_TCP_Send(client, message, strlen(message) + 1);
					if (result < strlen(message) + 1) { 
						std::cerr << "Error al enviar el mensaje al cliente: " << SDLNet_GetError() << std::endl;
					}
				}
			}
			if (client && SDLNet_SocketReady(client)) { //Recibe los mensajes del cliente
				// Buffer para almacenar los datos recibidos
				const int BUFFER_SIZE = 1024;
				char buffer[BUFFER_SIZE];
				memset(buffer, 0, BUFFER_SIZE);
				int result = SDLNet_TCP_Recv(client, buffer, BUFFER_SIZE);
				if (result > 0) {
					onRecieveMessage(buffer); //Si llega el mensaje, se procesa
				}
				
			}
		}
	}
	else {
		if (client != NULL && SDLNet_CheckSockets(socketSet, 0) > 0) { //El cliente comprueba mensajes
			if (SDLNet_SocketReady(client)) {
				// Buffer para almacenar los datos recibidos
				const int BUFFER_SIZE = 1024;
				char buffer[BUFFER_SIZE];
				memset(buffer, 0, BUFFER_SIZE);
				int result = SDLNet_TCP_Recv(client, buffer, BUFFER_SIZE);
				if (result > 0) {
					onRecieveMessage(buffer);
				}
				else { Message msg; msg.id = _m_EXIT; msg.mainMenuInfo.menuInfoData = "HOST DISCONNECTED"; manager_->send(msg); }  //Salida
			}

		}
		else if(!client) { Message msg; msg.id = _m_EXIT; msg.mainMenuInfo.menuInfoData = "HOST DISCONNECTED"; manager_->send(msg); }  //Salida
	}
	if (!client) { //Comprobación de salida del host, mientras espera al cliente
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
	//Procesado de la información de mensajes
	if (strncmp(m, "IndxSet", 7) == 0) {
		// Los contenidos de las cadenas son iguales
		cout << m[7] - 48 << endl;
		manager_->setPlayerIndex(m[7]-48); //Calculo para obtener el índice a partir de un char
		Message msg; msg.id = _m_CHANGEINDEX; 
		manager_->send(msg, true);

		//Con el cambio de indice tambien recibe el nombre del host
		string otherName = m;
		otherName = otherName.substr(8, otherName.size() - 8);
		manager_->setEnemyName(otherName);
	}
	else if (strncmp(m, "Name", 4) == 0) {
		//Mensaje para que el host ajuste el nombre del cliente
		string otherName = m;
		otherName = otherName.substr(4, otherName.size() - 4);
		manager_->setEnemyName(otherName);
	}
	else if (strncmp(m, "M", 1) == 0) {
		int index = (m[1] - 48); //Se procesa toda la inforación de un jugador
		string msgS = m;
		string sPX = msgS.substr(2, 4); int posX = stoi(sPX); //Posición X
		string sPY = msgS.substr(6, 4); int posY = stoi(sPY); //Posición Y
		string sR = msgS.substr(10, 4); 
		int Rot; //Rotación
		if (sR[1] == '-' || sR[2] == '-') { //Ajuste de lectura según la posición del signo negativo
			if (sR[1] == '-') sR = sR.substr(2, 2);
			else sR = sR.substr(3, 1);
			Rot = -stoi(sR);
		}
		else Rot = stoi(sR);
		string sS = msgS.substr(14, 1); int Shoot = stoi(sS); //Comprueba si se ha disparado
		Message msg; msg.id = _m_SHIPSTATE; 
		msg.shipData.idx = index; msg.shipData.pX = posX; msg.shipData.pY = posY; msg.shipData.R = Rot; msg.shipData.S = Shoot;
		manager_->send(msg); //Se le manda al sistema de la nave toda la información
	}
	else cout << "unknown message" << endl;
}
void MultiplayerState::checkExit() //Comprueba que no se quiera salir de la partida
{
	SDL_Event event;
	while (SDL_PollEvent(&event)) { //Controla la entrada
		if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) { //Devuelve al host a menú principal
			Message msg; msg.id = _m_EXIT; msg.mainMenuInfo.menuInfoData = ""; manager_->send(msg);  //Salida
		}
	}
}
//Envia mensajes al otro jugador
void MultiplayerState::sendMessage(string m)
{
	const char* message = m.c_str(); //Crea el mensaje
	int result = SDLNet_TCP_Send(client, message, strlen(message) + 1); //Lo envía
	if (result < strlen(message) + 1) { //En caso de no poder enviarlo, es que el cliente no está
		SDLNet_TCP_DelSocket(socketSet, client); //Se borra el socket del socketSet
		client = NULL; //Cliente a nulo para que vuelva al estado de espera
		Message msg; msg.id = _m_RESETPLAYERS; msg.resetShipsData.resetLives = true; //Recoloca a los jugadores en la posición inicial
		manager_->send(msg);
	}
}

void MultiplayerState::render()
{
	renderSys_->update(); //Renderizado de entidades
	if (!client) { //En caso de no haber cliente, muestra el texto de espera
		auto& sdl = *SDLUtils::instance();
		Texture lostText(sdl.renderer(), "WAITING FOR PLAYERS", sdl.fonts().at("CAPTURE50"), build_sdlcolor(0x05FAFFff));
		lostText.render(sdl.width() / 2 - 270, sdl.height() / 2 - 50);
	}
}

void MultiplayerState::refresh()
{
	manager_->refresh();
}

MultiplayerState::~MultiplayerState()
{
	if(manager_) delete manager_;
	//Cierre de sockets
	SDLNet_TCP_Close(client);
	SDLNet_FreeSocketSet(socketSet);
	if (!isClient && master_socket) SDLNet_TCP_Close(master_socket);
	// Cierra SDL_net
	SDLNet_Quit();
}


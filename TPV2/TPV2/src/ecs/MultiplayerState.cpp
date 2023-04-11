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
		
		
	}
	else {
		//Cliente
		
	}
	///* Main loop */
	//quit = 0;
	//while (!quit) { /* Wait a packet. UDP_Recv returns != 0 if a packet is coming */
	//	if (SDLNet_UDP_Recv(sd, p)) {
	//		printf("UDP Packet incoming\n");
	//		printf("\tChan: %d\n", p->channel);
	//		printf("\tData: %s\n", (char*)p->data);
	//		printf("\tLen: %d\n", p->len);
	//		printf("\tMaxlen: %d\n", p->maxlen);
	//		printf("\tStatus: %d\n", p->status);
	//		printf("\tAddress: %x %x\n", p->address.host, p->address.port);

	//		/* Quit if packet contains "quit" */
	//		if (!strcmp((char*)p->data, "quit")) quit = 1;
	//	}
	//}

	////CLIENTE

	//UDPsocket sd; IPaddress srvadd; UDPpacket* p; int quit;

	/////* Check for parameters */ if (argc < 3) { fprintf(stderr, "Usage: %s host port\n", argv[0]); exit(EXIT_FAILURE); }

	///* Initialize SDL_net */
	//if (SDLNet_Init() < 0) { fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError()); }

	///* Open a socket on random port */
	//if (!(sd = SDLNet_UDP_Open(0))) { fprintf(stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError()); }

	///* Resolve server name */
	////if (SDLNet_ResolveHost(&srvadd, argv[1], atoi(argv[2]))) { fprintf(stderr, "SDLNet_ResolveHost(%s %d): %s\n", argv[1], atoi(argv[2]), SDLNet_GetError()); }

	///* Allocate memory for the packet */
	//if (!(p = SDLNet_AllocPacket(512))) { fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError()); }

	///* Main loop */
	//quit = 0;
	//while (!quit) {
	//	printf("Fill the buffer\n>");
	//	scanf("%s", (char*)p->data);

	//	p->address.host = srvadd.host; /* Set the destination host */
	//	p->address.port = srvadd.port; /* And destination port */

	//	p->len = strlen((char*)p->data) + 1;
	//	SDLNet_UDP_Send(sd, -1, p); /* This sets the p->channel */

	//	/* Quit if packet contains "quit" */
	//	if (!strcmp((char*)p->data, "quit")) quit = 1;
	//}
	////SDLNet_FreePacket(p);
	

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

	SDLNet_Quit();
}


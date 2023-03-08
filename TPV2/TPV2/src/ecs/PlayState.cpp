#include "PlayState.h"
#include "Manager.h"
#include "../systems/AsteroidsSystem.h"
#include "../systems/BulletsSystem.h"
#include "../systems/CollisionsSystem.h"
#include "../systems/FighterSystem.h"
#include "../systems/GameCtrlSystem.h"
#include "../systems/RenderSystem.h"
PlayState::PlayState(Game* g, double w, double h) : GameState(w,h){ // Constructora
	game = g;
}
const std::string PlayState::s_playID = "PLAY";//ID del estado

void PlayState::update() //Se actualizan los objetos de la lista
{
	/*manager_->update(); //Llamada al manager
	asteroidsManager_->addAsteroidFrequency(); //Se chequea el tiempo para generar o no un asteroide
	checkCollisions(); //Colisiones*/
	gameCtrlSys_->update();
	fighterSys_->update();
	bulletSys_->update();
	asteroidSys_->update();
	collisionSys_->update();
	manager_->flushMessages();
}
void PlayState::render() //Renderizado del juego
{
	//manager_->render();
	renderSys_->update();
}
bool PlayState::onEnter() //Se inicializan los objetos
{
	manager_ = new Manager(game);
	//manager_->createPlayer();
	
	gameCtrlSys_ = manager_->addSystem<GameCtrlSystem>();

	asteroidSys_ = manager_->addSystem<AsteroidsSystem>();

	bulletSys_ = manager_->addSystem<BulletsSystem>();

	collisionSys_ = manager_->addSystem<CollisionsSystem>();

	fighterSys_ = manager_->addSystem<FighterSystem>();

	renderSys_ = manager_->addSystem<RenderSystem>();

	//Quitar
	//asteroidsManager_ = new AsteroidsManager(manager_, manager_->getPlayer(), 10);

	auto& sdl = *SDLUtils::instance();
	Music::setMusicVolume(8); //Musica de fondo y volumen
	sdl.musics().at("theme").play();
	SoundEffect::setChannelVolume(25);
	return true;
}

void PlayState::refresh()//Se refresca el manager para borrar entidades muertas
{
	manager_->refresh(); 
}

void PlayState::checkCollisions() { //Chequeo de colisiones
	/*vector<Entity*> asteroids = manager_->getEntitiesByGroup(ecs::_grp_ASTEROIDS); //Se guardan las distintas entidades
	vector<Entity*> bullets = manager_->getEntitiesByGroup(ecs::_grp_BULLETS);
	vector<Entity*> player = manager_->getEntitiesByGroup(ecs::_grp_PLAYER);
	bool reset = false;
	auto& sdl = *SDLUtils::instance();
	auto it = asteroids.begin();
	while (it!= asteroids.end() && !reset) { //Se recorren los asteroides
		Entity* a = *it;
		Transform* t1 = manager_->getComponent<Transform>(a);
		for (Entity* b : bullets) { //Primero comprueba sus colisiones con las balas
			Transform* t2 = manager_->getComponent<Transform>(b);
			if (manager_->isAlive(b) && Collisions::collidesWithRotation(t1->getPos(), t1->getW(), t1->getH(), t1->getRotation(), t2->getPos(), t2->getW(), t2->getH(), t2->getRotation())) {
				sdl.soundEffects().at("bang").play(); //Efecto de sonido
				asteroidsManager_->onCollision(a); //Llama al AM para destruir el asteroide
				manager_->setAlive(b,false);
			}
		}
		for (Entity* p : player) { //Comprueba la colisión con el jugador
			Transform* t3 = manager_->getComponent<Transform>(p);//Si colisiona
			if (Collisions::collidesWithRotation(t1->getPos(), t1->getW(), t1->getH(), t1->getRotation(), t3->getPos(), t3->getW(), t3->getH(), t3->getRotation())) {
				sdl.soundEffects().at("explosion").play();
				manager_->getComponent<Health>(p)->damage();//Se le hace daño
				t3->getPos() = Vector2D{ (float)WIN_WIDTH / 2, (float)WIN_HEIGHT / 2 }; t3->getRotation() = 0; t3->getVel() = Vector2D{ 0,0 };
				asteroidsManager_->destroyAllAsteroids(); // Se borran los asteroides
				reset = true; //Variable que termina el bucle y resetea el juego
			}
		}
		++it;
	}
	if (reset) resetGame(); //Reset del juego*/
}

void PlayState::resetGame() //Reset del juego
{
	refresh();
	if (manager_->isPlayerAlive()) game->pauseGame(); //En caso de tener vidas, se pausa el juego
	else { //Sino, se lanza la pantalla de derrota
		game->playerLoses();
		Entity* p = manager_->getPlayer();
		manager_->getComponent<Health>(p)->resetLives();
	}
	asteroidsManager_->createAsteroids(10); //Se crean asteroides
}

PlayState::~PlayState() { //Destructora
	Music::haltMusic();
	delete manager_;
	delete asteroidsManager_;
}

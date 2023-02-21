#include "PlayState.h"

PlayState::PlayState(Game* g, double w, double h) : GameState(w,h){ // Constructora
	game = g;
}
const std::string PlayState::s_playID = "PLAY";//ID del estado

void PlayState::update() //Se actualizan los objetos de la lista
{
	manager_->update();
	asteroidsManager_->addAsteroidFrequency();
	checkCollisions();
}
void PlayState::render() //Renderizado del juego
{
	manager_->render();
}
bool PlayState::onEnter() //Se inicializan los objetos
{
	manager_ = new Manager(game);
	manager_->createPlayer();
	asteroidsManager_ = new AsteroidsManager(manager_, manager_->getPlayer(), 10);
	auto& sdl = *SDLUtils::instance();
	Music::setMusicVolume(8);
	sdl.musics().at("theme").play();
	return true;
}

void PlayState::refresh()
{
	manager_->refresh();
}

void PlayState::checkCollisions() {
	vector<Entity*> asteroids = manager_->getEntitiesByGroup(ecs::_grp_ASTEROIDS);
	vector<Entity*> bullets = manager_->getEntitiesByGroup(ecs::_grp_BULLETS);
	vector<Entity*> player = manager_->getEntitiesByGroup(ecs::_grp_PLAYER);
	bool reset = false;
	auto& sdl = *SDLUtils::instance();
	auto it = asteroids.begin();
	while (it!= asteroids.end() && !reset) {
		Entity* a = *it;
		Transform* t1 = a->getComponent<Transform>();
		for (Entity* b : bullets) {
			Transform* t2 = b->getComponent<Transform>();
			if (b->isAlive() && Collisions::collidesWithRotation(t1->getPos(), t1->getW(), t1->getH(), t1->getRotation(), t2->getPos(), t2->getW(), t2->getH(), t2->getRotation())) {
				sdl.soundEffects().at("bang").play();
				asteroidsManager_->onCollision(a);
				b->setAlive(false);
			}
		}
		for (Entity* p : player) {
			Transform* t3 = p->getComponent<Transform>();
			if (Collisions::collidesWithRotation(t1->getPos(), t1->getW(), t1->getH(), t1->getRotation(), t3->getPos(), t3->getW(), t3->getH(), t3->getRotation())) {
				sdl.soundEffects().at("explosion").play();
				p->getComponent<Health>()->damage();
				t3->getPos() = Vector2D{ (float)WIN_WIDTH / 2, (float)WIN_HEIGHT / 2 }; t3->getRotation() = 0; t3->getVel() = Vector2D{ 0,0 };
				asteroidsManager_->destroyAllAsteroids();
				reset = true;
			}
		}
		++it;
	}
	if (reset) {
		refresh();
		asteroidsManager_->pauseTimer();
		if (manager_->isPlayerAlive()) game->pauseGame();
		else {
			game->playerLoses();
			manager_->getPlayer()->getComponent<Health>()->resetLives();
		}
		asteroidsManager_->createAsteroids(10);
	}
}

PlayState::~PlayState() {
	Music::haltMusic();
	delete manager_;
	delete asteroidsManager_;
}

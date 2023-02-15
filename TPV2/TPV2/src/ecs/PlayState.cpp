#include "PlayState.h"

PlayState::PlayState(Game* g, double w, double h) : GameState(w,h){ // Constructora
	game = g;
}
const std::string PlayState::s_playID = "PLAY";//ID del estado

void PlayState::update() //Se actualizan los objetos de la lista
{
	manager_->update();
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
	return true;
}

void PlayState::refresh()
{
	manager_->refresh();
}

void PlayState::checkCollisions() {
	vector<Entity*>& entities = manager_->getEntities();
	for (Entity* e : entities) {
		if (e->hasComponent(ecs::_GENERATIONS)) {
			Transform* t1 = e->getComponent<Transform>(ecs::_TRANSFORM);
			for (Entity* e2 :entities) {
				Transform* t2 = e2->getComponent<Transform>(ecs::_TRANSFORM);
				if (!e2->hasComponent(ecs::_GENERATIONS) && Collisions::collidesWithRotation(t1->getPos(), t1->getW(), t1->getH(), t1->getRotation(), t2->getPos(), t2->getW(), t2->getH(), t2->getRotation())) {
					if (e2->hasComponent(ecs::_HEALTH)) {
						asteroidsManager_->destroyAllAsteroids();
						e2->getComponent<Health>(ecs::_HEALTH)->damage();
						t2->getPos() = Vector2D{(float)WIN_WIDTH / 2, (float)WIN_HEIGHT / 2 };
						t2->getRotation() = 0;
						t2->getVel() = Vector2D{ 0,0 };
						game->pauseGame();
						asteroidsManager_->createAsteroids(10);
					}
					else if (e2->hasComponent(ecs::_DISABLEONEXIT)) {
						asteroidsManager_->onCollision(e);
						e2->setAlive(false);
					}
				}
			}
		}
	}
}

PlayState::~PlayState() {
	delete manager_;
	delete asteroidsManager_;
}

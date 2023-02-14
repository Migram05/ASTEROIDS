#include "Manager.h"
#include "Entity.h"
Manager::Manager(Game* g) : ents_(), game(g)
{
    ents_.reserve(100);
}

Entity* Manager::addEntity()
{
    Entity* e = new Entity();
    e->setAlive(true);
    e->setContext(this);
    ents_.push_back(e);
    return e;
}

void Manager::refresh()
{
    ents_.erase(
        std::remove_if(ents_.begin(), ents_.end(), [](Entity* e) {
        if (e->isAlive()) {
            return false;
        }
        else {
            delete e;
            return true;
        }
    }), 
    ents_.end());
}

void Manager::update()
{
    auto n = ents_.size();
    for (auto i = 0u; i < n; i++) ents_[i]->update();
}

void Manager::render()
{
    auto n = ents_.size();
    for (auto i = 0u; i < n; i++) ents_[i]->render();
}
void Manager::spawnShot(Vector2D pos, Vector2D dir, float rot) {
    Entity* aux = addEntity();
    aux->setContext(this);
    aux->addComponent<Transform>(ecs::_TRANSFORM, pos, 5, 30, dir, rot);
    aux->addComponent<Image>(ecs::_IMAGE, game->getTexture(Fire));
    aux->addComponent<DisableOnExit>(ecs::_DISABLEONEXIT, game->WIN_WIDTH, game->WIN_HEIGHT);
}

void Manager::createPlayer()
{
    player = addEntity();
    player->addComponent<Transform>(ecs::_TRANSFORM, game->WIN_WIDTH /2 -15, game->WIN_HEIGHT /2 -15, 30, 30);
    player->addComponent<Image>(ecs::_IMAGE, game->getTexture(Fighter1));
    player->addComponent<FighterControl>(ecs::_CTRL);
    player->addComponent<DeAcceleration>(ecs::_DEACCELERATION);
    player->addComponent<ShowOpposite>(ecs::_SHOWOPOSITE, game->WIN_WIDTH, game->WIN_HEIGHT);
    player->addComponent<Health>(ecs::_HEALTH, game->getTexture(Heart), game->WIN_WIDTH, game->WIN_HEIGHT, 5);
    player->addComponent<Gun>(ecs::_GUN);
}

Texture* Manager::getTexture(int t)
{
    return game->getTexture(t);
}

const int Manager::getWidth()
{
    return game->WIN_WIDTH;
}

const int Manager::getHeight()
{
    return game->WIN_HEIGHT;
}

void Manager::exitGame()
{
    game->exitGame();
}

vector<Entity*>& Manager::getEntities()
{
    return ents_;
}

Manager::~Manager()
{
    for (auto e : ents_) {
        delete e;
    }
}

#include "Manager.h"
#include "Entity.h"
Manager::Manager(Game* g) : entsByGroup_(), game(g)
{
    
    for (auto& groupEntities : entsByGroup_) {
        groupEntities.reserve(100);
    }
}

Entity* Manager::addEntity(ecs::grpId_type gId = ecs::_grp_GENERAL)
{
    Entity* e = new Entity();
    e->setAlive(true);
    e->setContext(this);
    entsByGroup_[gId].push_back(e);
    return e;
}

void Manager::refresh()
{
    for (ecs::grpId_type gId = 0; gId < ecs::maxGroupId; gId++) {
        auto& grpEnts = entsByGroup_[gId];
        grpEnts.erase(
            std::remove_if(grpEnts.begin(), grpEnts.end(), [](Entity* e) {
                    if (e->isAlive()) {
                        return false;
                    }
                    else {
                        delete e;
                        return true;
                    }
            }),
            grpEnts.end());
    }
}

void Manager::update()
{
    for (auto& ents : entsByGroup_) {
        auto n = ents.size();
        for (auto i = 0u; i < n; i++)
            ents[i]->update();
    }
}

void Manager::render()
{
    for (auto& ents : entsByGroup_) {
        auto n = ents.size();
        for (auto i = 0u; i < n; i++)
            ents[i]->render();
    }
}
void Manager::spawnShot(Vector2D pos, Vector2D dir, float rot) {
    Entity* aux = addEntity(ecs::_grp_BULLETS);
    aux->setContext(this);
    aux->addComponent<Transform>(pos, 5, 30, dir, rot);
    aux->addComponent<Image>(game->getTexture(Fire));
    aux->addComponent<DisableOnExit>(game->WIN_WIDTH, game->WIN_HEIGHT);
}

void Manager::createPlayer()
{
    player = addEntity(ecs::_grp_PLAYER);
    player->addComponent<Transform>(game->WIN_WIDTH /2 -15, game->WIN_HEIGHT /2 -15, 30, 30);
    player->addComponent<Image>( game->getTexture(Fighter1));
    player->addComponent<FighterControl>();
    player->addComponent<DeAcceleration>();
    player->addComponent<ShowOpposite>(game->WIN_WIDTH, game->WIN_HEIGHT);
    player->addComponent<Health>(game->getTexture(Heart), game->WIN_WIDTH, game->WIN_HEIGHT, 5);
    player->addComponent<Gun>(10);
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

bool Manager::isPlayerAlive()
{
    return player->getComponent<Health>()->getLives() > 0;
}

const vector<Entity*>& Manager::getEntitiesByGroup(grpId_type gId = ecs::_grp_GENERAL)
{
    return entsByGroup_[gId];
}

void Manager::addToGroupList(grpId_type gId, Entity* e)
{
    entsByGroup_[gId].push_back(e);
}

Manager::~Manager()
{
    for (auto& ents : entsByGroup_) {
        for (auto e : ents)
            delete e;
    }
}

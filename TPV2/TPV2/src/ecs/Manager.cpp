#include "Manager.h"
#include "Entity.h"
Manager::Manager(Game* g) : entsByGroup_(), game(g) //Constructora
{
    for (auto& groupEntities : entsByGroup_) { //Se reserva memoria
        groupEntities.reserve(100);
    }
}

Entity* Manager::addEntity(ecs::grpId_type gId = ecs::_grp_GENERAL) //Añade una entidad a su grupo correspondiente
{
    Entity* e = new Entity();
    e->setAlive(true);
    e->setContext(this);
    entsByGroup_[gId].push_back(e);
    return e;
}

void Manager::refresh() //Borra todas las entidades muertas
{
    for (ecs::grpId_type gId = 0; gId < ecs::maxGroupId; gId++) { //Pasa por los distintos grupos
        auto& grpEnts = entsByGroup_[gId];
        grpEnts.erase(
            std::remove_if(grpEnts.begin(), grpEnts.end(), [](Entity* e) {
                    if (e->isAlive()) {
                        return false;
                    }
                    else { //SI está muerta, se borra la entidad
                        delete e;
                        return true;
                    }
            }),
            grpEnts.end());
    }
}

void Manager::update() //Se actualizan todas las entidades
{
    for (auto& ents : entsByGroup_) {
        auto n = ents.size();
        for (auto i = 0u; i < n; i++)
            ents[i]->update();
    }
}

void Manager::render() //Se renderizan todas las entidades
{
    for (auto& ents : entsByGroup_) {
        auto n = ents.size();
        for (auto i = 0u; i < n; i++)
            ents[i]->render();
    }
}
void Manager::spawnShot(Vector2D pos, Vector2D dir, float rot) { //Creación de un disparo
    Entity* aux = addEntity(ecs::_grp_BULLETS); //Se añade al grupo de balas
    aux->setContext(this); 
    aux->addComponent<Transform>(pos, 5, 30, dir, rot);
    aux->addComponent<Image>(game->getTexture(Fire));
    aux->addComponent<DisableOnExit>(game->WIN_WIDTH, game->WIN_HEIGHT);
}

void Manager::createPlayer() //Se crea al jugador
{
    //EN este caso se guarda en una variable para poder referenciarla facilmente
    player = addEntity(ecs::_grp_PLAYER);
    player->addComponent<Transform>(game->WIN_WIDTH /2 -15, game->WIN_HEIGHT /2 -15, 30, 30);
    player->addComponent<Image>( game->getTexture(Fighter1));
    player->addComponent<FighterControl>();
    player->addComponent<DeAcceleration>();
    player->addComponent<ShowOpposite>(game->WIN_WIDTH, game->WIN_HEIGHT);
    player->addComponent<Health>(game->getTexture(Heart), game->WIN_WIDTH, game->WIN_HEIGHT, 5);
    player->addComponent<Gun>(10);
}

Texture* Manager::getTexture(int t) //Obtener textura de game
{
    return game->getTexture(t);
}

const int Manager::getWidth() //Devuelve el ancho
{
    return game->WIN_WIDTH;
}

const int Manager::getHeight()//Devuelve el alto
{
    return game->WIN_HEIGHT;
}

void Manager::exitGame() //Salir del juego
{
    game->exitGame();
}

bool Manager::isPlayerAlive() //Devuelve el estado del jugador
{
    return player->getComponent<Health>()->getLives() > 0;
}

const vector<Entity*>& Manager::getEntitiesByGroup(grpId_type gId = ecs::_grp_GENERAL) 
{
    //Devuelve un vector de las entidades de un grupo
    return entsByGroup_[gId];
}

void Manager::addToGroupList(grpId_type gId, Entity* e) //Añade entidad a grupo concreto
{
    entsByGroup_[gId].push_back(e);
}

Manager::~Manager() //Destructora
{
    for (auto& ents : entsByGroup_) { //Recorre todos los grupos
        for (auto e : ents)
            delete e;
    }
}

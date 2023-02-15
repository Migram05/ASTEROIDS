#include "AsteroidsManager.h"
#include "../ecs/Entity.h"
#include "../ecs/Manager.h"
AsteroidsManager::AsteroidsManager(Manager* m, Entity* p) : mngRef_(m), numAsteroids(10), player_(p)
{
    createAsteroids(numAsteroids);
}

AsteroidsManager::AsteroidsManager(Manager* m,Entity* p, int n) : mngRef_(m), player_(p)
{
    if (n > maxAsteroids) n = maxAsteroids;
    numAsteroids = n;
    createAsteroids(numAsteroids);
}

void AsteroidsManager::destroyAllAsteroids()
{
    vector<Entity*>& entities = mngRef_->getEntities();
    for (Entity* e : entities) {
        if (e->hasComponent(ecs::_GENERATIONS)) e->setAlive(false);
    }
}

void AsteroidsManager::createAsteroids(int n)
{
	for (int i = 0; i < n; ++i) {
        int type = rand() % 100;
        if (type < 70) createNormalAsteroid();
        else createSeakingAsteroid();
	}
}

void AsteroidsManager::createNormalAsteroid()
{
    Vector2D spawnPos(rand() % mngRef_->getWidth(), rand() % mngRef_->getHeight());
    Entity* a = mngRef_->addEntity();
    Vector2D dir = mngRef_->getPlayer()->getComponent<Transform>(ecs::_TRANSFORM)->getPos() - spawnPos;
    dir = dir + Vector2D(rand() % 200 - 100, rand() % 200 - 100);
    float mag = sqrt((dir.getX() * dir.getX()) + (dir.getY() * dir.getY()));
    dir = Vector2D((dir.getX() / mag) / 10, (dir.getY() / mag) / 10);
    dir.normalize();
    a->addComponent<Transform>(ecs::_TRANSFORM, spawnPos, 10, 10, dir);
    a->addComponent<FramedImage>(ecs::_FRAMEDIMAGE, mngRef_->getTexture(GrayAsteroid), 5, 6, 200);
    a->addComponent<ShowOpposite>(ecs::_SHOWOPOSITE, mngRef_->getWidth(), mngRef_->getHeight());
    a->addComponent<Generations>(ecs::_GENERATIONS);
}

void AsteroidsManager::createSeakingAsteroid()
{
    Vector2D spawnPos(rand() % mngRef_->getWidth(), rand() % mngRef_->getHeight());
    Entity* a = mngRef_->addEntity();
    Vector2D dir = mngRef_->getPlayer()->getComponent<Transform>(ecs::_TRANSFORM)->getPos() - spawnPos;
    //float mag = sqrt((dir.getX() * dir.getX()) + (dir.getY() * dir.getY()));
    //dir = Vector2D((dir.getX() / mag) / 2, (dir.getY() / mag) / 2);
    dir.normalize() / 2;
    a->addComponent<Transform>(ecs::_TRANSFORM, spawnPos, 10, 10);
    a->addComponent<FramedImage>(ecs::_FRAMEDIMAGE, mngRef_->getTexture(GoldAsteroid), 5, 6, 200);
    a->addComponent<ShowOpposite>(ecs::_SHOWOPOSITE, mngRef_->getWidth(), mngRef_->getHeight());
    a->addComponent<Follow>(ecs::_FOLLOW, player_);
    a->addComponent<Generations>(ecs::_GENERATIONS);
}

void AsteroidsManager::onCollision(Entity* e)
{
    e->setAlive(false);
    int gen = e->getComponent<Generations>(ecs::_GENERATIONS)->GetGeneration();
    --gen;
    if (gen > 1) {
        Transform* tr = e->getComponent<Transform>(ecs::_TRANSFORM);
        auto r = sdlutils().rand().nextInt(0, 360);
        auto pos = tr->getPos() + tr->getVel().rotate(r) * 2 * std::max(tr->getW(), tr->getH());
        auto vel = tr->getVel().rotate(r) * 1.1f;
        Entity* a = mngRef_->addEntity();
        a->addComponent<Transform>(ecs::_TRANSFORM, pos, 10, 10, vel);
        a->addComponent<FramedImage>(ecs::_FRAMEDIMAGE, mngRef_->getTexture(GrayAsteroid), 5, 6, 200);
        a->addComponent<ShowOpposite>(ecs::_SHOWOPOSITE, mngRef_->getWidth(), mngRef_->getHeight());
        a->addComponent<Generations>(ecs::_GENERATIONS, gen);
    }
    numAsteroids--;
}

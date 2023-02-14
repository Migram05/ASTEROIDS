#include "AsteroidsManager.h"
#include "../ecs/Entity.h"
#include "../ecs/Manager.h"
AsteroidsManager::AsteroidsManager(Manager* m, Entity* p) : mngRef_(m), numAsteroids(10), player_(p)
{
    createAsteroids(numAsteroids);
}

AsteroidsManager::AsteroidsManager(Manager* m,Entity* p, int n) : mngRef_(m), numAsteroids(n), player_(p)
{
    createAsteroids(numAsteroids);
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
    float mag = sqrt((dir.getX() * dir.getX()) + (dir.getY() * dir.getY()));
    dir = Vector2D((dir.getX() / mag) / 10, (dir.getY() / mag) / 10);
    a->addComponent<Transform>(ecs::_TRANSFORM, spawnPos, 30, 30, dir);
    a->addComponent<FramedImage>(ecs::_FRAMEDIMAGE, mngRef_->getTexture(GrayAsteroid), 5, 6, 200);
    a->addComponent<ShowOpposite>(ecs::_SHOWOPOSITE, mngRef_->getWidth(), mngRef_->getHeight());
}

void AsteroidsManager::createSeakingAsteroid()
{
    Vector2D spawnPos(rand() % mngRef_->getWidth(), rand() % mngRef_->getHeight());
    Entity* a = mngRef_->addEntity();
    Vector2D dir = mngRef_->getPlayer()->getComponent<Transform>(ecs::_TRANSFORM)->getPos() - spawnPos;
    float mag = sqrt((dir.getX() * dir.getX()) + (dir.getY() * dir.getY()));
    dir = Vector2D((dir.getX() / mag) / 2, (dir.getY() / mag) / 2);
    a->addComponent<Transform>(ecs::_TRANSFORM, spawnPos, 30, 30);
    a->addComponent<FramedImage>(ecs::_FRAMEDIMAGE, mngRef_->getTexture(GoldAsteroid), 5, 6, 200);
    a->addComponent<ShowOpposite>(ecs::_SHOWOPOSITE, mngRef_->getWidth(), mngRef_->getHeight());
    a->addComponent<Follow>(ecs::_FOLLOW, player_);
}

#include "AsteroidsManager.h"

AsteroidsManager::AsteroidsManager(Manager* m) : mngRef_(m), numAsteroids(10)
{
    createAsteroids(numAsteroids);
}

AsteroidsManager::AsteroidsManager(Manager* m, int n) : mngRef_(m), numAsteroids(n)
{
    createAsteroids(numAsteroids);
}

void AsteroidsManager::createAsteroids(int n)
{
	for (int i = 0; i < n; ++i) {
        Vector2D spawnPos(rand() % mngRef_->getWidth(), rand() % mngRef_->getHeight());
        Entity* a = mngRef_->addEntity();
        Vector2D dir = mngRef_->getPlayer()->getComponent<Transform>(ecs::_TRANSFORM)->getPos() - spawnPos;
        float mag = sqrt((dir.getX() * dir.getX()) + (dir.getY() * dir.getY()));
        dir = Vector2D((dir.getX() / mag) / 2, (dir.getY() / mag) / 2);
        a->addComponent<Transform>(ecs::_TRANSFORM, spawnPos, 30, 30);
        a->addComponent<FramedImage>(ecs::_FRAMEDIMAGE, mngRef_->getTexture(GrayAsteroid), 5, 6, 200);
        a->addComponent<ShowOpposite>(ecs::_SHOWOPOSITE, mngRef_->getWidth(), mngRef_->getHeight());
        a->addComponent<Follow>(ecs::_FOLLOW, player_);
	}
}

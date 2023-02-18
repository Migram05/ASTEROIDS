#include "AsteroidsManager.h"
#include "../ecs/Entity.h"
#include "../ecs/Manager.h"
AsteroidsManager::AsteroidsManager(Manager* m, Entity* p) : mngRef_(m) ,player_(p)
{
    createAsteroids(defaultNum);
}

AsteroidsManager::AsteroidsManager(Manager* m,Entity* p, int n) : mngRef_(m), player_(p)
{
    if (n > maxAsteroids) n = maxAsteroids;
    createAsteroids(n);
}

void AsteroidsManager::destroyAllAsteroids()
{
    vector<Entity*> entities = mngRef_->getEntitiesByGroup(ecs::_grp_ASTEROIDS);
    for (Entity* e : entities) {
        e->setAlive(false);
    }
    numAsteroids = 0;
}

void AsteroidsManager::createAsteroids(int n)
{
    auto& sdl = *SDLUtils::instance();
	for (int i = 0; i < n; ++i) {
        Vector2D spawnPos = borderSpawnLocation();
        Entity* a = mngRef_->addEntity(ecs::_grp_ASTEROIDS);
        Vector2D dir = mngRef_->getPlayer()->getComponent<Transform>()->getPos() - spawnPos;
        dir = dir + Vector2D(sdl.rand().nextInt(-100, 100) , sdl.rand().nextInt(-100, 100));
        dir = dir.normalize() * asteroidSpeed;
        a->addComponent<Transform>(spawnPos, 10, 10, dir);
        a->addComponent<ShowOpposite>(mngRef_->getWidth(), mngRef_->getHeight());
        int g = sdl.rand().nextInt(2, 4);
        a->addComponent<Generations>(g);
        int type = sdl.rand().nextInt(0, 100);
        if (type < 70) {
            a->addComponent<FramedImage>(mngRef_->getTexture(GrayAsteroid), 5, 6, 200);
        }
        else {
            a->addComponent<FramedImage>(mngRef_->getTexture(GoldAsteroid), 5, 6, 200);
            a->addComponent<Follow>(player_, asteroidSpeed);
        }
        numAsteroids++;
	}
}

void AsteroidsManager::createSmallerAsteroids(int n, int g, Entity* e) {
    for (int i = 0; i < n; ++i) {
        Transform* tr = e->getComponent<Transform>();
        auto r = sdlutils().rand().nextInt(0, 360);
        auto pos = tr->getPos() + tr->getVel().rotate(r) * 2 * std::max(tr->getW(), tr->getH());
        auto vel = tr->getVel().rotate(r) * 1.1f;
        Entity* a = mngRef_->addEntity(ecs::_grp_ASTEROIDS);
        a->addComponent<Transform>(pos, 10, 10, vel);
        a->addComponent<ShowOpposite>(mngRef_->getWidth(), mngRef_->getHeight());
        a->addComponent<Generations>(g);
        if (e->hasComponent(ecs::_FOLLOW)) {
            a->addComponent<FramedImage>( mngRef_->getTexture(GoldAsteroid), 5, 6, 200);
            a->addComponent<Follow>(player_, asteroidSpeed);
        }
        else a->addComponent<FramedImage>(mngRef_->getTexture(GrayAsteroid), 5, 6, 200);
    }
}

Vector2D AsteroidsManager::borderSpawnLocation()
{
    auto& sdl = *SDLUtils::instance();
    Vector2D sPosition;
    int r = rand() % 2;
    if (r == 0) {
        int r2 = rand() % 2;
        if (r2 == 0) {
            sPosition = Vector2D(0, sdl.rand().nextInt(0, mngRef_->getHeight()));
        }
        else {
            sPosition = Vector2D(mngRef_->getWidth(), sdl.rand().nextInt(0, mngRef_->getHeight()));
        }
    }
    else {
        int r2 = rand() % 2;
        if (r2 == 0) {
            sPosition = Vector2D(sdl.rand().nextInt(0, mngRef_->getWidth()), 0);
        }
        else {
            sPosition = Vector2D(sdl.rand().nextInt(0, mngRef_->getWidth()), mngRef_->getHeight());
        }
    }
    return sPosition;
}

void AsteroidsManager::addAsteroidFrequency()
{
    if (pause) {
        pause = false;
        timer_ = SDL_GetTicks();
    }
    if (SDL_GetTicks() - timer_ >= AsteroidTime * 1000) {
        createAsteroids(1);
        timer_ += AsteroidTime * 1000;
    }
}

void AsteroidsManager::onCollision(Entity* e)
{
    e->setAlive(false);
    int gen = e->getComponent<Generations>()->GetGeneration();
    --gen;
    numAsteroids--;
    if (gen > 0) {
        int num = maxAsteroids - numAsteroids;
        if (num > maxDivision) num = maxDivision;
        numAsteroids += num;
        createSmallerAsteroids(num, gen, e);
    }
    if (numAsteroids <= 0) cout << "GANASTE" << endl;
    
}

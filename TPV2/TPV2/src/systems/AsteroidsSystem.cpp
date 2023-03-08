#include "AsteroidsSystem.h"
#include "../ecs/Manager.h"
void AsteroidsSystem::receive(const Message& m)
{
	switch (m.id)
	{
	case _m_NEWGAME: onRoundStart();
	case _m_PLAYERLOST: onRoundOver();
	default: break;
	}
}

void AsteroidsSystem::initSystem()
{
}

void AsteroidsSystem::update()
{
    for (auto e : mngr_->getEntitiesByGroup(ecs::_grp_ASTEROIDS)) {
        Entity* p = mngr_->getPlayer();
        auto tr_ = mngr_->getComponent<Transform>(e);
        auto& position_ = tr_->getPos(); auto& velocity_ = tr_->getVel();
        if (mngr_->hasComponent<Follow>(e)) { //Se comprueba si el asteroide es de oro, y por lo tanto tiene que seguir al jugador
            Vector2D dir = mngr_->getComponent<Transform>(p)->getPos() - tr_->getPos();
            dir = dir.normalize() * asteroidSpeed;
            velocity_ = dir; //Se aplica el vector direcci�n
        }
        position_ = position_ + velocity_; //Se ajusta la posici�n del asteroide seg�n su direcci�n

        if (position_.getX() + tr_->getW() < 0) position_ = Vector2D{ (float)mngr_->getWidth() , position_.getY() };
        else if (position_.getX() > mngr_->getWidth()) position_ = Vector2D{ 0 , position_.getY() };

        if (position_.getY() + tr_->getH() < 0) position_ = Vector2D{ position_.getX() ,(float)mngr_->getHeight() };
        else if (position_.getY() > mngr_->getHeight()) position_ = Vector2D{ position_.getX() , 0 };
    }
}

void AsteroidsSystem::onCollision_AsteroidBullet(Entity* a)
{
}

void AsteroidsSystem::onRoundOver()
{
}

void AsteroidsSystem::onRoundStart()
{
	createAsteroids(defaultSpawnNum);
}

void AsteroidsSystem::createAsteroids(int n)
{
    auto& sdl = *SDLUtils::instance();
    for (int i = 0; i < n; ++i) {
        Vector2D spawnPos = borderSpawnLocation(); //Busca una posici�n en los bordes
        Entity* a = mngr_->addEntity(ecs::_grp_ASTEROIDS); //A�ade la entidad
        Entity* p = mngr_->getPlayer();
        Vector2D dir = mngr_->getComponent<Transform>(p)->getPos() - spawnPos; //Crea una direcci�n aleatoria
        dir = dir + Vector2D(sdl.rand().nextInt(-100, 100), sdl.rand().nextInt(-100, 100));
        dir = dir.normalize() * asteroidSpeed;
        mngr_->addComponent<Transform>(a, spawnPos, 10, 10, dir); //A�ade componentes
        mngr_->addComponent<ShowOpposite>(a, mngr_->getWidth(), mngr_->getHeight());
        int g = sdl.rand().nextInt(2, 4); //Generaci�n aleatoria entre (2 y 3)
        mngr_->addComponent<Generations>(a, g);
        int type = sdl.rand().nextInt(0, 100); //Decide si ser� asteroide dorado o no
        if (type < 70) {
            mngr_->addComponent<FramedImage>(a, mngr_->getTexture(GrayAsteroid), 5, 6, 200);
        }
        else {
            mngr_->addComponent<FramedImage>(a, mngr_->getTexture(GoldAsteroid), 5, 6, 200);
            mngr_->addComponent<Follow>(a, p, asteroidSpeed); //Componente exclusivo del dorado
        }
        numAsteroids_++;
    }
}
Vector2D AsteroidsSystem::borderSpawnLocation() //Busca una posici�n en los bordes
{
    auto& sdl = *SDLUtils::instance();
    Vector2D sPosition;
    int r = rand() % 2; //Decide si estar� en el borde lateral o en el borde superior
    if (r == 0) {
        int r2 = rand() % 2; //Vuelve a decidir si estar� en un lado u otro
        if (r2 == 0) {
            sPosition = Vector2D(0, sdl.rand().nextInt(0, mngr_->getHeight()));
        }
        else {
            sPosition = Vector2D(mngr_->getWidth(), sdl.rand().nextInt(0, mngr_->getHeight()));
        }
    }
    else { //Mismo procedimiento que antes pero para el otro caso
        int r2 = rand() % 2;
        if (r2 == 0) {
            sPosition = Vector2D(sdl.rand().nextInt(0, mngr_->getWidth()), 0);
        }
        else {
            sPosition = Vector2D(sdl.rand().nextInt(0, mngr_->getWidth()), mngr_->getHeight());
        }
    }
    return sPosition;
}

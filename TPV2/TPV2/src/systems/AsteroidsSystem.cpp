#include "AsteroidsSystem.h"
#include "../ecs/Manager.h"
void AsteroidsSystem::receive(const Message& m) 
{
	switch (m.id)
	{
    case _m_NEWGAME: onRoundStart(); break; //Cuando se empueza una partida nueva, se crean asteroides
	case (_m_PLAYERLOST ||_m_PLAYERWINS): onRoundOver(); break; //Al perder se destruyen los asteroides
    case _m_BULLETCOLLIDES: onCollision_AsteroidBullet(m.bulletCollision_data.a_); break; //Al colisionar con una bala, se destruye el asteroide
	default: break;
	}
}

void AsteroidsSystem::initSystem()
{
    
}

void AsteroidsSystem::update() //Se actualizan los asteroides
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

        //Efecto toroidal, en caso de salir de la pantalla, se muestra por el lado opuesto
        if (position_.getX() + tr_->getW() < 0) position_ = Vector2D{ (float)mngr_->getWidth() , position_.getY() };
        else if (position_.getX() > mngr_->getWidth()) position_ = Vector2D{ 0 , position_.getY() };

        if (position_.getY() + tr_->getH() < 0) position_ = Vector2D{ position_.getX() ,(float)mngr_->getHeight() };
        else if (position_.getY() > mngr_->getHeight()) position_ = Vector2D{ position_.getX() , 0 };
    }
    if (SDL_GetTicks() - timer_ >= AsteroidTime * 2000) { //Comprobaci�n para saber si estamos en pausa o no
        timer_ = SDL_GetTicks(); 
    }
    else if (SDL_GetTicks() - timer_ >= AsteroidTime * 1000 && numAsteroids_ < maxNum) { //Crea un asteroide dada la frecuencia
        //Crea un asteroide m�s si es posible
        createAsteroids(1);
        timer_ += AsteroidTime * 1000;
    }
}

void AsteroidsSystem::onCollision_AsteroidBullet(Entity* e)
{
    mngr_->setAlive(e, false); //Se desactiva el asteroide
    int gen = mngr_->getComponent<Generations>(e)->GetGeneration();
    --gen; //Se reduce su generaci�n
    numAsteroids_--;
    if (gen > 0) { //Si pude generar m�s asteroides, se divide
        int n = maxNum - numAsteroids_;
        if (n > maxDivision) n = maxDivision; //N�mero de asteroides, seg�n el m�ximo de divisi�n y el n�mero m�ximo de asteroides
        for (int i = 0; i < n; ++i) { //Crea asteroides
            Transform* tr = mngr_->getComponent<Transform>(e);
            auto r = sdlutils().rand().nextInt(0, 360);
            auto pos = tr->getPos() + tr->getVel().rotate(r) * 2 * std::max(tr->getW(), tr->getH());
            auto vel = tr->getVel().rotate(r) * 1.1f;
            Entity* a = mngr_->addEntity(ecs::_grp_ASTEROIDS);
            mngr_->addComponent<Transform>(a, pos, 10, 10, vel);
            mngr_->addComponent<ShowOpposite>(a, mngr_->getWidth(), mngr_->getHeight());
            mngr_->addComponent<Generations>(a, gen);
            //Si el asteroide originario era de oro, sus "hijos" tambien lo ser�n
            if (mngr_->hasComponent<Follow>(e)) {
                mngr_->addComponent<FramedImage>(a, mngr_->getTexture(GoldAsteroid), 5, 6, 200);
                mngr_->addComponent<Follow>(a, mngr_->getPlayer(), asteroidSpeed);
            }
            else mngr_->addComponent<FramedImage>(a, mngr_->getTexture(GrayAsteroid), 5, 6, 200);
            numAsteroids_++;
        }
    }
    if (numAsteroids_ <= 0) { //En caso de que no pueda crear m�s asteroides, comprueba el n�mero de asteroides, si es 0, gana el jugador
        Message msg; msg.id = _m_PLAYERWINS;
        mngr_->send(msg);
    }
}

void AsteroidsSystem::onRoundOver() //Destruye todos los asteroides
{
    vector<Entity*> entities = mngr_->getEntitiesByGroup(ecs::_grp_ASTEROIDS); //Busca en el grupo asteroids
    for (Entity* e : entities) {
        mngr_->setAlive(e, false);
    }
    numAsteroids_ = 0;
}

void AsteroidsSystem::onRoundStart() //Crea un n�mero por defecto de asteroides
{
	createAsteroids(defaultSpawnNum);
}

void AsteroidsSystem::createAsteroids(int n) //Crea asteroides
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

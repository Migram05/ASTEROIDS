#include "AsteroidsManager.h"
#include "../ecs/Entity.h"
#include "../ecs/Manager.h"
AsteroidsManager::AsteroidsManager(Manager* m, Entity* p) : mngRef_(m) ,player_(p) //Constuctora por defecto
{
    createAsteroids(defaultNum);
}

AsteroidsManager::AsteroidsManager(Manager* m,Entity* p, int n) : mngRef_(m), player_(p) //Constructora con ajuste de numAsteroids
{
    if (n > maxAsteroids) n = maxAsteroids;
    createAsteroids(n);
}

void AsteroidsManager::destroyAllAsteroids() //Destruye todos los asteroides
{
    vector<Entity*> entities = mngRef_->getEntitiesByGroup(ecs::_grp_ASTEROIDS); //Busca en el grupo asteroids
    for (Entity* e : entities) {
        mngRef_->setAlive(e, false);
    }
    numAsteroids = 0;
}

void AsteroidsManager::createAsteroids(int n) //Crea un número de asteroides n
{
    auto& sdl = *SDLUtils::instance();
	for (int i = 0; i < n; ++i) {
        Vector2D spawnPos = borderSpawnLocation(); //Busca una posición en los bordes
        Entity* a = mngRef_->addEntity(ecs::_grp_ASTEROIDS); //Añade la entidad
        Entity* p = mngRef_->getPlayer();
        Vector2D dir = mngRef_->getComponent<Transform>(p)->getPos() - spawnPos; //Crea una dirección aleatoria
        dir = dir + Vector2D(sdl.rand().nextInt(-100, 100) , sdl.rand().nextInt(-100, 100));
        dir = dir.normalize() * asteroidSpeed;
        mngRef_->addComponent<Transform>(a, spawnPos, 10, 10, dir); //Añade componentes
        mngRef_->addComponent<ShowOpposite>(a, mngRef_->getWidth(), mngRef_->getHeight());
        int g = sdl.rand().nextInt(2, 4); //Generación aleatoria entre (2 y 3)
        mngRef_->addComponent<Generations>(a, g);
        int type = sdl.rand().nextInt(0, 100); //Decide si será asteroide dorado o no
        if (type < 70) {
            mngRef_->addComponent<FramedImage>(a, mngRef_->getTexture(GrayAsteroid), 5, 6, 200);
        }
        else {
            mngRef_->addComponent<FramedImage>(a, mngRef_->getTexture(GoldAsteroid), 5, 6, 200);
            mngRef_->addComponent<Follow>(a, player_, asteroidSpeed); //Componente exclusivo del dorado
        }
        numAsteroids++;
	}
}

void AsteroidsManager::createSmallerAsteroids(int n, int g, Entity* e) { //Creación de un asteroide más pequeño
    for (int i = 0; i < n; ++i) {
        Transform* tr = mngRef_->getComponent<Transform>(e);
        auto r = sdlutils().rand().nextInt(0, 360);
        auto pos = tr->getPos() + tr->getVel().rotate(r) * 2 * std::max(tr->getW(), tr->getH());
        auto vel = tr->getVel().rotate(r) * 1.1f;
        Entity* a = mngRef_->addEntity(ecs::_grp_ASTEROIDS);
        mngRef_->addComponent<Transform>(a, pos, 10, 10, vel);
        mngRef_->addComponent<ShowOpposite>(a, mngRef_->getWidth(), mngRef_->getHeight());
        mngRef_->addComponent<Generations>(a, g);
        //Si el asteroide originario era de oro, sus "hijos" tambien lo serán
        if (mngRef_->hasComponent<Follow>(e)) {
            mngRef_->addComponent<FramedImage>(a, mngRef_->getTexture(GoldAsteroid), 5, 6, 200);
            mngRef_->addComponent<Follow>(a, player_, asteroidSpeed);
        }
        else mngRef_->addComponent<FramedImage>(a, mngRef_->getTexture(GrayAsteroid), 5, 6, 200);
    }
}

Vector2D AsteroidsManager::borderSpawnLocation() //Busca una posición en los bordes
{
    auto& sdl = *SDLUtils::instance();
    Vector2D sPosition;
    int r = rand() % 2; //Decide si estará en el borde lateral o en el borde superior
    if (r == 0) {
        int r2 = rand() % 2; //Vuelve a decidir si estará en un lado u otro
        if (r2 == 0) {
            sPosition = Vector2D(0, sdl.rand().nextInt(0, mngRef_->getHeight()));
        }
        else {
            sPosition = Vector2D(mngRef_->getWidth(), sdl.rand().nextInt(0, mngRef_->getHeight()));
        }
    }
    else { //Mismo procedimiento que antes pero para el otro caso
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

void AsteroidsManager::addAsteroidFrequency() //Crea un asteroide cada cierto tiempo
{
    if (SDL_GetTicks() - timer_ >= AsteroidTime * 2000) timer_ = SDL_GetTicks(); //Comprobación para saber si estamos en pausa o no
    else if (SDL_GetTicks() - timer_ >= AsteroidTime * 1000 && numAsteroids < maxAsteroids) {
        //Crea un asteroide más si es posible
        createAsteroids(1);
        timer_ += AsteroidTime * 1000;
    }
}

void AsteroidsManager::onCollision(Entity* e) //En caso de haber una colisión
{
    mngRef_->setAlive(e, false); //Se desactiva el asteroide
    int gen = mngRef_->getComponent<Generations>(e)->GetGeneration(); 
    --gen; //Se reduce su generación
    numAsteroids--;
    if (gen > 0) { //En caso de poder dividirse
        int num = maxAsteroids - numAsteroids;
        if (num > maxDivision) num = maxDivision; //Se calcula el número de divisiones (nunca supera el máximo)
        numAsteroids += num;
        createSmallerAsteroids(num, gen, e); //Se crean tantos asteroides como hagan falta
    }
    if (numAsteroids <= 0) cout << "GANASTE" << endl; //Si no hubiera asteroides, se gana la partida
    
}

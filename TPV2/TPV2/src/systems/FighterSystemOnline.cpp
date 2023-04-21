#include "FighterSystemOnline.h"
#include "../ecs/Manager.h"
#include "../ecs/MultiplayerState.h"
void FighterSystemOnline::receive(const Message& m)
{
	switch (m.id)
	{
	case _m_RESETPLAYERS: onFighterCollides(m.resetShipsData.resetLives); break; //Ambos eventos colocan al caza en el centro de la pantalla
	case _m_CHANGEINDEX: p = mngr_->getPlayer(mngr_->getPlayerIndex()); break;
	case _m_SHIPSTATE: setPlayerState(m.shipData.idx, m.shipData.pX, m.shipData.pY, m.shipData.R); break;
	default: break;
	}
}

void FighterSystemOnline::initSystem() //Al crear el sistema, se crea el jugador
{
	mngr_->createPlayer(2);//Crea 2 jugadores
	p = mngr_->getPlayer(mngr_->getPlayerIndex()); //Asigna al jugador su nave según el índice
	currentState = static_cast<MultiplayerState*>(mngr_->getGame()->getState());
}

void FighterSystemOnline::update() //Afctualiza la posición del jugador
{
	updatePosition();
	speedReduction();
	screenPositionCheck();
	moveAllPlayers();
}

void FighterSystemOnline::setPlayerState(int index, int posX, int posY, int Rot)
{
	//Se ajusta el estado de un jugador segun su indice
	auto player = mngr_->getPlayer(index); //Se obtiene el jugador que corresponde al index
	auto tr_ = mngr_->getComponent<Transform>(player);
	tr_->setPos(posX, posY);
	tr_->setRotation(Rot);
}

void FighterSystemOnline::moveAllPlayers() //Mueve al jugador
{
	auto mP = mngr_->getPlayer(mngr_->getPlayerIndex());
	auto tr_ = mngr_->getComponent<Transform>(mP);
	auto& position_ = tr_->getPos();
	Vector2D& v = tr_->getVel(); //Obtiene velocidad
	position_ = position_ + v; //Actualiza la posición
}


void FighterSystemOnline::updatePosition() //Mueve al caza
{
	auto tr_ = mngr_->getComponent<Transform>(p);
	auto gun_ = mngr_->getComponent<Gun>(p);
	auto& position_ = tr_->getPos();
	Vector2D& v = tr_->getVel(); //Obtiene velocidad
	float& fRotation = tr_->getRotation(); //Rotacion del caza
	Vector2D& forwardVector = tr_->getForward(); //Vector forward actual
	Vector2D& lastForward = tr_->getLastForward(); //Ultimo vector forward
	double rad = (fRotation) * (M_PI / 180);
	float c = cos(rad), s = sin(rad);
	forwardVector = Vector2D{ s, c }; //Se actualiza el vector forward según su rotación
	bool shot = false;
	SDL_Event event;
	auto& sdl = *SDLUtils::instance();
	Message msg;
	while (SDL_PollEvent(&event)) { //Controla la entrada
		if (event.type == SDL_KEYDOWN) {
			switch (event.key.keysym.sym)
			{
			case SDLK_w: {lastForward = forwardVector; v = Vector2D{ lastForward.getX() * speed, lastForward.getY() * -speed }; sdl.soundEffects().at("thrust").play(); 
				break; } //Avanza
			case SDLK_a: fRotation -= rotationSpeed; if (fRotation < -360) fRotation = 0; 
				break; //Rotación
			case SDLK_d: fRotation += rotationSpeed; if (fRotation > 360) fRotation = 0;  
				break;
			case SDLK_s: { //Dispara las armas
				auto& sdl = *SDLUtils::instance();
				if (sdl.currRealTime() - gun_->getLastShotTime() >= gun_->getShootRate()) { //Se comprueba la cadencia
					sdl.soundEffects().at("shot").play(); //Efecto de sonido
					gun_->setLastShoot(SDL_GetTicks());
					Vector2D dir = Vector2D{ forwardVector.getX() * gun_->getSpeed(), forwardVector.getY() * -gun_->getSpeed() };
					msg.id = _m_SHOOT; msg.shot_data.pos_ = (position_ + (dir * 4)); msg.shot_data.dir_ = dir; msg.shot_data.r_ = fRotation;
					mngr_->send(msg); //Envio del mensaje de disparo para el sistema de balas
					shot = true;
				} break;
			}
			case SDLK_ESCAPE: { //mensaje de salida
				currentState->sendMessage("Reset"); 
				msg.id = _m_EXIT;
				mngr_->send(msg, true);
				return;
			}
			default: break;
			}
		}
	}
	size_t n = 4;
	std::string pX = to_string((int)position_.getX());
	int precisionX = n - std::min(n, pX.size());
	pX.insert(0, precisionX, '0');
	std::string pY = to_string((int)position_.getY());
	int precisionY = n - std::min(n, pY.size());
	pY.insert(0, precisionY, '0');
	std::string R = to_string((int)fRotation);
	int precisionR = n - std::min(n, R.size());
	R.insert(0, precisionR, '0');
	currentState->sendMessage("M" + to_string(mngr_->getPlayerIndex())+ pX + pY + R + to_string(shot)); //Crea el mensaje con la info del jugador
}

void FighterSystemOnline::speedReduction() //Reduce la velocidad del caza
{
	for (int x = 0; x < nPlayers; ++x) {
		auto mP = mngr_->getPlayer(x);
		auto tr_ = mngr_->getComponent<Transform>(mP);
		Vector2D& v = tr_->getVel(); //Obtiene velocidad
		Vector2D& lastForward = tr_->getLastForward(); //Ultimo vector forward
		//Reducción del caza
		auto dAcceel_ = mngr_->getComponent<DeAcceleration>(mP);
		auto stopMargin = dAcceel_->getStopMargin();
		auto reduction = dAcceel_->getReduction();
		if (abs(v.getX()) > stopMargin || abs(v.getY()) > stopMargin) //En caso de que la velocidad sea superior al margen se reduce
			v = v + Vector2D{ lastForward.getX() * -reduction, lastForward.getY() * reduction };
		else v = { 0,0 }; //En caso de ser menor al margen, se detiene la nave
	}
}

void FighterSystemOnline::screenPositionCheck() //Movimiento toroidal
{
	//Comprueba que ningún caza se sale de la pantalla
	/*for (int x = 0; x < nPlayers; ++x) {
		auto mP = mngr_->getPlayer(x);

		auto tr_ = mngr_->getComponent<Transform>(mP);
		auto& position_ = tr_->getPos();
		//Movimiento toroidal
		if (position_.getX() + tr_->getW() < 0) position_ = Vector2D{ (float)mngr_->getWidth() , position_.getY() };
		else if (position_.getX() > mngr_->getWidth()) position_ = Vector2D{ 0 , position_.getY() };

		if (position_.getY() + tr_->getH() < 0) position_ = Vector2D{ position_.getX() ,(float)mngr_->getHeight() };
		else if (position_.getY() > mngr_->getHeight()) position_ = Vector2D{ position_.getX() , 0 };
	}*/
	auto mP = mngr_->getPlayer(mngr_->getPlayerIndex());

	auto tr_ = mngr_->getComponent<Transform>(mP);
	auto& position_ = tr_->getPos();
	//Movimiento toroidal
	if (position_.getX() + tr_->getW() < 0) position_ = Vector2D{ (float)mngr_->getWidth() , position_.getY() };
	else if (position_.getX() > mngr_->getWidth()) position_ = Vector2D{ 0 , position_.getY() };

	if (position_.getY() + tr_->getH() < 0) position_ = Vector2D{ position_.getX() ,(float)mngr_->getHeight() };
	else if (position_.getY() > mngr_->getHeight()) position_ = Vector2D{ position_.getX() , 0 };
	
}
void FighterSystemOnline::onFighterCollides(bool rL) //En caso de reiniciar su posición
{
	for (int x = 0; x < nPlayers; ++x) {
		auto mP = mngr_->getPlayer(x);
		auto tr_ = mngr_->getComponent<Transform>(mP);
		auto& pos = tr_->getPos();
		Vector2D& v = tr_->getVel(); //Obtiene velocidad
		v = Vector2D(0, 0); //Velocidad nula
		switch (x) //Según su indice se pone arriba o abajo
		{
		case 0: pos = Vector2D(mngr_->getWidth() / 2 - 15, mngr_->getHeight() * 0.9); break;
		case 1: pos = Vector2D(mngr_->getWidth() / 2 - 15, mngr_->getHeight() * 0.1); break;
		}
		if (rL) { //Si un jugador abandona, se reinician las vidas
			auto healthComp = mngr_->getComponent<Health>(mP);
			int& currentLives = healthComp->getLives();
			currentLives = healthComp->getInitialLives();
		}
	}
}
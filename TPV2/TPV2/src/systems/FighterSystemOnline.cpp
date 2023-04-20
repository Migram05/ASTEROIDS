#include "FighterSystemOnline.h"
#include "../ecs/Manager.h"
#include "../ecs/MultiplayerState.h"
void FighterSystemOnline::receive(const Message& m)
{
	switch (m.id)
	{
	case _m_RESETPLAYERS: onCollision_FighterAsteroid(); break; //Ambos eventos colocan al caza en el centro de la pantalla
	case _m_PLAYERWINS: onCollision_FighterAsteroid(); break;
	case _m_CHANGEINDEX: p = mngr_->getPlayer(mngr_->getPlayerIndex()); break;
	case _m_MOVESHIP: movePlayer(m.moveShip_data.indx); break;
	case _m_ROTATESHIP: rotatePlayer(m.rotateShip_data.indx, m.rotateShip_data.proportion); break;
	default: break;
	}
}

void FighterSystemOnline::initSystem() //Al crear el sistema, se crea el jugador
{
	mngr_->createPlayer(2);
	p = mngr_->getPlayer(mngr_->getPlayerIndex());
	currentState = static_cast<MultiplayerState*>(mngr_->getGame()->getState());
}

void FighterSystemOnline::update() //Afctualiza la posición del jugador
{
	updatePosition();
	speedReduction();
	screenPositionCheck();
	moveAllPlayers();
}

void FighterSystemOnline::movePlayer(int index) //Mueve a un jugador remoto
{
	auto pMove = mngr_->getPlayer(index); //Con el index recibido en el mensaje, se obtiene el jugador correspondiente
	auto& sdl = *SDLUtils::instance();

	auto tr_ = mngr_->getComponent<Transform>(pMove);
	auto& position_ = tr_->getPos();
	Vector2D& v = tr_->getVel(); //Obtiene velocidad
	float& fRotation = tr_->getRotation(); //Rotacion del caza
	Vector2D& forwardVector = tr_->getForward(); //Vector forward actual
	Vector2D& lastForward = tr_->getLastForward(); //Ultimo vector forward
	double rad = (fRotation) * (M_PI / 180);
	float c = cos(rad), s = sin(rad);
	forwardVector = Vector2D{ s, c }; //Se actualiza el vector forward según su rotación
	lastForward = forwardVector; 
	v = Vector2D{ lastForward.getX() * speed, lastForward.getY() * -speed }; 
	sdl.soundEffects().at("thrust").play();
	
}

void FighterSystemOnline::rotatePlayer(int index, int proportion) //Rota a un jugador remoto
{
	auto pRotate = mngr_->getPlayer(index); //Se obtiene el jugador que corresponde al index
	auto tr_ = mngr_->getComponent<Transform>(pRotate);
	float& fRotation = tr_->getRotation(); //Rotacion del caza
	fRotation += (rotationSpeed * proportion); 
	if (fRotation < -360 ||fRotation > 360) fRotation = 0;
}

void FighterSystemOnline::moveAllPlayers() //Ahora el movimiento de todas las naves lo gestiona este método
{
	for (int x = 0; x < nPlayers; ++x) {
		auto mP = mngr_->getPlayer(x);
		auto tr_ = mngr_->getComponent<Transform>(mP);
		auto& position_ = tr_->getPos();
		Vector2D& v = tr_->getVel(); //Obtiene velocidad
		position_ = position_ + v; //Actualiza la posición
	}
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
	SDL_Event event;
	auto& sdl = *SDLUtils::instance();
	Message msg;
	while (SDL_PollEvent(&event)) { //Controla la entrada
		if (event.type == SDL_KEYDOWN) {
			switch (event.key.keysym.sym)
			{
			case SDLK_w: {lastForward = forwardVector; v = Vector2D{ lastForward.getX() * speed, lastForward.getY() * -speed }; sdl.soundEffects().at("thrust").play(); 
				currentState->sendMessage("Move" + to_string(mngr_->getPlayerIndex()));
				break; } //Avanza
			case SDLK_a: fRotation -= rotationSpeed; if (fRotation < -360) fRotation = 0; currentState->sendMessage("RotateI" + to_string(mngr_->getPlayerIndex())); break; //Rotación
			case SDLK_d: fRotation += rotationSpeed; if (fRotation > 360) fRotation = 0; currentState->sendMessage("RotateD" + to_string(mngr_->getPlayerIndex())); break;
			case SDLK_s: { //Dispara las armas
				auto& sdl = *SDLUtils::instance();
				if (sdl.currRealTime() - gun_->getLastShotTime() >= gun_->getShootRate()) { //Se comprueba la cadencia
					sdl.soundEffects().at("shot").play(); //Efecto de sonido
					gun_->setLastShoot(SDL_GetTicks());
					Vector2D dir = Vector2D{ forwardVector.getX() * gun_->getSpeed(), forwardVector.getY() * -gun_->getSpeed() };
					msg.id = _m_SHOOT; msg.shot_data.pos_ = (position_ + (dir * 4)); msg.shot_data.dir_ = dir; msg.shot_data.r_ = fRotation;
					mngr_->send(msg);
					currentState->sendMessage("Shoot" + to_string(mngr_->getPlayerIndex()));
				} break;
			}
			case SDLK_ESCAPE: { //mensaje de salida
				msg.id = _m_EXIT; msg.mainMenuInfo.menuInfoData = "JUGADOR HA ABANDONADO";  mngr_->send(msg); currentState->sendMessage("Reset"); break;
			}
			default: break;
			}
		}
	}
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
	for (int x = 0; x < nPlayers; ++x) {
		auto mP = mngr_->getPlayer(x);

		auto tr_ = mngr_->getComponent<Transform>(mP);
		auto& position_ = tr_->getPos();
		//Movimiento toroidal
		if (position_.getX() + tr_->getW() < 0) position_ = Vector2D{ (float)mngr_->getWidth() , position_.getY() };
		else if (position_.getX() > mngr_->getWidth()) position_ = Vector2D{ 0 , position_.getY() };

		if (position_.getY() + tr_->getH() < 0) position_ = Vector2D{ position_.getX() ,(float)mngr_->getHeight() };
		else if (position_.getY() > mngr_->getHeight()) position_ = Vector2D{ position_.getX() , 0 };
	}
	
}
void FighterSystemOnline::onCollision_FighterAsteroid() //En caso de colisión con un asteroide
{
	for (int x = 0; x < nPlayers; ++x) {
		auto mP = mngr_->getPlayer(x);
		auto tr_ = mngr_->getComponent<Transform>(mP);
		auto& pos = tr_->getPos();
		Vector2D& v = tr_->getVel(); //Obtiene velocidad
		v = Vector2D(0, 0); //Velocidad nula
		switch (x)
		{
		case 0: pos = Vector2D(mngr_->getWidth() / 2 - 15, mngr_->getHeight() * 0.9); break;
		case 1: pos = Vector2D(mngr_->getWidth() / 2 - 15, mngr_->getHeight() * 0.1); break;
		case 2: pos = Vector2D(mngr_->getWidth() * 0.1, mngr_->getHeight() / 2 - 15); break;
		case 3: pos = Vector2D(mngr_->getWidth() * 0.9, mngr_->getHeight() / 2 - 15); break;
		default: pos = Vector2D(mngr_->getWidth() + x, mngr_->getHeight() + x); break; //A partir de 4 jugadores las posiciones son según su índice
		}
	}
}
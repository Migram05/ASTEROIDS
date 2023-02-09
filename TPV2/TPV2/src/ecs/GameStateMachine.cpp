#include "GameStateMachine.h"
GameStateMachine::GameStateMachine() {

}
void GameStateMachine::pushState(GameState* pState) //Añade un estado de juego
{
	secondary = stateStack.top(); //Asigna el estado actual al puntero secundario
	stateStack.push(pState);
	stateStack.top()->onEnter();
}
void GameStateMachine::popState() //Saca el estado de arriba
{
	if (!stateStack.empty()) //En caso de no estar vacía la pila
	{
		secondary = nullptr; 
		toDeleteList.push_back(stateStack.top());//Añade el estado actual a la pila de destrucción
		stateStack.pop();
	}
}
void GameStateMachine::changeState(GameState* pState) //Cambia el estado actual
{
	if (!stateStack.empty()) //En caso de no estar vacía
	{
		if (stateStack.top()->getStateID() == pState->getStateID()) //Si el estado añadido y el actual son iguales, no hace nada
		{
			return;
		}
		secondary = nullptr;
		toDeleteList.push_back(stateStack.top());
		stateStack.pop();
	}
	stateStack.push(pState); //Añade el estado y lo ejecuta
	stateStack.top()->onEnter();
}
GameState* GameStateMachine::currentState() { //Devuelve el estado actual
	return stateStack.top();
}
void GameStateMachine::update() //Update de la pila
{
	if (!stateStack.empty())
	{
		stateStack.top()->update();
	}
}
void GameStateMachine::render() //Render de la pila
{

	if (!stateStack.empty())
	{
		if (secondary != nullptr) secondary->render(); //Si el estado secundario existe lo renderiza por debajo del estado actual
		stateStack.top()->render();
	}
}
void GameStateMachine::clearList() { //Borra la lista de estados para borrar
	if (!toDeleteList.empty()) {
		for (GameState* g : toDeleteList) delete g;
		toDeleteList.clear();
	}
}
GameStateMachine::~GameStateMachine() { //Borra todos los estados de la pila
	while (!stateStack.empty()) {
		delete stateStack.top();
		stateStack.pop();
	}
}


// Miguel Ramírez Castrillo

#include <iostream>
#include "game/Game.h"
using namespace std;
int main(int ac, char **av) {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); //Control de basura
	Game* game = nullptr; //Crea el puntero
	try {
		game = new Game(); //Creación del juego
		game->Run(); //Bucle de juego
	}
	catch (string s) { cout << s << endl; }
	catch (exception& e) { //Muestra las excepciones por pantalla
		cout << e.what() << endl;
	}
	delete game; //Destruye game
	return 0;
}


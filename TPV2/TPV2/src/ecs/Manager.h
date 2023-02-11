#pragma once
#include <vector>
using namespace std;
class Entity;
class Manager
{
public:
	Manager();
	virtual ~Manager();
	Entity* addEntity();
	void refresh();
	void update();
	void render();
private:
	vector<Entity*> ents_;
};


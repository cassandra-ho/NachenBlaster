#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>
#include <vector>



// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
class Actor;
class NachenBlaster;
class Alien;

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetDir);
    void addO(double x, double y, std::string des);
	void addSA(std::string des);
	std::string addString();
	double eucDis(double x1, double y1, double x2, double y2);
	Alien* getCollidingAlien(Actor* o);
	NachenBlaster* getCollidingPlayer(Actor* o);
	Actor* getNB() const;
	void remove();
	void shipDied();

	
	virtual int init();
    virtual int move();
    virtual void cleanUp();
	~StudentWorld();
private:
	std::vector<Actor*> a;
	Actor* nb;
	int numShips;
	int deadShips;
	int remainShips;
	int totShipsToDestroy;
	double maxShips;
};

#endif // STUDENTWORLD_H_

/*– 
  - keeps track of the whole game world (the space field and all of its inhabitants such as
Smallgons, Snagglegons, the NachenBlaster, stars, goodies, projectiles, etc.). 

  - responsible for initializing the game world at the start of the game
  - asking all the actors to do something during each tick of the game
  - destroying an actor when it disappears (e.g an alien dies, an alien, 
    projectile or star flies off the screen, an explosion finishes exploding, 
    a projectile impacts a ship, etc.)
  - destroying *all* of the actors in the game world when the user loses a life.
*/

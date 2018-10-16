#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <vector>
#include <iostream>
#include<sstream>
#include <iomanip>
#include <cmath>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetDir)
	: GameWorld(assetDir), nb(nullptr){}





void StudentWorld::addO(double x, double y, string des) {
	Actor* temp;
	if (des == "ELGoodie") {
		temp = new ELGoodie(x, y, this);

	}
	else if (des == "RGoodie") {
		temp = new RGoodie(x, y, this);

	}
	else if (des == "FTGoodie") {
		temp = new FTGoodie(x, y, this);
	}
	else if (des == "Cabbage") {
		temp = new Cabbage(x, y, this);

	}
	else if (des == "Turnip") {
		temp = new Turnip(x, y, this);

	}
	else if (des == "NTorpedo") {
		temp = new Torpedo(x, y, 0, this);

	}
	else if (des == "ATorpedo") {
		temp = new Torpedo(x, y, 180, this);

	}
	else if (des == "Explosion") {
		temp = new Explosion(x, y, this);

	}
	a.push_back(temp);
}

void StudentWorld::addSA(string des) {
	if (des == "Alien") {
		int min;
		if (maxShips < remainShips)
			min = maxShips;
		else
			min = remainShips;
		if (numShips < min) {
			int s1 = 60;
			int s2 = 20 + (getLevel() * 5);
			int s3 = 5 + (getLevel() * 10);
			int s = s1 + s2 + s3;
			int ra = randInt(1, s);
			double y = randInt(0, VIEW_HEIGHT - 1);
			Actor* temp;
			if (ra < s1) {
				temp = new Smallgon(VIEW_WIDTH - 1, y, this);
				a.push_back(temp);
			}
			else if (ra < 60 + s2) {
				temp = new Smoregon(VIEW_WIDTH - 1, y, this);
				a.push_back(temp);
			}
			else {
				temp = new Snagglegon(VIEW_WIDTH - 1, y, this);
				a.push_back(temp);
			} //probabilities
			numShips++;
		} //adds ship if it needs to
	}
	else if (des == "Star") {
		int c = randInt(1, 15);
		if (c == 1) {
			double si;
			int sx;
			int sy;
			si = randInt(5, 50);
			sx = VIEW_WIDTH - 1;
			sy = randInt(0, VIEW_HEIGHT - 1);
			Actor* t = new Star(sx, sy, si / 100, this);
			a.push_back(t);
		} //1 in 15 chance, creates new star
	}
}


string StudentWorld::addString() {
	ostringstream s;
	string l = "Lives: ";
	string h = "  Health:";
	string sc = "  Score: ";
	string p = "%";
	string le = "  Level: ";
	string ca = "  Cabbages:";
	string t = "  Torpedoes: ";
	
	int hea = 100 * (static_cast<NachenBlaster *>(nb)->stats("Health") / 50.0);
	int cab = 100 * (static_cast<NachenBlaster *>(nb)->stats("Cabbage") / 30.0);
	int torp = static_cast<NachenBlaster *>(nb)->stats("Torpedo");
	s.fill(' ');
	s << l << getLives() << h << setw(4) << hea << p << sc << getScore()
		<< le << getLevel() << ca << setw(4) << cab << p << t << torp;
	string x = s.str();
	return x;
}


double StudentWorld::eucDis(double x1, double y1, double x2, double y2) {
	double x = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
	return x;
} //finds distance



Alien* StudentWorld::getCollidingAlien(Actor* o) {
	for (int i = 0; i < a.size(); i++) {
		if (a[i]->description() == "Smoregon" || a[i]->description() == "Smallgon" || a[i]->description() == "Snagglegon") {
			if (eucDis(o->getX(), o->getY(), a[i]->getX(), a[i]->getY())<(0.75*(o->getRadius() + a[i]->getRadius()))) {
				return static_cast<Alien*>(a[i]);
			} //checks distance between NB and aliens
		}
	}
	return nullptr;
}

NachenBlaster* StudentWorld::getCollidingPlayer(Actor * o) {
	if (eucDis(o->getX(), o->getY(), nb->getX(), nb->getY()) < (0.75*(o->getRadius() + nb->getRadius()))) {
		return static_cast<NachenBlaster*> (nb);
	} //checks distance between all other items and NB
	return nullptr;
}


Actor* StudentWorld::getNB() const {
	return nb;
} //return NB pointer


void StudentWorld::remove() {
	vector<Actor*>::iterator q = a.begin();
	while (q != a.end()) {
		if (!(*q)->isAlive()) {	
			if ((*q)->description() == "Smallgon" || (*q)->description() == "Smoregon" || (*q)->description() == "Snagglegon") {
				numShips--;
			} //if there is an alien, reduce the number of ships
			delete (*q);
			q = a.erase(q);
			continue;
		} //otherwise delete dead actors
		q++;
	}//loop through actors to find dead actors
}



void StudentWorld::shipDied() {
	deadShips++;
	remainShips = totShipsToDestroy - deadShips;
} //increases ships killed by player


int StudentWorld::init()
{
	nb = new NachenBlaster(this);
	double si;
	int sx;
	int sy;
	Actor* temp;
	for (int i = 0; i < 30; i++) {
		si = randInt(5, 50);
		sx = randInt(0, VIEW_WIDTH - 1);
		sy = randInt(0, VIEW_HEIGHT - 1);
		temp = new Star(sx, sy, si / 100, this);
		a.push_back(temp);
	} //adds 30 stars

	deadShips = 0;
	totShipsToDestroy = 6 + (4 * getLevel());
	maxShips = 4 + 0.5*getLevel();
	numShips = 0;
	remainShips = totShipsToDestroy - deadShips; //initializes values
	return GWSTATUS_CONTINUE_GAME;
}

//Your StudentWorld’s init() member function must :
//1. Initialize the data structures used to keep track of your game’s world.
//2. Allocate and insert a valid NachenBlaster object into the game world.
//3. Allocate and insert star objects into the game world.


// constructs a representation of the current level in your StudentWorld object and 
//populates it with initial objects(e.g., stars), using one or more data structures
//called by provided code either:
//(a) when the game first starts
//(b) when the player completes the current level and advances to a new level(that needs to be initialized),
//(c) when the user loses a life(but has more lives left) and the game is ready to restart at the current level.


int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
	if (nb->isAlive()) {
		nb->doSomething();
		setGameStatText(addString());
	}
	addSA("Alien");
	for (int i = 0; i < a.size(); i++) {
		if (a[i]->isAlive()) {
			a[i]->doSomething();
			if (!nb->isAlive()) {
				decLives();
				return GWSTATUS_PLAYER_DIED;
				
			}
			if (deadShips == totShipsToDestroy && nb->isAlive()) {
				return GWSTATUS_FINISHED_LEVEL;
			}
			setGameStatText(addString());
		}
	}
	remove();
	addSA("Star");
	setGameStatText(addString());
	return GWSTATUS_CONTINUE_GAME;
}
// Runs a single tick of the game
// responsible for asking each of the game actors (e.g., the NachenBlaster, each Smallgon, 
// goodie, projectile, star, explosion, etc.) to try to do something: e.g., move
// themselves and/or perform their specified behavior.
// responsible for disposing of(i.e., deleting) actors that need to disappear during a given tick

void StudentWorld::cleanUp()
{
	if (nb != nullptr) {
		nb->nowDead();
		delete nb;
		nb = nullptr;
	} //deletes NB and sets it to nullptr
	int x = a.size();
	for (int i = x; i > 0; i--) {
		delete a[i-1];
		a.pop_back();
	} //deletes all actors
	
}
// when the NachenBlaster completes the current level or loses a life
//(i.e., its hit points reach zero due to being shot by or colliding with the aliens)
//The cleanup() method is responsible for freeing all actors(e.g., all
//Smallgon objects, all star objects, all explosion objects, all projectiles, all goodie objects,
//the NachenBlaster object, etc.) that are currently in the game

StudentWorld::~StudentWorld() {
	cleanUp(); //calls cleanup
}
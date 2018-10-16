#include "Actor.h"
#include "StudentWorld.h"
#include <string>
using namespace std;
// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

//ACTOR
Actor::Actor(int iD, double x, double y, int dir, double size, int depth, StudentWorld* w):
	GraphObject(iD, x,y, dir, size, depth), m_alive(true), m_world(w) {}


string Actor::description() const {
	return m_descript;
}
//returns the name

void Actor::enemy(string en) {
	m_enemy = en;
} //sets enemy


StudentWorld* Actor::getWorld() const {
	return m_world;
}//returns world pointer


bool Actor::isAlive() const{
	return m_alive;
} //returns alive status

void Actor::nowDead() {
	m_alive = false;
} //the actor has died

bool Actor::offScreen() {
	double x = getX();
	double y = getY();
	if (x <= 0 ||x>=VIEW_WIDTH) {
		return true;
	}
	return false;
}
//checks if object is off the screen



string Actor::retEnemy() const {
	return m_enemy;
}

void Actor::setDescript(string descript) {
	m_descript = descript;
}
//sets the name


Actor::~Actor(){} //virtual destructor


//DAMAGEABLE OBJECT
DO::DO(int iD, double x, double y, int dir, double size, int depth, StudentWorld* w) : Actor(iD, x, y, dir, size, depth, w) {}

void DO::setHP(double hp) {
	m_hp = hp;
}

void DO::sufferDamage(string cause) {
	if (cause == "Cabbage" || cause == "Turnip") {
		setHP(retHP() - 2);
		getWorld()->playSound(SOUND_BLAST);
	}
	else if (cause == "Torpedo") {
		setHP(retHP() - 8);
		getWorld()->playSound(SOUND_BLAST);
	}
	else if (cause == "NachenBlaster") {
		setHP(0);
		getWorld()->playSound(SOUND_DEATH);
	}
	else if (cause == "Smallgon" || cause == "Smoregon") {
		setHP(retHP() - 5);
		getWorld()->playSound(SOUND_DEATH);
	}
	else if (cause == "Snagglegon") {
		setHP(retHP() - 15);
		getWorld()->playSound(SOUND_DEATH);
	}
}


double DO::retHP() const {
	return m_hp;
}

DO::~DO(){}

//ALIEN
Alien::Alien(int iD, double x, double y, int dir, int fpl, double ts, StudentWorld* w) : DO(iD, x, y, dir, 1.5, 1, w),
 m_fpl(fpl), m_ts(ts)
{
	if (description() == "Snagglegon") {
		setHP(10*(1+(getWorld()->getLevel())*0.1));
	}
	else {
		setHP(5 * (1 + (getWorld()->getLevel())*0.1));
	}
	enemy("NachenBlaster");
} //sets healthpoints based on the type of alien


void Alien::newFlightPlan() {
	double y = getY();
	if (m_fpl == 0 || y >= VIEW_HEIGHT - 1 || y <= 0) {
		if (y >= VIEW_HEIGHT - 1) {
			m_td = "dl";
		} //if about to go off top
		else if (y <= 0) {
			m_td = "ul";
		} //if about to go off bottom
		else if (m_fpl == 0) {
			string dir;
			int x = randInt(0, 2);
			switch (x) {
			case 0:
				dir = "l";
				break;
			case 1:
				dir = "ul";
				break;
			default:
				dir = "dl";
				break;
			} //chooses random direction if flight plan length is less than 0
			m_td = dir;
		}
		m_fpl = randInt(1, 32); //changes flight plan length
	}
} //changes flightplan for aliens other than snagglegons

void Alien::newTD(string td) {
	m_td = td;
} //sets travel direction

void Alien::fire() {
		getWorld()->addO(getX() - 14, getY(), "Turnip");
		getWorld()->playSound(SOUND_ALIEN_SHOOT);
	
} //fires with a certain probability and plays sound

void Alien::doSomething() {
	if (!isAlive())
		return;
	if (offScreen()) {
		nowDead();
		return;
	}
	NachenBlaster* c = getWorld()->getCollidingPlayer(this);
	if (c!=nullptr) {
		sufferDamage(c->description());
		c->sufferDamage(description());
	}
	newFlightPlan();
	double x = getX();
	double y = getY();
	if (getWorld()->getNB()->getX() < x && (getWorld()->getNB()->getY()-4) <= (y) 
		&& (getWorld()->getNB()->getY()+4) >= (y)) {
		if (description() == "Smallgon" || description() == "Smoregon") {
			int r = randInt(1, ((20 / getWorld()->getLevel()) + 5));
			if (r == 1) {
				fire(); //fires
				return;
			}
			if (description() == "Smoregon" && r == 2) {
				m_td = "l"; //resets travel direction
				m_fpl = VIEW_WIDTH; //changes flight plan length
				m_ts = 5; //updates speed
			}
		} //if to the right of NB and within 4 pixels of y of NB, then with a certain probability do something
		else {
			int r = randInt(1, ((15 / getWorld()->getLevel()) + 10));
			if (r == 1) {
				fire();
				return;
			}
		} //if to the right of NB and within 4 pixels of y of NB and a Snagglegon, then do something
		
	}
	if (m_td == "ul") {
		moveTo(x - m_ts, y + m_ts);
		m_fpl--;
	}
	else if (m_td == "dl") {
		moveTo(x - m_ts, y - m_ts);
		m_fpl--;
	}
	else if (m_td == "l") {
		moveTo(x - m_ts, y);
		m_fpl--;
	} //moves the alien
	c = getWorld()->getCollidingPlayer(this);
	if (c != nullptr) {
		sufferDamage(c->description());
		c->sufferDamage(description());
	}
}

void Alien::sufferDamage(string cause) {
	DO::sufferDamage(cause);
	if (retHP() <= 0) {
		getWorld()->addO(getX(), getY(), "Explosion");
		nowDead();
		extra();
		getWorld()->shipDied();
	}
}

void Alien::extra() {
	getWorld()->increaseScore(250);
}

Alien::~Alien(){}


//SMALLGON
Smallgon::Smallgon(double x, double y, StudentWorld* w) :Alien(IID_SMALLGON, x, y, 0, 0, 2.0, w) {
	string temp = "Smallgon";
	setDescript(temp);
}

Smallgon::~Smallgon(){}

//SMOREGON
Smoregon::Smoregon(double x, double y, StudentWorld* w) : Alien(IID_SMOREGON, x, y, 0, 0, 2.0, w) {
	string temp = "Smoregon";
	setDescript(temp);
}

void Smoregon::extra() {
	getWorld()->increaseScore(250);
	int r = randInt(1, 3);
	if (r == 1) {
		int c = randInt(1, 2);
		if (c == 1) {
			getWorld()->addO(getX(), getY(), "RGoodie");
		}
		else
			getWorld()->addO(getX(), getY(), "FTGoodie");
	}
}

Smoregon::~Smoregon(){}

//SNAGGLEGON
Snagglegon::Snagglegon(double x, double y, StudentWorld* w) :Alien(IID_SNAGGLEGON, x, y, 0, 0, 1.75, w) {
	string temp = "Snagglegon";
	setDescript(temp);
	newTD("dl");
} //sets travel direction initially

void Snagglegon::newFlightPlan() {
	double y = getY();
	if (y >= VIEW_HEIGHT - 1) {
		newTD("dl");
	}
	else if (y <= 0) {
		newTD("ul");
	}
} //Snagglegon's flight plan

void Snagglegon::fire() {
	getWorld()->addO(getX() - 14, getY(), "ATorpedo");
	getWorld()->playSound(SOUND_TORPEDO);
} //fire

void Snagglegon::extra() {
	getWorld()->increaseScore(1000);
	int r = randInt(1, 6);
	if (r == 1) {
		getWorld()->addO(getX(), getY(), "ELGoodie");
	}
}

Snagglegon::~Snagglegon(){}


//NACHENBLASTER
NachenBlaster::NachenBlaster(StudentWorld* w) :DO(IID_NACHENBLASTER, 0, 128, 0, 1.0, 0, w),
m_cabEP(30), m_torp(0) {
	string temp = "NachenBlaster";
	setDescript(temp);
	enemy("Alien");
	setHP(50);
}

void NachenBlaster::inc(int amt, string cause) {
	if (cause == "RGoodie") {
		if (retHP() + amt >= 50)
			setHP(50);
		else
			setHP(retHP() + amt);
	}
	else if (cause == "ELGoodie") {
		getWorld()->incLives();
	}
	else if (cause == "FTGoodie") {
		m_torp += 5;
	}
}

bool NachenBlaster::decTorp() {
	if (m_torp > 0) {
		m_torp--;
		return true;
	}
	else
		return false;
}
//reduces number of torpedos if possible, otherwise return false



void NachenBlaster::doSomething() {
	if (!isAlive())
		return;
	int ch;
	double x = getX();
	double y = getY();
	if (getWorld()->getKey(ch)) {
		switch (ch) {
		case KEY_PRESS_LEFT:
			if ((x - 6) >= 0 && (x) < VIEW_WIDTH) {
				moveTo(x - 6, y);
			}
			break; //moves appropriately
		case KEY_PRESS_RIGHT:
			if ((x) >= 0 && (x + 6) < VIEW_WIDTH) {
				moveTo(x + 6, y);
			}
			break; //moves appropriately
		case KEY_PRESS_DOWN:
			if ((y - 6) >= 0 && (y) < VIEW_HEIGHT) {
				moveTo(x, y - 6);
			}
			break; //moves appropriately
		case KEY_PRESS_UP:
			if ((y) >= 0 && (y + 6) < VIEW_HEIGHT) {
				moveTo(x, y + 6);
			}
			break; //moves appropriately
		case KEY_PRESS_SPACE:
			if (m_cabEP >= 5) {
				m_cabEP -= 5;
				string temp = "Cabbage";
				getWorld()->addO(x + 12, y, temp);
				getWorld()->playSound(SOUND_PLAYER_SHOOT);
			}
			break;
			//fire cabbage
			//play sound
		case KEY_PRESS_TAB:
			if (decTorp()) {
				string temp = "NTorpedo";
				getWorld()->addO(x + 12, y, temp);
				getWorld()->playSound(SOUND_PLAYER_SHOOT);
				//fire Torpedo
				//play sound
			}
			break;
		default:
			break;
		}
	}
	Alien* c = getWorld()->getCollidingAlien(this);
	if (c != nullptr) {
		sufferDamage(c->description());
		c->sufferDamage(description());
	}
	if (m_cabEP + 1 >= 30)
		m_cabEP = 30;
	else
		m_cabEP += 1;
}
//key change and cabbage reupload

int NachenBlaster::stats(std::string des) const {
	if (des == "Cabbage")
		return m_cabEP;
	else if (des == "Torpedo")
		return m_torp;
	else if (des == "Health")
		return retHP();
	return -1;
}

void NachenBlaster::sufferDamage(string cause) {
	DO::sufferDamage(cause);
	if (retHP() <= 0) {
		nowDead();
	}
}

NachenBlaster::~NachenBlaster() {}

//EXPLOSION
Explosion::Explosion(double x, double y, StudentWorld* w): Actor(IID_EXPLOSION, x, y, 0, 1, 0, w), m_ticks(0){
	string temp = "Explosion";
	setDescript(temp);
}


void Explosion::doSomething() {
	double x = getSize();
	if (m_ticks == 0) {
		m_ticks++;
		return;
	} //keeps size at 1 for first tick
	else if (m_ticks < 4) {
		m_ticks++;
		x *= 1.5;
		setSize(x);
	} //ticks 2-4 increase size
	else {
		setSize(0); //removes explosion from screen
		nowDead(); //explosion dies
	}
}

Explosion::~Explosion(){}


//GOODIE
Goodie::Goodie(int iD, double x, double y, StudentWorld* w) : Actor(iD, x, y, 0, 0.5, 1, w) {}

void Goodie::collide() {
	getWorld()->increaseScore(100);
	nowDead();
	getWorld()->playSound(SOUND_GOODIE);
}

Goodie::~Goodie(){}

//ELGOODIE
ELGoodie::ELGoodie(double x, double y, StudentWorld* w) : Goodie(IID_LIFE_GOODIE, x, y, w) {
	string temp = "ELGoodie";
	setDescript(temp);
}

void ELGoodie::doSomething() {
	if (!isAlive())
		return;
	if (offScreen()) {
		nowDead();
		return;
	}
	NachenBlaster* t = getWorld()->getCollidingPlayer(this);
	if (t != nullptr) {
		Goodie::collide();
		t->inc(0, description());
		return;
	}
	moveTo(getX() - 0.75, getY() - 0.75);
	t = getWorld()->getCollidingPlayer(this);
	if (t != nullptr) {
		Goodie::collide();
		t->inc(0, description());
		return;
	}
}

ELGoodie::~ELGoodie(){}

//RGoodie

RGoodie::RGoodie(double x, double y, StudentWorld* w) : Goodie(IID_REPAIR_GOODIE, x, y, w){
	string temp = "RGoodie";
	setDescript(temp);
}

void RGoodie::doSomething() {
	if (!isAlive())
		return;
	if (offScreen()) {
		nowDead();
		return;
	}
	NachenBlaster* t = getWorld()->getCollidingPlayer(this);
	if (t != nullptr) {
		Goodie::collide();
		t->inc(10, description());
		return;
	}
	moveTo(getX() - 0.75, getY() - 0.75);
	t = getWorld()->getCollidingPlayer(this);
	if (t != nullptr) {
		Goodie::collide();
		t->inc(10, description());
		return;
	}
}

RGoodie::~RGoodie(){}

//FTGoodie

FTGoodie::FTGoodie(double x, double y, StudentWorld* w): Goodie(IID_TORPEDO_GOODIE, x, y, w){
	string temp = "FTGoodie";
	setDescript(temp);
}

void FTGoodie::doSomething() {
	if (!isAlive())
		return;
	if (offScreen()) {
		nowDead();
		return;
	}
	NachenBlaster* t = getWorld()->getCollidingPlayer(this);
	if (t != nullptr) {
		Goodie::collide();
		t->inc(0, description());
		return;
	}
	moveTo(getX() - 0.75, getY() - 0.75);
	t = getWorld()->getCollidingPlayer(this);
	if (t != nullptr) {
		Goodie::collide();
		t->inc(0, description());
		return;
	}
}

FTGoodie::~FTGoodie(){}




//PROJECTILE
Projectile::Projectile(int iD, double x, double y, int dir, StudentWorld* w):Actor(iD, x, y, dir, .5, 1, w) {}



void Projectile::doSomething() {
	if (!isAlive()) {
		return;
	}
	if (offScreen()) {
		nowDead();
		return;
	}
 	if(col())
		return;
	move();
	rotate();
	if(col())
		return;
}

bool Projectile::col() {
	if (retEnemy() == "Alien") {
		Alien* t = getWorld()->getCollidingAlien(this);
		if (t != nullptr) {
			t->sufferDamage(description());
			nowDead();
			return true;
		}
	}
	else if (retEnemy() == "NachenBlaster") {
		NachenBlaster* n = getWorld()->getCollidingPlayer(this);
		if (n != nullptr) {
			n->sufferDamage(description());
			nowDead();
			return true;
		}
	}
	return false;
}


void Projectile::rotate() {
	int dir = getDirection();
	setDirection(dir + 20);
} //rotates

Projectile::~Projectile(){}

//Cabbage
Cabbage::Cabbage(double x, double y, StudentWorld* w):Projectile(IID_CABBAGE, x, y, 0, w){
	string temp = "Cabbage";
	setDescript(temp);
	enemy("Alien");
}


void Cabbage::move() {
	double x = getX();
	double y = getY();
	moveTo(x + 8, y);
} //moves it appropriately

Cabbage::~Cabbage() {}


//TORPEDO
Torpedo::Torpedo(double x, double y, int dir, StudentWorld* w) : Projectile(IID_TORPEDO, x, y, dir, w) {
	string temp = "Torpedo";
	setDescript(temp);
	if (dir == 0)
		enemy("Alien");
	else
		enemy("NachenBlaster");
}



void Torpedo::move() {
	double x = getX();
	double y = getY();
	if (getDirection() == 0) {
		moveTo(x + 8, y);
	}
	else if (getDirection() == 180) {
		moveTo(x - 8, y);
	}
	
} //moves it appropriately

void Torpedo::rotate() {
	return;
}

Torpedo::~Torpedo(){}

//TURNIP
Turnip::Turnip(double x, double y, StudentWorld* w) :Projectile(IID_TURNIP, x, y, 0, w) {
	string temp = "Turnip";
	setDescript(temp);
	enemy("NachenBlaster");
}



void Turnip::move() {
	double x = getX();
	double y = getY();
	moveTo(x - 6, y);
} //moves it appropriately

Turnip::~Turnip() {}




//STAR
Star::Star(double x, double y, double size, StudentWorld* w): Actor(IID_STAR, x, y, 0, size, 3, w){
	string temp = "Star";
	setDescript(temp);
}

void Star::doSomething() {
	double x = getX();
	double y = getY();
	moveTo(x - 1, y); //moves one pixel left
	if (getX() <= -1) {
		nowDead(); //if it is off the screen, it is now dead
	}
}

Star::~Star(){}












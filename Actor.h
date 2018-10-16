#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "GameConstants.h"
#include <string>
class StudentWorld;

class Actor: public GraphObject {
public:
	Actor(int iD, double x, double y, int dir, double size, int depth, StudentWorld* w);
	std::string description() const;
	virtual void doSomething() = 0;
	void enemy(std::string en);
	StudentWorld* getWorld() const;
	bool isAlive() const; 
	void nowDead();
	bool offScreen();
	std::string retEnemy() const;
	void setDescript(std::string descript);
	virtual ~Actor();
private:
	bool m_alive;
	std::string m_enemy;
	std::string m_descript;
	StudentWorld* m_world;

};

class DO : public Actor {
public:
	DO(int iD, double x, double y, int dir, double size, int depth, StudentWorld* w);
	virtual void sufferDamage(std::string cause);
	double retHP() const;
	void setHP(double hp);
	virtual ~DO();
private:
	double m_hp;
};

//ALIEN
class Alien : public DO {
public:
	Alien(int iD, double x, double y, int dir, int fpl, double ts, StudentWorld* w);
	virtual void doSomething();
	virtual void fire();
	virtual void newFlightPlan();
	void newTD(std::string td);
	virtual void sufferDamage(std::string cause);
	virtual void extra();
	virtual ~Alien();
private:
	double m_ts;
	int m_fpl;
	std::string m_td;
};

//SMALLGON
class Smallgon : public Alien {
public:
	Smallgon(double x, double y, StudentWorld* w);
	virtual ~Smallgon();
};

//SMOREGON
class Smoregon : public Alien {
public:
	Smoregon(double x, double y, StudentWorld* w);
	virtual void extra();
	virtual ~Smoregon();
};

//SNAGGLEGON
class Snagglegon :public Alien {
public:
	Snagglegon(double x, double y, StudentWorld* w);
	virtual void newFlightPlan();
	virtual void fire();
	virtual void extra();
	virtual ~Snagglegon();

};

//NACHENBLASTER
class NachenBlaster : public DO {
public:
	NachenBlaster(StudentWorld* w);
	bool decTorp();
	virtual void doSomething();
	int stats(std::string des) const;
	void inc(int amt, std::string cause);
	virtual void sufferDamage(std::string cause);
	virtual ~NachenBlaster();
private:
	int m_cabEP;
	int m_torp;
};


class Explosion : public Actor {
public:
	Explosion(double x, double y, StudentWorld* w);
	virtual void doSomething();
	virtual ~Explosion();
private:
	int m_ticks;
};



//GOODIE
class Goodie : public Actor {
public:
	Goodie(int iD, double x, double y, StudentWorld* w);
	void collide();
	virtual ~Goodie();
};

//ELGOODIE
class ELGoodie : public Goodie {
public:
	ELGoodie(double x, double y, StudentWorld* w);
	virtual void doSomething();
	virtual ~ELGoodie();
};

//RGOODIE
class RGoodie : public Goodie {
public:
	RGoodie(double x, double y, StudentWorld* w);
	virtual void doSomething();
	virtual ~RGoodie();
};
//FTGOODIE
class FTGoodie :public Goodie {
public:
	FTGoodie(double x, double y, StudentWorld* w);
	virtual void doSomething();
	virtual ~FTGoodie();
};



//PROJECTILE
class Projectile : public Actor {
public:
	Projectile(int iD, double x, double y, int dir, StudentWorld* w);
	virtual void doSomething();
	virtual void move() = 0;
	bool col();
	virtual void rotate();
	virtual ~Projectile();
};

//CABBAGE
class Cabbage : public Projectile {
public:
	Cabbage(double x, double y, StudentWorld* w);
	virtual void move();
	virtual ~Cabbage();
};

//TORPEDO
class Torpedo : public Projectile {
public:
	Torpedo(double x, double y, int dir, StudentWorld* w);
	virtual void move();
	virtual void rotate();
	virtual ~Torpedo();
};

//TURNIP
class Turnip : public Projectile {
public:
	Turnip(double x, double y, StudentWorld* w);
	virtual void move();
	virtual ~Turnip();
};



//STAR
class Star : public Actor {
public:
	Star(double x, double y, double size, StudentWorld* w);
	virtual void doSomething();
	~Star();
};





#endif // ACTOR_H_

#pragma once
#include "npc.h"
#include <string>
using namespace std;

class Character
{
protected:
    int hp;
    int maxHp;
    int move;
    int dex;
    int str;
    int wits;
	int toughness;
    int dmg;

    string name;

public:
    Character(const string& name, int hp, int stamina, int move, int dex, int str, int wits, int dmg);

    virtual ~Character() = default;

	virtual void attack(Character& target);

    void takeDamage(int amount);

    bool isAlive() const;

    int getHp() const;

    string getName() const;
};

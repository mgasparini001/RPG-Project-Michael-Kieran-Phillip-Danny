#pragma once
#include <string>

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

    std::string name;

public:
    Character(const std::string& name, int hp, int move, int dex, int str, int wits, int toughness, int dmg);

    virtual ~Character() = default;

    virtual void attack(Character& target);

    void takeDamage(int amount);

    bool isAlive() const;

    int getHp() const;

    std::string getName() const;
	
};

#pragma once
#include <string>

// Base Character class
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
	// constructor
    Character(const std::string& name, int hp, int move, int dex, int str, int wits, int toughness, int dmg);

	// virtual destructor (virtual for cleanup in derived classes)
    virtual ~Character() = default;

	// basic attack function (virtual so derived classes can override if neccessary)
    virtual void attack(Character& target);

	// random dice roll function for various uses
	int diceRoll(int x);

	// function to reduce hp when taking damage
    void takeDamage(int amount);

	// check if character is alive
    bool isAlive() const;

	// get current hp
    int getHp() const;
	
	// get character name
    std::string getName() const;

};

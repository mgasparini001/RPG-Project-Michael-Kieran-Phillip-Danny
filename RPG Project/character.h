#pragma once
#include <string>
#include <iostream>
// Base Character class
class Character
{
protected:
    int Armor;
    int HP;
    int Melee;
    int Range;
    int AP; 
    int Dmg;
    int Stamina;
    

    std::string Name;

public:
	// constructor
    Character(const std::string& name, int hp, int melee, int range, int armor, int stamina, int dmg, int ap);

	// virtual destructor (virtual for cleanup in derived classes)
    virtual ~Character() = default;

    virtual void attackMessage(Character& target) = 0;

	// basic attack function (virtual so derived classes can override if neccessary)
    void attack(Character& target, bool attackType);

	// random dice roll function for various uses
	int diceRoll(int x);

	// function to reduce hp when taking damage
    void takeDamage(int amount);

    int getArmor();

	// check if character is alive
    bool isAlive() const;

	// get current hp
    int getHp() const;

    int getDmg();

    int getAp();

    void rest();

    bool flee();

    int getStamina();
	
	// get character name
    std::string getName() const;


};

#include "character.h"
#include <string>
#include <iostream>
using std :: string;
using std::cout;
using std::endl;

// character constructor
Character::Character(const string& name, int hp, int melee, int range, int armor, int stamina, int dmg, int ap) {
    Name = name;
    Armor=armor;
    HP=hp;
    Melee=melee;
    Range=range;
    AP=ap;
    Dmg=dmg;
    Stamina=stamina;
}

//armor getter
int Character::getArmor() {
    return Armor;
}
//input: Address of character object and boolean represnting attack type, Output: void, does an attack
void Character::attack(Character& target, bool attackType){
    // attackType
    if (attackType) {
        if (Stamina > 100) {
            Stamina -= 100;
            //melee attack
            if (diceRoll(Melee) + AP > +target.getArmor()) {//sees if attack will pierce target's armor (character's skill for AP and dmg)
                attackMessage(target);
                target.takeDamage(diceRoll(Melee) + Dmg);
            }
        }
        else {
            std::cout << Name << "is too exhuasted to make the attack!";
        }
    }
    else {
        //ranged attack
        if (Stamina > 100) {
            Stamina -= 100;
            if (diceRoll(AP) > +target.getArmor()) {//sees if attack will pierce target's armor (character's skill for dmg, AP for AP)
                attackMessage(target);
                target.takeDamage(diceRoll(Melee) + Dmg);
            }
            else
            {
                std::cout << "The attack missed!" << std::endl;
            }
        }
        else
        {
            std::cout << Name << "is too exhuasted to make the attack!";
        }
    }
}



//Input: int represnting the sided die that is to be rolled, Output: int, Rolls a dice
int Character::diceRoll(int x){
    return (rand() % x) + 1;
}

//Input: int representing a specfic amount of damage to be taken, Output: void, reduces hp by the given amount
void Character::takeDamage(int amount){
    HP -= amount;
    if (HP < 0){
        HP = 0;
    }
}

//Output: bool, check to see if the character is alive
bool Character::isAlive() const{
    return HP > 0;
}

//hp getter
int Character::getHp() const{
    return HP;
}

// get character name
string Character::getName() const{
    return Name;
}

//damage getter
int Character::getDmg()
{
    return Dmg;
}

//armor penitration getter
int Character::getAp()
{
    return AP;
}

//stamina getter
int Character::getStamina()
{
    return Stamina;
}

//Output: void, restores some of the characters stamina
void Character::rest()
{
    Stamina += Stamina / 5;
}

//Output: bool, function where character tries to flee, returns true if succesful, false otherwise
bool Character::flee()
{
    cout << Name << " attempts to flee from combat!" << endl;

    // roll a 20 side dice to see if they succeed
    int fleeRoll = diceRoll(20);

    if (fleeRoll >= 15) {
        cout << getName() << " successfully escaped!" << endl;
        return true;
    }
    else {
        cout << getName() << " failed to escape!" << endl;
        return false;
    }
}


void Character::setHp(int hp) {
    HP = hp;
}

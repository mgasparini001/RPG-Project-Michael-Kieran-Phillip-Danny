#include "character.h"
#include <string>
#include <iostream>
using std :: string;
using std::cout;

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

int Character::getArmor() {
    return Armor;
}
// basic attack function
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
        }
        else
        {
            std::cout << Name << "is too exhuasted to make the attack!";
        }
    }
}



// random dice roll function for various uses
int Character::diceRoll(int x){
    return (rand() % x) + 1;
}

// function to reduce hp when taking damage
void Character::takeDamage(int amount){
    HP -= amount;
    if (HP < 0){
        HP = 0;
    }
}

// check if character is alive
bool Character::isAlive() const{
    return HP > 0;
}

// get current hp
int Character::getHp() const{
    return HP;
}

// get character name
string Character::getName() const{
    return Name;
}

int Character::getDmg()
{
    return Dmg;
}

int Character::getAp()
{
    return AP;
}

int Character::getStamina()
{
    return Stamina;
}


void Character::rest()
{
    Stamina += Stamina / 5;
}

bool Character::flee()
{
    cout << getName() << " attempts to flee from combat!" << endl;

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

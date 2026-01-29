#include "character.h"
#include <string>
#include <iostream>
using std :: string;

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
                int damage = diceRoll(Melee) + Dmg;
                std::cout << Name << " hits " << target.getName() << " for " << damage << " damage!";
                target.takeDamage(damage);
            }
        }
        else {
            
        }
    }
    else {
        //ranged attack
        if (Stamina > 100) {
            Stamina -= 100;
            if (diceRoll(AP) > +target.getArmor()) {//sees if attack will pierce target's armor (character's skill for dmg, AP for AP)
                int damage = diceRoll(Range);
                std::cout << Name << " shoots " << target.getName() << " for " << damage << " damage!";
                target.takeDamage(damage);
            }
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

void Character::rest()
{
    Stamina += Stamina / 5;
}

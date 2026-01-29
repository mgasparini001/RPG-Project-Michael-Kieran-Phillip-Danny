#include "character.h"
#include <string>
using std::string;

// character constructor
Character::Character(const string& name, int hp, int melee, int range, int armor, int stamina, int dmg, int ap) {
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
    int damage = 0;

    // attackType
    if (attackType) {
        if (stamina > 100) {
            stamina -= 100;
            //melee attack
            if (diceRoll(melee) + AP > +target.getArmor()) {//sees if attack will pierce target's armor (character's skill for AP and dmg)
                target.takeDamage(diceRoll(melee) + dmg);
            }
        }
        else {
            
        }
    }
    else {
        //ranged attack
        if (stamina > 100) {
            stamina -= 100;
            if (diceRoll(AP) > +target.getArmor()) {//sees if attack will pierce target's armor (character's skill for dmg, AP for AP)
                target.takeDamage(diceRoll(range));
            }
        }
    }
}

void rest(){}

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
    return name;
}
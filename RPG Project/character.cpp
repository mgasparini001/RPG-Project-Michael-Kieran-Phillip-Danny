#include "character.h"
#include <string>
using std::string;

// character constructor
Character::Character(const string& name, int HP, int melee, int range, int Armor)
    : name(name), HP(HP), melee(melee), range(range), Armor(Armor) {}

// basic attack function
void Character::attack(Character& target, bool attackType){
    int damage = 0;
    
    // attackType
    if (attackType) {
        //melee attack
    } else {
        //ranged attack
    }
    
    // Apply damage to target
    target.takeDamage(damage);
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
    return name;
}
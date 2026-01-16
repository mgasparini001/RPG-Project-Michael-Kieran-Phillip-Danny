#include "character.h"
#include <string>
using std::string;

// character constructor
Character::Character(const string& name, int hp, int move, int dex, int str, int wits, int toughness, int dmg) : name(name), hp(hp), maxHp(hp), move(move), dex(dex), str(str), wits(wits), toughness(toughness), dmg(dmg) {}

// basic attack function
void Character::attack(Character& target){
    target.takeDamage(dmg);
}

// random dice roll function for various uses
int Character::diceRoll(int x){
    return (rand() % x) + 1;
}

// function to reduce hp when taking damage
void Character::takeDamage(int amount){
    hp -= amount;
    if (hp < 0){
        hp = 0;
    }
}

// check if character is alive
bool Character::isAlive() const{
    return hp > 0;
}

// get current hp
int Character::getHp() const{
    return hp;
}

// get character name
string Character::getName() const{
    return name;
}

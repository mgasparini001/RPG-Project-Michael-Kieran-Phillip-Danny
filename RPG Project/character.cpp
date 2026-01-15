#include "character.h"
#include <string>
using std::string;

Character::Character(const string& name, int hp, int move, int dex, int str, int wits, int toughness, int dmg) : name(name), hp(hp), maxHp(hp), move(move), dex(dex), str(str), wits(wits), toughness(toughness), dmg(dmg) {}

void Character::attack(Character& target){
    target.takeDamage(dmg);
}

void Character::takeDamage(int amount){
    hp -= amount;
    if (hp < 0){
        hp = 0;
    }
}

bool Character::isAlive() const{
    return hp > 0;
}

int Character::getHp() const{
    return hp;
}

string Character::getName() const{
    return name;
}

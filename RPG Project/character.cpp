#include "character.h"
#include <string>
#include <iostream>
#include "ItemRegistry.h"
#include "player.h"
#include "inventory.h"
using std :: string;
using std::cout;
using std::endl;


// character constructor
Character::Character(const string& name, int hp, int melee, int range, int armor, int stamina, int dmg, int ap, bool itemEquipped) {
    Name = name;
    Armor=armor;
    HP=hp;
    Melee=melee;
    Range=range;
    AP=ap;
    Dmg=dmg;
    Stamina=stamina;
    hasItemEquipped = itemEquipped;
}

//armor getter
int Character::getArmor() {
    return Armor;
}
//input: Address of character object and boolean represnting attack type, Output: void, does an attack
bool Character::attack(Character& target, bool attackType){
    // attackType
    if (attackType) {
        if (Stamina > 100) {
            Stamina -= 100;
            //melee attack
            if (diceRoll(Melee) + AP > +target.getArmor()) {//sees if attack will pierce target's armor (character's skill for AP and dmg)
                attackMessage(target);
                target.takeDamage(diceRoll(Melee) + Dmg);
                return true;
            }
            else
            {
                std::cout << "Qwaping, the attack did nothing";
                return false;
            }
        }
        else {
            std::cout << Name << "is too exhuasted to make the attack!";
            return false;
        }
    }
    else {
        //ranged attack
        if (Stamina > 50) {
            Stamina -= 50;
            if (diceRoll(AP) > +target.getArmor()) {//sees if attack will pierce target's armor (character's skill for dmg, AP for AP)
                attackMessage(target);
                target.takeDamage(diceRoll(Range) + Dmg);
                return true;
            }
            else
            {
                std::cout << "The attack missed!" << std::endl;
                return false;
            }
        }
        else
        {
            std::cout << Name << "is too exhuasted to make the attack!";
            return false;
        }
    }
}



//Input: int represnting the sided die that is to be rolled, Output: int, Rolls a dice
int Character::diceRoll(int x)
{
    return (rand() % x) + 1;
}

//Input: int representing a specfic amount of damage to be taken, Output: void, reduces hp by the given amount
void Character::takeDamage(int amount)
{
    HP -= amount;
    if (HP < 0){
        HP = 0;
    }
}

//Output: bool, check to see if the character is alive
bool Character::isAlive() const
{
    return HP > 0;
}

//hp getter
int Character::getHp() const
{
    return HP;
}

// get character name
string Character::getName() const
{
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
    std::cout << Name << " attempts to flee from combat!" << std::endl;

    // roll a 20 side dice to see if they succeed
    int fleeRoll = diceRoll(20);

    if (fleeRoll >= 15) {
        std::cout << getName() << " successfully escaped!" << std::endl;
        return true;
    }
    else {
        std::cout << getName() << " failed to escape!" << std::endl;
        return false;
    }
}


void Character::setHp(int hp) {
    HP = hp;
}

void Character::setStamina(int stamina) {
    Stamina = stamina;
}

// Inventory management implementation
Inventory& Character::getInventory() {
    return inventory;
}

void Character::addItemToInventory(int itemID, ItemRegistry& registry, int quantity) {
    std::shared_ptr<Item> item = registry.getItem(itemID);
    if (!item)
    {
        return;
    }
    inventory.addItem(item, quantity);
}

bool Character::removeItemFromInventory(int itemID, Character &p, int quantity) {
    return inventory.removeItem(itemID, p, quantity);
}

int Character::getItemQuantity(int itemID) const {
    return inventory.getQuantity(itemID);
}

bool Character::hasItem(int itemID) const {
    return inventory.hasItem(itemID);
}

bool Character::getHasItemEquipped()
{
    return hasItemEquipped;
}

void Character::unequipItem()
{
    //find equipped item
    InventoryNode* current = inventory.getHead();
    while (current != nullptr && current->isEquipped == false)
    {
        //std::cout << registry.getItemName(current->itemID) << " x" << current->quantity << "\n";
        current = current->next;
    }
    if (current == nullptr || !current->item)
    {
        return;
    }
    const int itemId = current->item->getId();
    current->isEquipped = false;
    hasItemEquipped = false;

    switch (itemId)
    {
    case 0:
        AP -= 3;
        break;
    case 2:
        Armor -= 5;
        break;
    case 3:
        Dmg -= 2;
        HP -= 5;
        Stamina -= 100;
        break;
    case 4:
        Dmg -= 2;
        break;
    case 5:
        Armor -= 2;
        break;
    case 6:
        Armor -= 3;
        break;
    case 10:
        Dmg -= 6;
        AP -= 4;
        break;
    case 11:
        Armor -= 6;
        break;
    case 12:
        Dmg -= 4;
        AP -= 3;
        Stamina -= 100;
        break;
    case 13:
        Dmg -= 3;
        AP -= 3;
        Armor -= 3;
        HP -= 10;
        Stamina -= 100;
        break;
    case 17:
        Dmg -= 8;
        AP -= 5;
        break;
    case 18:
        Armor -= 9;
        break;
    case 19:
        Armor -= 3;
        break;
    case 21:
        Dmg -= 5;
        AP -= 5;
        Armor -= 3;
        break;
    case 24:
        Dmg -= 10;
        AP -= 6;
        break;
    case 25:
        Dmg -= 11;
        AP -= 4;
        HP -= 10;
        break;
    case 26:
        Armor -= 12;
        break;
    case 27:
        Armor -= 6;
        break;
    case 28:
        Armor -= 5;
        Stamina -= 200;
        break;
    case 32:
        Dmg -= 2;
        AP -= 2;
        Stamina -= 300;
        break;
    default:
        break;
    }
   
}
bool Character::equipItem(int equippedItemID, bool inBattle)
{
    
    InventoryNode* current = inventory.getHead();

    while (current != nullptr && (!current->item || current->item->getId() != equippedItemID))
    {
        current = current->next;
    }
   // checks for if you have the item before equipping it
    if (current == nullptr)
    {
        return false;
    }

    if (hasItemEquipped)
    {
        unequipItem();
    }
   

    const int itemId = current->item ? current->item->getId() : -1;
    if (itemId < 0 || (current->item->isConsumable() && !inBattle))
    {
        return false;
    }

    current->isEquipped = true;
    hasItemEquipped = true;

    switch (itemId)
    {
    case 0:
        AP += 3;
        break;
    case 1:
        HP += 12;
        break;
    case 2:
        Armor += 5;
        break;
    case 3:
        Dmg += 2;
        HP += 5;
        Stamina += 100;
        break;
    case 4:
        Dmg += 2;
        break;
    case 5:
        Armor += 2;
        break;
    case 6:
        Armor += 3;
        break;
    case 7:
        HP += 5;
        Stamina += 500;
        break;
    case 8:
        HP += 5;
        Stamina += 50;
        break;
    case 9:
        HP += 3;
        break;
    case 10:
        Dmg += 6;
        AP += 4;
        break;
    case 11:
        Armor += 6;
        break;
    case 12:
        Dmg += 4;
        AP += 3;
        Stamina += 100;
        break;
    case 13:
        Dmg += 3;
        AP += 3;
        Armor += 3;
        HP += 10;
        Stamina += 100;
        break;
    case 14:
        HP += 15;
        break;
    case 15:
        HP += 10;
        break;
    case 16:
        HP += 20;
        break;
    case 17:
        Dmg += 8;
        AP += 5;
        break;
    case 18:
        Armor += 9;
        break;
    case 19:
        Armor += 3;
        break;
    case 21:
        Dmg += 5;
        AP += 5;
        Armor += 3;
        break;
    case 22:
        HP += 15;
        break;
    case 23:
        HP += 10;
        break;
    case 24:
        Dmg += 10;
        AP += 6;
        break;
    case 25:
        Dmg += 11;
        AP += 4;
        HP += 10;
        break;
    case 26:
        Armor += 12;
        break;
    case 27:
        Armor += 6;
        break;
    case 28:
        Armor += 5;
        Stamina += 200;
        break;
    case 29:
        HP += 5;
        break;
    case 30:
        HP += 15;
        break;
    case 31:
        HP += 50;
        Stamina += 300;
        break;
    case 32:
        Dmg += 2;
        AP += 2;
        Stamina += 300;
        break;
    default:
        break;
    }
    return true;
   
}

int Character::getEquippedItemID()
{
    InventoryNode* current = inventory.getHead();
    while (current != nullptr && current->isEquipped == false)
    {
        //std::cout << registry.getItemName(current->itemID) << " x" << current->quantity << "\n";
        current = current->next;
    }

    if (current == nullptr || !current->item)
    {
        return -1;
    }
    return current->item->getId();
}

void Character::heal()
{
    HP += 10;
}

    


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
    //unequip
    int itemId = current->item->getId();
    if (itemId == 0)
    {
        current->isEquipped = false;
        hasItemEquipped = false;
        AP -= 3;
    }

    else if (itemId == 1)
    {
        current->isEquipped = false;
        hasItemEquipped = false;
        HP -= 12;

    }

    else if (itemId == 2)
    {
        current->isEquipped = false;
        hasItemEquipped = false;

    }
    else if (itemId == 3)
    {
        current->isEquipped = false;
        hasItemEquipped = false;
        Armor -= 2;
    }
    else if (!hasItemEquipped && itemId == 4)
    {
        // current->itemID = equippedItemID;
        current->isEquipped = true;
        hasItemEquipped = true;
        AP -= 2;
        
    }
    else if (!hasItemEquipped && itemId == 5)
    {
        // current->itemID = equippedItemID;
        current->isEquipped = true;
        hasItemEquipped = true;
        AP -= 5;
        Armor -= 1;

    }
    else if (!hasItemEquipped && itemId == 6)
    {
        // current->itemID = equippedItemID;
        current->isEquipped = true;
        hasItemEquipped = true;
        AP -= 7;
        Stamina -= 250;
    }
   
}
bool Character::equipItem(int equippedItemID)
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
   

    int itemId = current->item ? current->item->getId() : -1;
    if (!hasItemEquipped && itemId == 0) {

        
        current->isEquipped = true;
        hasItemEquipped = true;
        AP += 3;
    }

    else if (!hasItemEquipped && itemId == 1)
    {
       // current->itemID = equippedItemID;
        current->isEquipped = true;
        hasItemEquipped = true;
        HP += 12;
    }

    else if (!hasItemEquipped && itemId == 2)
    {
       // current->itemID = equippedItemID;
        current->isEquipped = true;
        hasItemEquipped = true;

    }

    else if (!hasItemEquipped && itemId == 3)
    {
       // current->itemID = equippedItemID;
        current->isEquipped = true;
        hasItemEquipped = true;
        Armor += 2;

    }

    else if (!hasItemEquipped && itemId == 4)
    {
        // current->itemID = equippedItemID;
        current->isEquipped = true;
        hasItemEquipped = true;
        AP += 2;

    }
    else if (!hasItemEquipped && itemId == 5)
    {
        // current->itemID = equippedItemID;
        current->isEquipped = true;
        hasItemEquipped = true;
        AP += 5;
        Armor += 1;

    }
    else if (!hasItemEquipped && itemId == 6)
    {
        // current->itemID = equippedItemID;
        current->isEquipped = true;
        hasItemEquipped = true;
        AP += 7;
        Stamina += 250;

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

    


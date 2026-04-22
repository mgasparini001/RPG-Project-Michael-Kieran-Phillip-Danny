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
            else
            {
                std::cout << "Qwaping, the attack did nothing\n";
            }
        }
        else {
            std::cout << Name << "is too exhuasted to make the attack!\n";
        }
    }
    else {
        //ranged attack
        if (Stamina > 50) {
            Stamina -= 50;
            if (diceRoll(AP) > +target.getArmor()) {//sees if attack will pierce target's armor (character's skill for dmg, AP for AP)
                attackMessage(target);
                target.takeDamage(diceRoll(Range) + Dmg);
            }
            else
            {
                std::cout << "The attack missed!" << std::endl;
            }
        }
        else
        {
            std::cout << Name << "is too exhuasted to make the attack!\n";
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

void Character::addItemToInventory(int itemID, int Dmg, int Armor, int ap, int sp, int hp, bool consumable, int quantity) {
    inventory.addItem(itemID, Dmg, Armor, ap, sp, hp, consumable, quantity);
}

bool Character::removeItemFromInventory(int itemID, ItemRegistry registry, player &p, int quantity) {
    return inventory.removeItem(itemID, registry, p, quantity);
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

void Character::unequipItem(ItemRegistry& registry)
{
    //find equipped item
    InventoryNode* current = inventory.getHead();
    if (hasItemEquipped)
    {
        while (current->isEquipped == false)
        {
            //std::cout << registry.getItemName(current->itemID) << " x" << current->quantity << "\n";
            current = current->next;
        }
        
        if (current != nullptr)
        {
            if (current->isConsummable)
            {
                current->isEquipped = false;
                equippedItem = nullptr;
                hasItemEquipped = false;
                equippedItemID = NULL;
                return;
            }

            current->isEquipped = false;
            equippedItem = nullptr;
            hasItemEquipped = false;
            equippedItemID = NULL;
            if (current->AP != NULL)
            {
                AP -= current->AP;
            }
            if (current->dmg != NULL)
            {
                Dmg -= current->dmg;
            }
            if (current->armor != NULL)
            {
                Armor -= current->armor;
            }
            if (current->HP != NULL)
            {
                HP -= current->HP;
            }
            if (current->stamina != NULL)
            {
                Stamina -= current->stamina;
            }
        }
    }
    
    
    //unequip
  /*  if (current->itemID == 0)
    {
        current->isEquipped = false;
        hasItemEquipped = false;
        AP -= 3;
    }

    else if (current->itemID == 1)
    {
        current->isEquipped = false;
        hasItemEquipped = false;

    }

    else if (current->itemID == 2)
    {
        current->isEquipped = false;
        hasItemEquipped = false;

    }

    else
    {
        current->isEquipped = false;
        hasItemEquipped = false;
        Armor -= 2;
    }*/
}
bool Character::equipItem(int equippedItemID, ItemRegistry &registry)
{
    
    InventoryNode* current = inventory.getHead();

    while (current != nullptr && current->itemID != equippedItemID)
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
        unequipItem(registry);
    }
    if (current->isConsummable)
    {
        current->isEquipped = true;
        hasItemEquipped = true;
        return true;
    }
    if (current != nullptr)
    {
        //equippedItem = current;
        current->isEquipped = true;
        hasItemEquipped = true;
        if (current->AP != NULL)
        {
            AP += current->AP;
        }
        if (current->dmg != NULL)
        {
            Dmg += current->dmg;
        }
        if (current->armor != NULL)
        {
            Armor += current->armor;
        }
        if (current->HP != NULL)
        {
            HP += current->HP;
        }
        if (current->stamina != NULL)
        {
            Stamina += current->stamina;
        }
        
    }
    

    //if (!hasItemEquipped && current->itemID == 0) {

    //    //attach equipped item to a var or node******
    //    current->isEquipped = true;
    //    hasItemEquipped = true;
    //    AP += 3;
    //}

    //else if (!hasItemEquipped && current->itemID == 1)
    //{
    //   // current->itemID = equippedItemID;
    //    current->isEquipped = true;
    //    hasItemEquipped = true;

    //}

    //else if (!hasItemEquipped && current->itemID == 2)
    //{
    //   // current->itemID = equippedItemID;
    //    current->isEquipped = true;
    //    hasItemEquipped = true;

    //}

    //else if (!hasItemEquipped && current->itemID == 3)
    //{
    //   // current->itemID = equippedItemID;
    //    current->isEquipped = true;
    //    hasItemEquipped = true;
    //    Armor += 2;

    //}
    return true;
   
}
bool Character::consumeItem(int itemID, ItemRegistry& registry)
{
    InventoryNode* current = inventory.getHead();

    while (current != nullptr && current->itemID != itemID)
    {
        current = current->next;
    }
    // checks for if you have the item before consumming it
    if (current == nullptr)
    {
        return false;
    }

    
    if (current != nullptr)
    {
        
        if (current->AP != NULL)
        {
            AP += current->AP;
        }
        if (current->dmg != NULL)
        {
            Dmg += current->dmg;
        }
        if (current->armor != NULL)
        {
            Armor += current->armor;
        }
        if (current->HP != NULL)
        {
            HP += current->HP;
        }
        if (current->stamina != NULL)
        {
            Stamina += current->stamina;
        }
        current->quantity -= 1;
        // if quantity is zero remove the node
        if (current->quantity <= 0)
        {
            if (hasItemEquipped == true)
            {
                unequipItem(registry);
            }

            if (current->prev != nullptr)
                current->prev->next = current->next;
            else
                inventory.setHead(current->next);

            if (current->next != nullptr)
                current->next->prev = current->prev;
            else
                inventory.setTail(current->prev);

            delete current;
        }
    }
    return true;
}
int Character::getEquippedItemID()
{
    if (hasItemEquipped)
    {
        InventoryNode* current = inventory.getHead();
        while (current->isEquipped == false)
        {
            //std::cout << registry.getItemName(current->itemID) << " x" << current->quantity << "\n";
            current = current->next;
        }

        return current->itemID;
    }
    
}



    


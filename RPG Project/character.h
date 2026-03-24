#pragma once
#include <string>
#include <iostream>
#include "Inventory.h"
#include "item.h"
#include <memory>

//forward declare so it can be used in character declaration
struct stats;
struct InventoryNode;
class ItemRegistry;
class player;

// Base Character class
class Character
{
protected:
    stats characterStats;

    // characters stats after accounting for item stat modifiers
    stats totalStats;

    // hp/stamina amount which changes in battle, not max hp/stamina
    int currentHP;
    int currentStamina;
    int Melee;
    int Range;
   
    std::shared_ptr<Item> equippedItem;
    bool hasItemEquipped;
    
    std::string Name;
    Inventory inventory;

public:
   
    Character();

	// constructor
    Character(const string& name, int melee, int range, bool itemEquipped, stats characterstats, stats totalstats, int currenthp, int currentstamina, int ap, int armor, int dmg);

	// virtual destructor (virtual for cleanup in derived classes)
    virtual ~Character() = default;

    virtual void attackMessage(Character& target) = 0;

	// basic attack function (virtual so derived classes can override if neccessary)
    bool attack(Character& target, bool attackType);

	// random dice roll function for various uses
	int diceRoll(int x);

	// function to reduce hp when taking damage
    void takeDamage(int amount);

    stats getCharacterStats();

    int getArmor();

	// check if character is alive
    bool isAlive() const;

	// get current hp
    int getHp() const;

    int getDmg();

    int getAp();

    void rest();

    bool flee();

    int getStamina();
	
	// get character name
    std::string getName() const;

    void setHp(int hp);

    // Inventory management
    Inventory& getInventory();
    void addItemToInventory(int itemID, ItemRegistry& registry, int quantity = 1);
    bool removeItemFromInventory(int itemID, Character &p, int quantity = 1);
    int getItemQuantity(int itemID) const;
    bool hasItem(int itemID) const;

   
    bool getHasItemEquipped();
    void unequipItem();
    bool equipItem(int equippedItemID);
    int getEquippedItemID();
    
    void heal();
   

    
    // updates characters strength with equipped items
    //stats getItemStats(Item item);
   
    stats getTotalStats();

    };
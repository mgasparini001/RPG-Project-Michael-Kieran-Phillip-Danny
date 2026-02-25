#pragma once

#include <memory>
#include "item.h"

//forward declare so it can be used in inventory declaration
class Character;
class player;

// node for dll in inventory
struct InventoryNode
{
    std::shared_ptr<Item> item; // item object
    int quantity;  // how much we has   
    InventoryNode* next;
    InventoryNode* prev;
    bool isEquipped;
     //how many different items you have in your inventory

    InventoryNode(const std::shared_ptr<Item>& item, int qty = 1)
        : item(item), quantity(qty), next(nullptr), prev(nullptr), isEquipped(false) {
    }
};

// inventory class
class Inventory
{
private:
    InventoryNode* head;
    InventoryNode* tail;
    int Size;

public:
    // constructor
    Inventory();

    // destructor for bang bang boom
    ~Inventory();

    int getSize();
    void setSize(int size);
    // add or increase
    void addItem(const std::shared_ptr<Item>& item, int quantity = 1);

    // remove or decrease
    bool removeItem(int itemID, Character &p, int quantity = 1);

    // get amount of specific item
    int getQuantity(int itemID) const;

    // check if inventory contains some item
    bool hasItem(int itemID) const;

    // clear entire inventory
    void clear();

    // print inventory contents with item names
    void printInventory(Character &p) const;

    InventoryNode* getHead();

    const std::shared_ptr<Item>& getItem(int id);
 
};

//test commit

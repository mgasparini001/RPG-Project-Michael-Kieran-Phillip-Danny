#pragma once
#include "ItemRegistry.h"

// node for dll in inventory
struct InventoryNode
{
    int itemID;      // id
    int quantity;    // how much we has
    InventoryNode* next;
    InventoryNode* prev;

    InventoryNode(int id, int qty = 1)
        : itemID(id), quantity(qty), next(nullptr), prev(nullptr) {}
};

// inventory class
class Inventory
{
private:
    InventoryNode* head;
    InventoryNode* tail;

public:
    // constructor
    Inventory();

    // destructor for bang bang boom
    ~Inventory();

    // add or increase
    void addItem(int itemID, int quantity = 1);

    // remove or decrease
    bool removeItem(int itemID, int quantity = 1);

    // get amount of specific item
    int getQuantity(int itemID) const;

    // check if inventory contains some item
    bool hasItem(int itemID) const;

    // clear entire inventory
    void clear();

    // print inventory contents with item names
    void printInventory(ItemRegistry& registry) const;
};

//test commit

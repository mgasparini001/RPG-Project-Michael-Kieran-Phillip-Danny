#pragma once


//forward declare so it can be used in inventory declaration
class Character;
class player;
class ItemRegistry;

// node for dll in inventory
struct InventoryNode
{
    int itemID;      // id
    int quantity;  // how much we has   
    InventoryNode* next;
    InventoryNode* prev;
    bool isEquipped;
     //how many different items you have in your inventory

    InventoryNode(int id, int qty = 1)
        : itemID(id), quantity(qty), next(nullptr), prev(nullptr), isEquipped(false) {
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
    void addItem(int itemID, int quantity = 1);

    // remove or decrease
    bool removeItem(int itemID, ItemRegistry registry, Character &p, int quantity = 1);

    // get amount of specific item
    int getQuantity(int itemID) const;

    // check if inventory contains some item
    bool hasItem(int itemID) const;

    // clear entire inventory
    void clear(ItemRegistry itemreg);

    // print inventory contents with item names
    void printInventory(ItemRegistry& registry, Character &p) const;

    InventoryNode* getHead();
 
};

//test commit

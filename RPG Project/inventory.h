#pragma once


//forward declare so it can be used in inventory declaration
class Character;
class player;
class ItemRegistry;

// node for dll in inventory
struct InventoryNode
{
    int itemID;      // id
    int quantity;    // how much we has
    InventoryNode* next;
    InventoryNode* prev;
    bool isEquipped;
    int dmg;
    int armor;
    int AP;
    int HP;
    int stamina;
    
    InventoryNode(int id, int qty = 1, int Dmg, int Armor, int ap, int hp, int sp)
        : itemID(id), quantity(qty), next(nullptr), prev(nullptr), isEquipped(false), dmg(Dmg), armor(Armor), AP(ap), HP(hp), stamina(sp) {
    }
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
    bool removeItem(int itemID, ItemRegistry registry, player &p, int quantity = 1);

    // get amount of specific item
    int getQuantity(int itemID) const;

    // check if inventory contains some item
    bool hasItem(int itemID) const;

    // clear entire inventory
    void clear();

    // print inventory contents with item names
    void printInventory(ItemRegistry& registry, Character &p) const;

    InventoryNode* getHead();
 
    int getDmg(int itemID);

    int getArmor(int itemID);

    int getAP(int itemID);

    int getHP(int itemID);

    int getStamina(int itemID);
};

//test commit

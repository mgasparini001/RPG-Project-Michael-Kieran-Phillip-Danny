#include "Inventory.h"
#include <iostream>
#include "player.h"
#include "ItemRegistry.h"

Inventory::Inventory() : head(nullptr), tail(nullptr), Size(0) {}

Inventory::~Inventory()
{
   //sorry, i changed clear params trying to keep track of inv size so user cant add item outside id range
   // -mike 
   //clear();
}
int Inventory::getSize()
{
    return Size;
}

void Inventory::setSize(int size)
{
    Size = size;
}

void Inventory::addItem(int itemID, int quantity)
{
    // check for existing item
    InventoryNode* current = head;
    while (current != nullptr)
    {
        if (current->itemID == itemID)
        {
            current->quantity += quantity;
            return;
        }
        current = current->next;
    }
 
    // item not found, create new one
    InventoryNode* newNode = new InventoryNode(itemID, quantity);
    // keeps track of how many item types there are
    Size += 1;
    
    if (head == nullptr)
    {
        head = tail = newNode;
    }
    else
    {
        tail->next = newNode;
        newNode->prev = tail;
        tail = newNode;
    }
}


bool Inventory::removeItem(int itemID, ItemRegistry registry, Character &p, int quantity)
{
    InventoryNode* current = head;

    while (current != nullptr)
    {
        if (current->itemID == itemID)
        {
            current->quantity -= quantity;

            // if quantity is zero remove the node
            if (current->quantity <= 0)
            {
                if (p.getHasItemEquipped() == true)
                {
                    p.unequipItem(registry);
                }
               
                if (current->prev != nullptr)
                    current->prev->next = current->next;
                else
                    head = current->next;

                if (current->next != nullptr)
                    current->next->prev = current->prev;
                else
                    tail = current->prev;

                delete current;
                Size -= 1;
            }
            return true;
        }
        current = current->next;
    }
    return false;
}

int Inventory::getQuantity(int itemID) const
{
    InventoryNode* current = head;

    while (current != nullptr)
    {
        if (current->itemID == itemID)
            return current->quantity;
        current = current->next;
    }
    return 0;
}

bool Inventory::hasItem(int itemID) const
{
    return getQuantity(itemID) > 0;
}


void Inventory::clear(ItemRegistry itemreg)
{
    while (head != nullptr)
    {
        InventoryNode* temp = head;
        head = head->next;
        delete temp;
        Size -= 1;
    }
    tail = nullptr;
}




void Inventory::printInventory(ItemRegistry& registry, Character &p) const
{
    if (head == nullptr)
    {
        std::cout << "Inventory is empty.\n";
        return;
    }

    InventoryNode* current = head;

    std::cout << "=== Inventory ===\n";

    if (p.getHasItemEquipped() == true)
    {
        std::cout << "\nEquipped: " << registry.getItemName(p.getEquippedItemID()) << std::endl << std::endl;
    }
    else
    {
        std::cout << "Equipped: N/A\n";
    }
    
   
    while (current != nullptr)
    {
        std::cout << current->itemID << ") " << registry.getItemName(current->itemID) << " x" << current->quantity << "\n";
        current = current->next;
       
    }
    std::cout << "\t\tCHOOSE AN ITEM TO LOOK AT ITS STATS (WIP/not req)\n";
    std::cout << "==================\n";
}

InventoryNode* Inventory::getHead() {
    return head;
}


//test commit
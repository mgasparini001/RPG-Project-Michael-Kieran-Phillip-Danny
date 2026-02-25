#include "Inventory.h"
#include <iostream>
#include "player.h"

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

void Inventory::addItem(const std::shared_ptr<Item>& item, int quantity)
{
    if (!item || quantity <= 0)
    {
        return;
    }
    // check for existing item
    InventoryNode* current = head;
    while (current != nullptr)
    {
        if (current->item && current->item->getId() == item->getId())
        {
            current->quantity += quantity;
            return;
        }
        current = current->next;
    }
 
    // item not found, create new one
    InventoryNode* newNode = new InventoryNode(item, quantity);
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


bool Inventory::removeItem(int itemID, Character &p, int quantity)
{
    InventoryNode* current = head;

    while (current != nullptr)
    {
        if (current->item && current->item->getId() == itemID)
        {
            current->quantity -= quantity;

            // if quantity is zero remove the node
            if (current->quantity <= 0)
            {
                if (p.getHasItemEquipped() == true)
                {
                    p.unequipItem();
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
        if (current->item && current->item->getId() == itemID)
            return current->quantity;
        current = current->next;
    }
    return 0;
}

bool Inventory::hasItem(int itemID) const
{
    return getQuantity(itemID) > 0;
}


void Inventory::clear()
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




void Inventory::printInventory(Character &p) const
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
        InventoryNode* equipped = head;
        while (equipped != nullptr && equipped->isEquipped == false)
        {
            equipped = equipped->next;
        }
        if (equipped && equipped->item)
        {
            std::cout << "\nEquipped: " << equipped->item->getName() << std::endl << std::endl;
        }
        else
        {
            std::cout << "Equipped: N/A\n";
        }
    }
    else
    {
        std::cout << "Equipped: N/A\n";
    }
    
   
    while (current != nullptr)
    {
        if (current->item)
        {
            std::cout << current->item->getId() << ") " << current->item->getName() << " x" << current->quantity << "\n";
        }
        current = current->next;
       
    }
    std::cout << "\t\tCHOOSE AN ITEM TO LOOK AT ITS STATS (WIP/not req)\n";
    std::cout << "==================\n";
}

InventoryNode* Inventory::getHead() {
    return head;
}

const std::shared_ptr<Item>& Inventory::getItem(int id)
{
    InventoryNode* current = head;

    while (current != nullptr)
    {
        if (current->item && current->item->getId() == id)
            return current->item;
        current = current->next;
    }
}


void Inventory::printInventoryStore(Character& p) const
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
        InventoryNode* equipped = head;
        while (equipped != nullptr && equipped->isEquipped == false)
        {
            equipped = equipped->next;
        }
        if (equipped && equipped->item)
        {
            std::cout << "\nEquipped: " << equipped->item->getName() << std::endl << std::endl;
        }
        else
        {
            std::cout << "Equipped: N/A\n";
        }
    }
    else
    {
        std::cout << "Equipped: N/A\n";
    }


    while (current != nullptr)
    {
        if (current->item)
        {
            std::cout << current->item->getId() << ") " << current->item->getName() << " x" << current->quantity << "Price for one: " << current->item->getValue() << "\n";
        }
        current = current->next;

    }
    std::cout << "\t\tCHOOSE AN ITEM TO LOOK AT ITS STATS (WIP/not req)\n";
    std::cout << "==================\n";
}

//test commit
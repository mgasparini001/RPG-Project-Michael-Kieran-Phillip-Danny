#include "Inventory.h"
#include <iostream>
#include "player.h"

Inventory::Inventory() : head(nullptr), tail(nullptr) {}

Inventory::~Inventory()
{
    clear();
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

bool Inventory::removeItem(int itemID, int quantity)
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
                if (current->prev != nullptr)
                    current->prev->next = current->next;
                else
                    head = current->next;

                if (current->next != nullptr)
                    current->next->prev = current->prev;
                else
                    tail = current->prev;

                delete current;
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


void Inventory::clear()
{
    while (head != nullptr)
    {
        InventoryNode* temp = head;
        head = head->next;
        delete temp;
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
    std::cout << "Equipped: " << registry.getItemName(current->itemID) << std::endl;
    while (current != nullptr)
    {
        std::cout << registry.getItemName(current->itemID) << " x" << current->quantity << "\n";
        current = current->next;
    }
    std::cout << "==================\n";
}

InventoryNode* Inventory::getHead() {
    return head;
}


//test commit
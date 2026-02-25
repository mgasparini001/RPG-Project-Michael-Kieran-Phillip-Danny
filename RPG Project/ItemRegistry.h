#pragma once
#include <memory>
#include <string>
#include "item.h"

// Simple item registry - just a table mapping item IDs to names
class ItemRegistry
{
private:
    static const int MAX_ITEMS = 50;
    std::shared_ptr<Item> items[MAX_ITEMS];
   
public:
    ItemRegistry();
    
    // Set item for an ID
    void setItem(int itemID, const std::shared_ptr<Item>& item);
    
    // Get item by ID
    std::shared_ptr<Item> getItem(int itemID) const;

    // Get item name by ID
    std::string getItemName(int itemID) const;
    
    // Print all registered items
    void printRegistry() const;
};

//test commit
#pragma once
#include <string>

// Simple item registry - just a table mapping item IDs to names
class ItemRegistry
{
private:
    static const int MAX_ITEMS = 50;
    std::string itemNames[MAX_ITEMS];

public:
    ItemRegistry();
    
    // Set item name for an ID
    void setItemName(int itemID, const std::string& name);
    
    // Get item name by ID
    std::string getItemName(int itemID) const;
    
    // Print all registered items
    void printRegistry() const;
};

//test commit
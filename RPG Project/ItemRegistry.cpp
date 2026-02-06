#include <iostream>
#include "ItemRegistry.h"

ItemRegistry::ItemRegistry()
{
    // set all items as empty strings
    for (int i = 0; i < MAX_ITEMS; i++)
    {
        itemNames[i] = "";
    }
    equippedItemName = "";
}

// set item to ID
void ItemRegistry::setItemName(int itemID, const std::string& name)
{
    if (itemID >= 0 && itemID < MAX_ITEMS)
    {
        itemNames[itemID] = name;
    }
}

// get the item by ID
std::string ItemRegistry::getItemName(int itemID) const
{
    if (itemID >= 0 && itemID < MAX_ITEMS)
    {
        return itemNames[itemID];
    }
    return "Unknown Item";
}

// print all registered items
void ItemRegistry::printRegistry() const
{
    for (int i = 0; i < MAX_ITEMS; i++)
    {
        if (itemNames[i] != "")
        {
            std::cout << "ID " << i << ": " << itemNames[i] << "\n";
        }
    }
}



//test commit

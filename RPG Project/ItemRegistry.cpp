#include <iostream>
#include "ItemRegistry.h"

ItemRegistry::ItemRegistry()
{
    // set all items as empty
    for (int i = 0; i < MAX_ITEMS; i++)
    {
        items[i] = nullptr;
    }
}

// set item to ID
void ItemRegistry::setItem(int itemID, const std::shared_ptr<Item>& item)
{
    if (itemID >= 0 && itemID < MAX_ITEMS)
    {
        items[itemID] = item;
    }
}

std::shared_ptr<Item> ItemRegistry::getItem(int itemID) const
{
    if (itemID >= 0 && itemID < MAX_ITEMS)
    {
        return items[itemID];
    }
    return nullptr;
}

// get the item by ID
std::string ItemRegistry::getItemName(int itemID) const
{
    std::shared_ptr<Item> item = getItem(itemID);
    if (item)
    {
        return item->getName();
    }
    return "Unknown Item";
}

bool ItemRegistry::getItemisConsumable(int itemID) const
{
    std::shared_ptr<Item> item = getItem(itemID);
    if (item)
    {
        return item->isConsumable();
    }
    return "Unknown Item";
}
// print all registered items
void ItemRegistry::printRegistry() const
{
    for (int i = 0; i < MAX_ITEMS; i++)
    {
        if (items[i])
        {
            std::cout << "ID " << i << ": " << items[i]->getName() << "\n";
        }
    }
}

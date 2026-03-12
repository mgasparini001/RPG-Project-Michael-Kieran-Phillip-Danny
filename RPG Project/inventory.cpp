#include "Inventory.h"
#include <iostream>
#include "player.h"
#include "ItemRegistry.h"
using namespace std;

//forward declare
void refreshScreen();

Inventory::Inventory() : head(nullptr), tail(nullptr), Size(0) {}

Inventory::~Inventory()
{
   
   clear();
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


void Inventory::printInventoryStore(Character &o, player& p) const
{
    if (head == nullptr)
    {
        std::cout << "The store is empty.\n";
        return;
    }

    InventoryNode* current = head;

    std::cout << "=== Store ===\n";
    std::cout << "Gold: " << p.getGold() << std::endl << std::endl;

    while (current != nullptr)
    {
        if (current->item)
        {
            std::cout << current->item->getId() << ") " << current->item->getName() << " x" << current->quantity << " | Price for one: " << current->item->getValue() << "\n";
        }
        current = current->next;

    }
    std::cout << "\t\tCHOOSE AN ITEM TO LOOK AT ITS STATS (WIP/not req)\n";
    std::cout << "==================\n";
}


void Inventory::manageInventory(player& p1, ItemRegistry& registry, bool inBattle)
{

    bool inInv = true;
    bool isEquipped = false;
    while (inInv)
    {
        //WIP inv screen
        refreshScreen();
        cout << "\n    INVENTORY    ";
        cout << "\n  Player: " << p1.getName() << "\n\n";

        // manage inv in battle

        if (inBattle == true)
        {
              
            cout << R"(
============================
|  1. View Inventory       |
|  2. Equip/Use Item       |
|  3. Exit                 |
============================
)";

            if (p1.getHasItemEquipped() == true)
            {
                std::cout << "\n\t\t\tEquipped: " << registry.getItemName(p1.getEquippedItemID()) << std::endl << std::endl;
            }
            int choice;
            bool valid = false;
            while (!valid)
            {
                while (!(cin >> choice))
                {
                    cout << "Invalid input, please enter a number\n";
                    cin.clear();
                    cin.ignore(1000, '\n');
                }
                cin.clear();
                cin.ignore(1000, '\n');
                valid = true;
            }

            if (choice == 1)
            {
                refreshScreen();
                cout << "\n";
                p1.getInventory().printInventory(p1);
                cout << "press enter";
                cin.ignore();
                cin.get();
            }
          
            else if (choice == 2)
            {
                refreshScreen();
                p1.getInventory().printInventory(p1);
                int itemID;

                cout << "enter item ID: ";
                cin >> itemID;
                while (!(p1.hasItem(itemID)))
                {
                    cout << "Outside input range, womp womp\n";
                    cout << "enter a valid ID: ";
                    cin >> itemID;
                }

                bool equip = p1.equipItem(itemID);
                if (equip == true)
                {
                    if (registry.getItemisConsumable(itemID) == true)
                    {
                        cout << "Consumed " << registry.getItemName(itemID) << "!\n";
                        p1.removeItemFromInventory(itemID, p1, 1);

                    }

                    else
                    {
                        isEquipped = true;
                        cout << "Equipped " << registry.getItemName(itemID) << "!\n";
                    }

                }
                else
                {
                    cout << "you cant equip an item you dont have, dingus\n";
                }

                cout << "press enter";
                cin.ignore();
                cin.get();

            }
            else if (choice == 3)
            {
                inInv = false;
                refreshScreen();
            }
            else
            {
                cout << "invalid option womp womp\n";
            }
        }

        // manage inv outside battle

        else
        {


            cout << R"(
============================
|  1. Drop Item            |
|  2. View Inventory       |
|  3. Equip/Use Item       |
|  4. Exit                 |
============================
)";

            if (p1.getHasItemEquipped() == true)
            {
                std::cout << "\n\t\t\tEquipped: " << registry.getItemName(p1.getEquippedItemID()) << std::endl << std::endl;
            }
            int choice;
            cin >> choice;

           if (choice == 1)
            {
                refreshScreen();
                cout << "\n";
                p1.getInventory().printInventory(p1);

                int itemID, quantity;
                cout << "enter item ID to remove: ";
                cin >> itemID;
                cout << "enter quantity: ";
                cin >> quantity;

                if (p1.removeItemFromInventory(itemID, p1, quantity))
                {
                    cout << "Removed " << quantity << " of " << registry.getItemName(itemID) << "!\n";
                }
                else
                {
                    cout << "Item not found!\n";
                }
                cout << "press enter";
                cin.ignore();
                cin.get();
            }
            else if (choice == 2)
            {
                refreshScreen();
                cout << "\n";
                p1.getInventory().printInventory(p1);
                cout << "press enter";
                cin.ignore();
                cin.get();
            }
           
            else if (choice == 3)
            {
                refreshScreen();
                p1.getInventory().printInventory(p1);
                int itemID;

                cout << "enter item ID: ";
                cin >> itemID;
                while (!(p1.hasItem(itemID)))
                {
                    cout << "Outside input range, womp womp\n";
                    cout << "enter a valid ID: ";
                    cin >> itemID;
                }

                bool equip = p1.equipItem(itemID);
                if (equip == true)
                {
                    if (registry.getItemisConsumable(itemID) == true)
                    {
                        cout << "Consumed " << registry.getItemName(itemID) << "!\n";
                        p1.removeItemFromInventory(itemID, p1, 1);

                    }

                    else
                    {
                        isEquipped = true;
                        cout << "Equipped " << registry.getItemName(itemID) << "!\n";
                    }

                }
                else
                {
                    cout << "you cant equip an item you dont have, dingus\n";
                }

                cout << "press enter";
                cin.ignore();
                cin.get();

            }
            else if (choice == 4)
           {
               inInv = false;
               refreshScreen();
           }
            else
            {
                cout << "invalid option womp womp\n";
            }
        }
    }
    refreshScreen();
}

void Inventory::inventoryDebug(player& p1, ItemRegistry& registry)
{

    bool inInv = true;
    bool isEquipped = false;
    while (inInv)
    {
        //WIP inv screen
        refreshScreen();
        cout << "\n    INVENTORY    ";
        cout << "\n  Player: " << p1.getName() << "\n\n";

        cout << R"(
============================
|  1. Add Item             |
|  2. Remove Item          |
|  3. View Inventory       |
|  4. Equip/Use Item       |
|  5. Exit                 |
============================
)";

        if (p1.getHasItemEquipped() == true)
        {
            std::cout << "\n\t\t\tEquipped: " << registry.getItemName(p1.getEquippedItemID()) << std::endl << std::endl;
        }
        int choice;
        cin >> choice;

        if (choice == 1)
        {
            refreshScreen();
            cout << "\aitems:\n";
            registry.printRegistry();

            int itemID, quantity;
            cout << "enter item ID: ";
            cin >> itemID;
            cout << "enter quantity: ";
            cin >> quantity;

            p1.addItemToInventory(itemID, registry, quantity);
            cout << "Added " << quantity << " of " << registry.getItemName(itemID) << "!\n";
            cout << "press enter";
            cin.ignore();
            cin.get();
        }
        else if (choice == 2)
        {
            refreshScreen();
            cout << "\n";
            cout << "\aitems:\n";
            registry.printRegistry();
            cout << endl;
            p1.getInventory().printInventory(p1);

            int itemID, quantity;
            cout << "enter item ID to remove: ";
            cin >> itemID;
            cout << "enter quantity: ";
            cin >> quantity;

            if (p1.removeItemFromInventory(itemID, p1, quantity))
            {
                cout << "Removed " << quantity << " of " << registry.getItemName(itemID) << "!\n";
            }
            else
            {
                cout << "Item not found!\n";
            }
            cout << "press enter";
            cin.ignore();
            cin.get();
        }
        else if (choice == 3)
        {
            refreshScreen();
            cout << "\n";
            p1.getInventory().printInventory(p1);
            cout << "press enter";
            cin.ignore();
            cin.get();
        }
        else if (choice == 5)
        {
            inInv = false;
            refreshScreen();
        }
        else if (choice == 4)
        {
            refreshScreen();
            cout << "\aitems:\n";
            registry.printRegistry();
            cout << endl;
            p1.getInventory().printInventory(p1);
            int itemID;

            cout << "enter item ID: ";
            cin >> itemID;
            while (!(p1.hasItem(itemID)))
            {
                cout << "Outside input range, womp womp\n";
                cout << "enter a valid ID: ";
                cin >> itemID;
            }

            bool equip = p1.equipItem(itemID);
            if (equip == true)
            {
                if (registry.getItemisConsumable(itemID) == true)
                {
                    cout << "Consumed " << registry.getItemName(itemID) << "!\n";
                    p1.removeItemFromInventory(itemID, p1, 1);

                }

                else
                {
                    isEquipped = true;
                    cout << "Equipped " << registry.getItemName(itemID) << "!\n";
                }

            }
            else
            {
                cout << "you cant equip an item you dont have, dingus\n";
            }

            cout << "press enter";
            cin.ignore();
            cin.get();

        }
        else
        {
            cout << "invalid option womp womp\n";
        }
    }
    refreshScreen();
}
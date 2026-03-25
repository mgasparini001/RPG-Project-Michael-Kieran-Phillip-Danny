
#pragma once
class enemy;
class player;
class ItemRegistry;
class Inventory;

// Input: an enemy and a player    Output: an interactive battle loop that reverts to main menu upon ending
// desc: displays enemy name and hp, and starts battle sequence loop, letting user either fight, rest, or flee. uses Character hp, stamina, dmg, and armor penetration (strength)
// lets player decide on melee or ranged attack, with ranged having a chance to miss but taking less stamina

void enterBattle(enemy& enemy, player& p1, ItemRegistry& registry, Inventory inv, bool canRun, bool isWild);
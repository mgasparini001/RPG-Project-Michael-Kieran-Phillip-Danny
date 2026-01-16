#include <iostream>
#include <conio.h>
#include <windows.h>
#include "player.h"
#include "enemy.h"
#include "npc.h"
#include "item.h"

using namespace std;

// map size
const int mapwidth = 30;
const int mapheight = 12;

// clear screen function
void clearScreen() {
    system("cls");
}

// global variables
char map[mapheight][mapwidth];
int playerX = 5;
int playerY = 5;

// set up enemys
enemy goblin("goblin", 30, 3, 4, 5, 2, 2, 8, 50);
int goblinX = 15;
int goblinY = 7;
bool goblinAlive = true;

// npcs data
npc merchant("merchant", "Hey kid, wanna buy a sword?", 20, 1, 2, 1, 8, 0, 0, 0);
int merchantX = 10;
int merchantY = 3;

// item data
Item potion("health potion", "restores 20 hp", 50, true);
int potionX = 8;
int potionY = 8;
bool potionExists = true;

// create map
void setupMap() {
    // fill with empty spaces
    for (int y = 0; y < mapheight; y++) {
        for (int x = 0; x < mapwidth; x++) {
            map[y][x] = ' ';
        }
    }
    
    // add borders of walls
    for (int x = 0; x < mapwidth; x++) {
        map[0][x] = '#';
        map[mapheight - 1][x] = '#';
    }
    for (int y = 0; y < mapheight; y++) {
        map[y][0] = '#';
        map[y][mapwidth - 1] = '#';
    }
    
    // add more walls
    for (int x = 10; x < 15; x++) {
        map[5][x] = '#';
    }
}

// check if player can move to pos
bool canMove(int x, int y) {
    // dont let the player go outside the map
    if (x <= 0 || x >= mapwidth - 1) return false;
    if (y <= 0 || y >= mapheight - 1) return false;
    
    // dont let the player go through walls
    if (map[y][x] == '#') return false;
    
    return true;
}

// Draw everything on screen
void drawGame(player& p) {
    clearScreen();
    
    // Draw each position on the map
    for (int y = 0; y < mapheight; y++) {
        for (int x = 0; x < mapwidth; x++) {
            
            // Check what to draw at this position
            if (x == playerX && y == playerY) {
                cout << '@';  // Player
            }
            else if (x == goblinX && y == goblinY && goblinAlive) {
                cout << 'E';  // Enemy
            }
            else if (x == merchantX && y == merchantY) {
                cout << 'N';  // NPC
            }
            else if (x == potionX && y == potionY && potionExists) {
                cout << 'I';  // Item
            }
            else {
                cout << map[y][x];  // Wall or empty space
            }
        }
        cout << endl;
    }
    
    // Show player stats
    cout << "\n=== " << p.getName() << " ===" << endl;
    cout << "HP: " << p.getHp() << " | Gold: " << p.gold << endl;
    cout << "\nControls: [WASD] Move | [E] Interact | [Q] Quit" << endl;
}

// Check if player is next to something and interact with it
void interact(player& p) {
    // Check if next to enemy
    if (abs(goblinX - playerX) <= 1 && abs(goblinY - playerY) <= 1 && goblinAlive) {
        cout << "\nYou encountered a " << goblin.getName() << "!" << endl;
        cout << "Press [1] to attack: ";
        char choice = _getch();
        
        if (choice == '1') {
            p.attack(goblin);
            
            if (!goblin.isAlive()) {
                cout << "\nYou defeated the goblin!" << endl;
                int gold = goblin.lootDrop();
                p.gold += gold;
                goblinAlive = false;
            } else {
                goblin.attack(p);
            }
        }
        cout << "\nPress any key to continue...";
        _getch();
        return;
    }
    
    // Check if next to NPC
    if (abs(merchantX - playerX) <= 1 && abs(merchantY - playerY) <= 1) {
        p.interact(merchant);
        cout << "\nPress any key to continue...";
        _getch();
        return;
    }
    
    // Check if standing on item
    if (playerX == potionX && playerY == potionY && potionExists) {
        p.interact(potion);
        potionExists = false;  // Item picked up
        cout << "\nPress any key to continue...";
        _getch();
        return;
    }
    
    cout << "\nNothing here to interact with." << endl;
    cout << "Press any key to continue...";
    _getch();
}

int main() {
    // Setup random numbers
    srand(time(0));
    
    // Create the player
    player hero("Hero", 100, 5, 6, 7, 5, 3, 15);
    
    // Setup the map
    setupMap();
    
    // Main game loop
    bool playing = true;
    while (playing && hero.isAlive()) {
        
        // Draw everything
        drawGame(hero);
        
        // Get player input
        char key = _getch();
        
        // Handle movement
        if (key == 'w' && canMove(playerX, playerY - 1)) {
            playerY--;  // Move up
        }
        else if (key == 's' && canMove(playerX, playerY + 1)) {
            playerY++;  // Move down
        }
        else if (key == 'a' && canMove(playerX - 1, playerY)) {
            playerX--;  // Move left
        }
        else if (key == 'd' && canMove(playerX + 1, playerY)) {
            playerX++;  // Move right
        }
        // Handle interaction
        else if (key == 'e' || key == 'E') {
            interact(hero);
        }
        // Handle quit
        else if (key == 'q' || key == 'Q') {
            playing = false;
        }
    }
    
    // Game over
    clearScreen();
    if (!hero.isAlive()) {
        cout << "\n=== GAME OVER ===" << endl;
        cout << "You were defeated!" << endl;
    } else {
        cout << "\n=== Thanks for playing! ===" << endl;
    }
    
    return 0;
}

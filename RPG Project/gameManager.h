#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "overworldMap.h"
#include "MapController.h"
#include "player.h"
#include "ItemRegistry.h"
#include "Store.h"
#include "npc.h"
#include "battle.h"
#include <filesystem>
#include <memory>
#include <string>
#include <vector>

// Manages the overall game state and rendering to a single window
class GameManager
{
public:
    static constexpr int WINDOW_WIDTH = 1440;
    static constexpr int WINDOW_HEIGHT = 960;
    static constexpr float MAP_VIEWPORT_HEIGHT_RATIO = 0.82f;

    GameManager();
    ~GameManager();

    // Initialize the game
    bool initialize();

    // Main game loop
    void run();

private:
    enum class RunPhase
    {
        SaveSelection,
        Playing,
        ConfirmExitSave
    };

    // Window and rendering
    sf::RenderWindow window;
    sf::Font font;
    sf::Music backgroundMusic;

    // Game state
    OverworldMap map;
    MapController mapController;
    player mapPlayer;
    ItemRegistry itemRegistry;
    std::unique_ptr<store> mapShop;
    std::unique_ptr<npc> shopNpc;

    // Interaction popup state (only text shown during play)
    std::string popupMessage;
    float popupTimeRemaining = 0.0f;

    // Input debouncing
    float timeSinceLastMove = 0.0f;
    static constexpr float MOVE_COOLDOWN = 0.15f;

    bool inventoryOpen = false;
    bool shopOpen = false;
    int selectedInventoryIndex = 0;

    RunPhase runPhase = RunPhase::SaveSelection;
    std::vector<std::filesystem::path> availableSaveFiles;
    std::filesystem::path selectedSaveFile;
    int selectedSaveIndex = 0;
    std::string saveSelectionMessage;
    std::string exitPromptMessage;

    bool musicReady = false;

    std::unique_ptr<BattleEncounter> activeBattle;

    // Helper methods
    void renderMapViewport();
    void renderClientViewport();
    void renderPopup();
    void renderInventoryPanel(float panelTop, float panelHeight);
    void renderShopPanel(float panelTop, float panelHeight);
    void renderSaveSelectionScreen();
    void renderExitPrompt();
    void tryInteraction();
    void showPopup(const std::string& message, float seconds = 2.2f);
    bool initializeBackgroundMusic();
    void initializeMapGameplayState();
    void applyDefaultPlayerState();
    OverworldMap::PlayerState buildPlayerStateForSave();
    void applyLoadedPlayerState(const OverworldMap::PlayerState& state);
    bool initializeMapFromSaveFile();
    bool loadSelectedSaveFile();
    bool loadSaveFile(const std::filesystem::path& path);
    bool saveCurrentGameToSelectedFile();
    std::vector<int> getInventoryItemIds();
    void equipSelectedInventoryItem();
    bool getShopListing(int& itemId, std::string& itemName, int& itemPrice) const;
    void tryBuyShopItem();
    void handleKeyPressed(sf::Keyboard::Scancode scancode);
    void handleSaveSelectionInput(sf::Keyboard::Scancode scancode);
    void handleExitConfirmInput(sf::Keyboard::Scancode scancode);
    void handlePlayingKeyInput(sf::Keyboard::Scancode scancode);
    void startWildBattle();
    void renderBattleOverlay();
    void handleInput();
    void update(float deltaTime);
    void render();
};

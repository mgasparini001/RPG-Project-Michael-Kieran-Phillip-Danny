#include "gameManager.h"
#include <algorithm>
#include <iostream>
#include <filesystem>
#include <sstream>
#include <vector>

#include "item.h"
#include "fodder.h"

// tiny helpers for finding assets/save files no matter where app starts from
namespace
{
std::filesystem::path resolveAssetPath(const std::string& fileName)
{
    const std::filesystem::path cwd = std::filesystem::current_path();
    const std::filesystem::path candidates[] = {
        cwd / fileName,
        cwd / "RPG Project" / fileName,
        cwd / "x64" / "Debug" / fileName,
        cwd.parent_path() / "RPG Project" / fileName,
        cwd.parent_path() / fileName
    };

    for (const auto& candidate : candidates)
    {
        if (std::filesystem::exists(candidate))
        {
            return candidate;
        }
    }

    return cwd / fileName;
}

// checks a couple places for Maps folder
std::filesystem::path resolveMapsDirectory()
{
    const std::filesystem::path cwd = std::filesystem::current_path();
    const std::filesystem::path candidates[] = {
        cwd / "RPG Project" / "Maps",
        cwd / "Maps",
        cwd.parent_path() / "RPG Project" / "Maps"
    };

    for (const auto& candidate : candidates)
    {
        if (std::filesystem::exists(candidate) && std::filesystem::is_directory(candidate))
        {
            return candidate;
        }
    }

    // Default location
    return cwd / "RPG Project" / "Maps";
}


// grab all json save files and keep them sorted
std::vector<std::filesystem::path> getMapFiles(const std::filesystem::path& mapsDir)
{
    std::vector<std::filesystem::path> files;
    if (!std::filesystem::exists(mapsDir))
    {
        return files;
    }

    for (const auto& entry : std::filesystem::directory_iterator(mapsDir))
    {
        if (!entry.is_regular_file())
        {
            continue;
        }

        if (entry.path().extension() != ".json")
        {
            continue;
        }

        const std::string fileName = entry.path().filename().string();
        if (fileName.size() >= 11 && fileName.substr(fileName.size() - 11) == "_reset.json")
        {
            continue;
        }

        files.push_back(entry.path());
    }

    std::sort(files.begin(), files.end());
    return files;
}
}

// setup main game window and starting player object
GameManager::GameManager()
    : window(sf::VideoMode(sf::Vector2u(WINDOW_WIDTH, WINDOW_HEIGHT)), "RPG Game"),
      mapPlayer("Ash", 50, 8, 4, 3, 1000, 6, 7, 0, 100)
{
    window.setFramerateLimit(60);
}

// nothing special needed on destroy right now
GameManager::~GameManager()
{
}

// try to load map and player state from a save file, return success
bool GameManager::initialize()
{
    // try local font first then windows font fallback
    const std::vector<std::string> fontPaths = {
        "arial.ttf",
        "C:\\Windows\\Fonts\\arial.ttf"
    };

    for (const auto& path : fontPaths)
    {
        if (font.openFromFile(path))
        {
            break;
        }
    }

    initializeBackgroundMusic();
    initializeMapGameplayState();
    mapController.initialize(&map);

    if (!initializeMapFromSaveFile())
    {
        return false;
    }

    return true;
}

// main game loop, runs until player closes window
void GameManager::run()
{
    sf::Clock clock;

    // main game loop runs til player closes window
    while (window.isOpen())
    {
        float deltaTime = clock.restart().asSeconds();

        // handle window + keyboard events
        while (const auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                handleKeyPressed(keyPressed->scancode);
            }
        }

        if (runPhase == RunPhase::Playing)
        {
            handleInput();
        }

        // update timers and ui bits
        update(deltaTime);

        // draw current frame
        window.clear(sf::Color::Black);
        render();
        window.display();
    }
}

// top level key handler, goes to diff handlers based on game state
void GameManager::handleKeyPressed(sf::Keyboard::Scancode scancode)
{
    // keep save select input isolated
    if (runPhase == RunPhase::SaveSelection)
    {
        handleSaveSelectionInput(scancode);
        return;
    }

    // keep exit confirm input isolated
    if (runPhase == RunPhase::ConfirmExitSave)
    {
        handleExitConfirmInput(scancode);
        return;
    }

    // normal gameplay keys
    handlePlayingKeyInput(scancode);
}

//save file input handeling
void GameManager::handleSaveSelectionInput(sf::Keyboard::Scancode scancode)
{
    if (scancode == sf::Keyboard::Scancode::Escape)
    {
        window.close();
    }
    else if (scancode == sf::Keyboard::Scancode::Up)
    {
        if (!availableSaveFiles.empty())
        {
            selectedSaveIndex = (selectedSaveIndex - 1 + static_cast<int>(availableSaveFiles.size())) %
                                static_cast<int>(availableSaveFiles.size());
        }
    }
    else if (scancode == sf::Keyboard::Scancode::Down)
    {
        if (!availableSaveFiles.empty())
        {
            selectedSaveIndex = (selectedSaveIndex + 1) % static_cast<int>(availableSaveFiles.size());
        }
    }
    else if (scancode == sf::Keyboard::Scancode::Enter)
    {
        if (loadSelectedSaveFile())
        {
            runPhase = RunPhase::Playing;
            showPopup("Loaded save: " + selectedSaveFile.filename().string(), 1.4f);
            saveSelectionMessage.clear();
        }
        else
        {
            saveSelectionMessage = "Failed to load selected save file.";
        }
    }
}

// handle the input for exit confirm
void GameManager::handleExitConfirmInput(sf::Keyboard::Scancode scancode)
{
    if (resetConfirmPending)
    {
        if (scancode == sf::Keyboard::Scancode::Y)
        {
            resetGameState();
            exitPromptMessage.clear();
            return;
        }
        if (scancode == sf::Keyboard::Scancode::N || scancode == sf::Keyboard::Scancode::Escape)
        {
            resetConfirmPending = false;
            runPhase = RunPhase::Playing;
            exitPromptMessage.clear();
            showPopup("Reset cancelled", 1.0f);
            return;
        }
        return;
    }

    if (scancode == sf::Keyboard::Scancode::Y)
    {
        if (saveCurrentGameToSelectedFile())
        {
            window.close();
        }
        else
        {
            exitPromptMessage = "Save failed. Press N to quit without saving or Esc to return.";
        }
    }
    else if (scancode == sf::Keyboard::Scancode::N)
    {
        window.close();
    }
    else if (scancode == sf::Keyboard::Scancode::R)
    {
        resetConfirmPending = true;
        exitPromptMessage = "Reset game to starting state? Y = reset, N/Esc = cancel";
    }
    else if (scancode == sf::Keyboard::Scancode::Escape)
    {
        resetConfirmPending = false;
        runPhase = RunPhase::Playing;
        exitPromptMessage.clear();
    }
}

// gameplay input handle, only on during normal play
void GameManager::handlePlayingKeyInput(sf::Keyboard::Scancode scancode)
{
    // while in battle, all key input goes to the battle overlay only
    if (activeBattle)
    {
        activeBattle->handleKey(scancode);
        return;
    }

    if (activeDialogue.open)
    {
        if (scancode == sf::Keyboard::Scancode::Up)
        {
            stepDialogueSelection(-1);
        }
        else if (scancode == sf::Keyboard::Scancode::Down)
        {
            stepDialogueSelection(1);
        }
        else if (scancode == sf::Keyboard::Scancode::Enter)
        {
            confirmDialogueSelection();
        }
        else if (scancode == sf::Keyboard::Scancode::Escape)
        {
            closeDialogue();
        }
        return;
    }

    if (scancode == sf::Keyboard::Scancode::Tab)
    {
        if (inventoryOpen || shopOpen)
        {
            showPopup("Close inventory/shop before editor mode", 1.4f);
            return;
        }

        mapEditorEnabled = !mapEditorEnabled;
        if (mapEditorEnabled)
        {
            showPopup("Map editor on | Brush: " + getEditorBrushName(), 1.6f);
        }
        else
        {
            showPopup("Map editor off", 1.0f);
        }
        return;
    }

    if (mapEditorEnabled && handleMapEditorKeyInput(scancode))
    {
        return;
    }

    // escape closes ui first, if no ui open then ask to save/quit
    if (scancode == sf::Keyboard::Scancode::Escape)
    {
        if (inventoryOpen)
        {
            inventoryOpen = false;
            showPopup("Inventory closed", 0.9f);
        }
        else if (shopOpen)
        {
            shopOpen = false;
            showPopup("Shop closed", 0.9f);
        }
        else
        {
            runPhase = RunPhase::ConfirmExitSave;
            resetConfirmPending = false;
            exitPromptMessage = "Pause menu: R = reset game, Y = save and quit, N = quit, Esc = cancel";
        }
    }
    else if (scancode == sf::Keyboard::Scancode::F5)
    {
        if (saveCurrentGameToSelectedFile())
        {
            showPopup("Game saved", 0.9f);
        }
        else
        {
            showPopup("Save failed", 1.1f);
        }
    }
    else if (scancode == sf::Keyboard::Scancode::E)
    {
        inventoryOpen = !inventoryOpen;
        if (inventoryOpen)
        {
            selectedInventoryIndex = 0;
            showPopup("Inventory opened", 0.9f);
        }
        else
        {
            showPopup("Inventory closed", 0.9f);
        }
    }
    else if (scancode == sf::Keyboard::Scancode::F)
    {
        if (shopOpen)
        {
            shopOpen = false;
            showPopup("Shop closed", 0.9f);
        }
        else if (!inventoryOpen)
        {
            tryInteraction();
        }
    }
    else if (scancode == sf::Keyboard::Scancode::Up)
    {
        // Inventory takes priority. If it's open, shop scrolling is locked.
        if (inventoryOpen)
        {
            if (selectedInventoryIndex > 0)
            {
                --selectedInventoryIndex;
            }
        }
        else if (shopOpen)
        {
            if (selectedShopIndex > 0)
            {
                --selectedShopIndex;
            }
        }
    }
    else if (scancode == sf::Keyboard::Scancode::Down)
    {
        // Inventory takes priority. If it's open, shop scrolling is locked.
        if (inventoryOpen)
        {
            const auto items = getInventoryItemIds();
            if (!items.empty() && selectedInventoryIndex < static_cast<int>(items.size()) - 1)
            {
                ++selectedInventoryIndex;
            }
        }
        else if (shopOpen)
        {
            const auto shopItems = getShopItemIds();
            if (!shopItems.empty() && selectedShopIndex < static_cast<int>(shopItems.size()) - 1)
            {
                ++selectedShopIndex;
            }
        }
    }
    else if (scancode == sf::Keyboard::Scancode::S && shopOpen && inventoryOpen)
    {
        trySellSelectedItem();
    }
    else if (scancode == sf::Keyboard::Scancode::Enter)
    {
        if (inventoryOpen)
        {
            equipSelectedInventoryItem();
        }
        else if (shopOpen)
        {
            tryBuyShopItem();
        }
    }
}

// handle player movement input + wild battle checks
void GameManager::handleInput()
{
    // no movement while menus are up
    if (inventoryOpen || shopOpen || activeBattle || activeDialogue.open)
    {
        return;
    }

    
    // simple cooldown so movement doesn't spam
    timeSinceLastMove += 0.016f;
    
    if (timeSinceLastMove < MOVE_COOLDOWN)
    {
        return;
    }
    

    int dx = 0, dy = 0;
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::W) || 
        sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Up))
    {
        dy = -1;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::S) || 
             sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Down))
    {
        dy = 1;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::A) || 
             sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Left))
    {
        dx = -1;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::D) || 
             sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Right))
    {
        dx = 1;
    }
    
    if (dx != 0 || dy != 0)
    {
        if (mapController.movePlayer(dx, dy, mapEditorEnabled))
        {
            timeSinceLastMove = 0.0f;

            if (!mapEditorEnabled &&
                map.isEnemySpawnTileAtPosition(map.getPlayerChunkX(), map.getPlayerChunkY(), map.getPlayerTileX(), map.getPlayerTileY()) &&
                mapPlayer.diceRoll(8) == 1)
            {
                startWildBattle();
            }
        }
    }
}

// update game state, used for popup timers and battle end checks
void GameManager::update(float deltaTime)
{
    // popup expires over time
    if (popupTimeRemaining > 0.0f)
    {
        popupTimeRemaining -= deltaTime;
        if (popupTimeRemaining <= 0.0f)
        {
            popupTimeRemaining = 0.0f;
            popupMessage.clear();
        }
    }

    if (activeBattle && activeBattle->shouldClose())
    {
        bool playerDefeated = (mapPlayer.getHp() <= 0);
        
        showPopup(activeBattle->getEndMessage(), 1.4f);
        activeBattle.reset();

        // stop battle music and resume background music
        if (battleMusic.getStatus() == sf::SoundSource::Status::Playing)
        {
            battleMusic.stop();
        }
        if (musicReady && backgroundMusic.getStatus() != sf::SoundSource::Status::Playing)
        {
            backgroundMusic.play();
        }

        // Handle death state
        if (playerDefeated)
        {
            if (!selectedSaveFile.empty())
            {
                loadSaveFile(selectedSaveFile);
                showPopup("Defeated. Loaded last save.", 2.0f);
            }
            else
            {
                resetGameState(); // This automatically pulls the _reset.json file
                showPopup("Defeated. Game reset.", 2.0f);
            }
        }
    }
}

// render the current game state to the window
void GameManager::render()
{
    // each run phase has its own ui flow
    if (runPhase == RunPhase::SaveSelection)
    {
        renderSaveSelectionScreen();
        return;
    }

    if (runPhase == RunPhase::ConfirmExitSave)
    {
        renderMapViewport();
        renderClientViewport();
        renderExitPrompt();
        return;
    }

    renderMapViewport();
    renderClientViewport();
    if (activeBattle)
    {
        renderBattleOverlay();
    }
    if (activeDialogue.open)
    {
        renderDialogueOverlay();
    }
    renderPopup();
}

// top part of window is world view
void GameManager::renderMapViewport()
{
    // top part of window is world view
    mapController.render(window, WINDOW_WIDTH, WINDOW_HEIGHT, MAP_VIEWPORT_HEIGHT_RATIO);
}

// bottom part of window is for controls, stats, inventory, shop, etc
void GameManager::renderClientViewport()
{
    // bottom panel for controls/stats/inventory/shop
    const float panelTop = static_cast<float>(WINDOW_HEIGHT) * MAP_VIEWPORT_HEIGHT_RATIO;
    const float panelHeight = static_cast<float>(WINDOW_HEIGHT) - panelTop;

    sf::RectangleShape panelBg({static_cast<float>(WINDOW_WIDTH), panelHeight});
    panelBg.setPosition({0.0f, panelTop});
    panelBg.setFillColor(sf::Color(18, 24, 30));
    window.draw(panelBg);

    sf::RectangleShape separator({static_cast<float>(WINDOW_WIDTH), 2.0f});
    separator.setPosition({0.0f, panelTop});
    separator.setFillColor(sf::Color(140, 160, 180));
    window.draw(separator);

    if (font.getInfo().family.empty())
    {
        return;
    }

    const std::string controlsText =
        mapEditorEnabled
        ? "Editor: 1 Grass 2 Water 3 Sand 4 Rock 5 Wall 6 Shop 7 NPC 8 Spawn | Place: F | Erase: R | Save: F6 | Exit: Tab"
        : "Move: WASD/Arrows | Interact: F | Inventory: E | Equip: Enter | Save: F5 | Editor: Tab";

    sf::Text controls(font, controlsText, 18);
    controls.setFillColor(sf::Color(220, 228, 238));
    controls.setPosition({20.0f, panelTop + 10.0f});
    window.draw(controls);

    sf::Text status(font,
        "Gold: " + std::to_string(mapPlayer.getGold()) +
        "   Equipped Item ID: " + std::to_string(mapPlayer.getEquippedItemID()) +
        (mapEditorEnabled ? ("   Editor Brush: " + getEditorBrushName()) : ""),
        18);
    status.setFillColor(sf::Color(200, 212, 226));
    status.setPosition({20.0f, panelTop + 38.0f});
    window.draw(status);

    if (inventoryOpen)
    {
        renderInventoryPanel(panelTop, panelHeight);
    }

    if (shopOpen)
    {
        renderShopPanel(panelTop, panelHeight);
    }
}

// full screen list of save files
void GameManager::renderSaveSelectionScreen()
{
    // full screen list of save files
    sf::RectangleShape bg({static_cast<float>(WINDOW_WIDTH), static_cast<float>(WINDOW_HEIGHT)});
    bg.setFillColor(sf::Color(17, 25, 35));
    window.draw(bg);

    if (font.getInfo().family.empty())
    {
        return;
    }

    sf::Text title(font, "Select Save File", 42);
    title.setFillColor(sf::Color(236, 244, 255));
    title.setPosition({54.0f, 42.0f});
    window.draw(title);

    sf::Text help(font, "Use Up/Down to choose, Enter to load, Esc to quit", 22);
    help.setFillColor(sf::Color(198, 214, 233));
    help.setPosition({54.0f, 96.0f});
    window.draw(help);

    float y = 160.0f;
    for (int i = 0; i < static_cast<int>(availableSaveFiles.size()); ++i)
    {
        const bool selected = (i == selectedSaveIndex);
        sf::RectangleShape rowBg({static_cast<float>(WINDOW_WIDTH) - 108.0f, 40.0f});
        rowBg.setPosition({54.0f, y - 6.0f});
        rowBg.setFillColor(selected ? sf::Color(46, 72, 100) : sf::Color(28, 40, 55));
        rowBg.setOutlineColor(selected ? sf::Color(154, 198, 255) : sf::Color(68, 92, 120));
        rowBg.setOutlineThickness(1.0f);
        window.draw(rowBg);

        const std::string label = std::to_string(i + 1) + ". " + availableSaveFiles[static_cast<std::size_t>(i)].filename().string();
        sf::Text fileName(font, label, 22);
        fileName.setFillColor(selected ? sf::Color::White : sf::Color(214, 227, 243));
        fileName.setPosition({68.0f, y});
        window.draw(fileName);
        y += 52.0f;
    }

    if (!saveSelectionMessage.empty())
    {
        sf::Text message(font, saveSelectionMessage, 20);
        message.setFillColor(sf::Color(255, 187, 187));
        message.setPosition({54.0f, static_cast<float>(WINDOW_HEIGHT) - 56.0f});
        window.draw(message);
    }
}

//overlay for confirming exit and save options
void GameManager::renderExitPrompt()
{
    // modal overlay when player tries to quit
    if (font.getInfo().family.empty())
    {
        return;
    }

    sf::RectangleShape overlay({static_cast<float>(WINDOW_WIDTH), static_cast<float>(WINDOW_HEIGHT)});
    overlay.setFillColor(sf::Color(0, 0, 0, 110));
    window.draw(overlay);

    sf::RectangleShape panel({720.0f, 220.0f});
    panel.setOrigin({panel.getSize().x * 0.5f, panel.getSize().y * 0.5f});
    panel.setPosition({static_cast<float>(WINDOW_WIDTH) * 0.5f, static_cast<float>(WINDOW_HEIGHT) * 0.5f});
    panel.setFillColor(sf::Color(18, 24, 34, 245));
    panel.setOutlineColor(sf::Color(220, 230, 240));
    panel.setOutlineThickness(2.0f);
    window.draw(panel);

    sf::Text title(font, "Exit Game", 36);
    title.setFillColor(sf::Color::White);
    title.setPosition({panel.getPosition().x - 320.0f, panel.getPosition().y - 76.0f});
    window.draw(title);

    sf::Text message(font, exitPromptMessage, 22);
    message.setFillColor(sf::Color(220, 228, 238));
    message.setPosition({panel.getPosition().x - 320.0f, panel.getPosition().y - 20.0f});
    window.draw(message);

    sf::Text actions(font, "Y: save and quit    N: quit without saving    Esc: cancel", 20);
    actions.setFillColor(sf::Color(190, 205, 222));
    actions.setPosition({panel.getPosition().x - 320.0f, panel.getPosition().y + 42.0f});
    window.draw(actions);
}

// inventory panel on left side of client viewport
void GameManager::renderInventoryPanel(float panelTop, float panelHeight)
{
    // inventory list with current selected item
    sf::RectangleShape invBg({620.0f, panelHeight - 24.0f});
    invBg.setPosition({16.0f, panelTop + 12.0f});
    invBg.setFillColor(sf::Color(12, 16, 24, 240));
    invBg.setOutlineColor(sf::Color(205, 218, 234));
    invBg.setOutlineThickness(2.0f);
    window.draw(invBg);

    std::string titleText = shopOpen 
        ? "Inventory (Up/Down: select, Enter: equip, S: sell, E: close)" 
        : "Inventory (Up/Down to select, Enter to equip, E to close)";
    
    // Moved title up slightly
    sf::Text title(font, titleText, 20);
    title.setFillColor(sf::Color::White);
    title.setPosition({28.0f, panelTop + 18.0f}); 
    window.draw(title);

    const auto itemIds = getInventoryItemIds();
    if (itemIds.empty())
    {
        sf::Text empty(font, "No items in inventory.", 18);
        empty.setFillColor(sf::Color(215, 215, 215));
        empty.setPosition({28.0f, panelTop + 46.0f});
        window.draw(empty);
        return;
    }

    if (selectedInventoryIndex >= static_cast<int>(itemIds.size()))
    {
        selectedInventoryIndex = static_cast<int>(itemIds.size()) - 1;
    }

    // Tighter spacing and higher start offset to avoid bottom clipping
    const float itemSpacing = 22.0f;
    int itemsPerColumn = (static_cast<int>(itemIds.size()) + 1) / 2;

    for (int i = 0; i < static_cast<int>(itemIds.size()); ++i)
    {
        const int itemId = itemIds[i];
        const std::string marker = (i == selectedInventoryIndex) ? "> " : "  ";
        const int quantity = mapPlayer.getInventory().getQuantity(itemId);
        const std::string line = marker + std::to_string(itemId) + ") " +
                                 itemRegistry.getItemName(itemId) + " x" +
                                 std::to_string(quantity);

        sf::Text itemText(font, line, 18);
        itemText.setFillColor(i == selectedInventoryIndex ? sf::Color(255, 238, 145)
                                                          : sf::Color(220, 226, 235));
        
        // Math for 2 columns: top-to-bottom, then left-to-right
        int col = i / itemsPerColumn;
        int row = i % itemsPerColumn;
        
        // Offset the second column by 300 pixels
        float xOffset = col * 300.0f;
        itemText.setPosition({28.0f + xOffset, panelTop + 46.0f + (row * itemSpacing)});
        window.draw(itemText);
    }
}

//shop panel on right side of client viewport, shows current item for sale and price
void GameManager::renderShopPanel(float panelTop, float panelHeight)
{
    sf::RectangleShape shopBg({540.0f, panelHeight - 24.0f});
    shopBg.setPosition({WINDOW_WIDTH - 556.0f, panelTop + 12.0f});
    shopBg.setFillColor(sf::Color(24, 20, 14, 240));
    shopBg.setOutlineColor(sf::Color(255, 216, 140));
    shopBg.setOutlineThickness(2.0f);
    window.draw(shopBg);

    // Moved title up slightly
    sf::Text title(font, "Shopkeeper (Up/Down: Select, Enter: Buy)", 20);
    title.setFillColor(sf::Color(255, 232, 170));
    title.setPosition({WINDOW_WIDTH - 544.0f, panelTop + 18.0f});
    window.draw(title);

    if (!shopNpc) return;

    // Count valid items
    int itemCount = 0;
    InventoryNode* countNode = shopNpc->getInventory().getHead();
    while (countNode != nullptr) {
        if (countNode->item && countNode->quantity > 0) {
            itemCount++;
        }
        countNode = countNode->next;
    }

    if (itemCount == 0) {
        sf::Text empty(font, "Sold out!", 18);
        empty.setPosition({WINDOW_WIDTH - 544.0f, panelTop + 46.0f});
        window.draw(empty);
        return;
    }

    // Tighter spacing and higher start offset
    const float itemSpacing = 22.0f;
    int itemsPerColumn = (itemCount + 1) / 2; 
    int i = 0;
    
    // Iterate and render
    InventoryNode* current = shopNpc->getInventory().getHead();
    while (current != nullptr) {
        if (current->item && current->quantity > 0) {
            bool isSelected = (i == selectedShopIndex);
            std::string marker = isSelected ? "> " : "  ";
            
            std::string line = marker + current->item->getName() + 
                               " (" + std::to_string(current->item->getValue()) + "G)";

            sf::Text itemText(font, line, 18);
            itemText.setFillColor(isSelected ? sf::Color(255, 238, 145) : sf::Color(220, 226, 235));
            
            int col = i / itemsPerColumn;
            int row = i % itemsPerColumn;
            
            // Offset the second column by 260 pixels
            float xOffset = col * 260.0f; 
            itemText.setPosition({WINDOW_WIDTH - 544.0f + xOffset, panelTop + 46.0f + (row * itemSpacing)});
            
            window.draw(itemText);
            i++;
        }
        current = current->next;
    }
}

// small popup messager
void GameManager::renderPopup()
{
    // short status popup near map area
    if (popupMessage.empty() || popupTimeRemaining <= 0.0f)
    {
        return;
    }

    if (font.getInfo().family.empty())
    {
        return;
    }

    sf::RectangleShape popupBg({560.0f, 96.0f});
    popupBg.setOrigin({popupBg.getSize().x * 0.5f, popupBg.getSize().y * 0.5f});
    popupBg.setPosition({static_cast<float>(WINDOW_WIDTH) * 0.5f,
                         static_cast<float>(WINDOW_HEIGHT) * MAP_VIEWPORT_HEIGHT_RATIO - 56.0f});
    popupBg.setFillColor(sf::Color(15, 18, 24, 232));
    popupBg.setOutlineColor(sf::Color(204, 214, 225));
    popupBg.setOutlineThickness(2.0f);
    window.draw(popupBg);

    sf::Text popupText(font, popupMessage, 24);
    popupText.setFillColor(sf::Color::White);
    popupText.setPosition({popupBg.getPosition().x - (popupBg.getSize().x * 0.5f) + 24.0f,
                           popupBg.getPosition().y - 16.0f});
    window.draw(popupText);
}

// overlay for npc dialogue
void GameManager::renderDialogueOverlay()
{
    if (!activeDialogue.open || font.getInfo().family.empty())
    {
        return;
    }

    const ActiveDialogueNode* node = getActiveDialogueNodeById(activeDialogue.currentNodeId);
    if (!node)
    {
        return;
    }

    sf::RectangleShape overlay({static_cast<float>(WINDOW_WIDTH), static_cast<float>(WINDOW_HEIGHT)});
    overlay.setFillColor(sf::Color(0, 0, 0, 120));
    window.draw(overlay);

    sf::RectangleShape panel({1020.0f, 320.0f});
    panel.setOrigin({panel.getSize().x * 0.5f, panel.getSize().y * 0.5f});
    panel.setPosition({static_cast<float>(WINDOW_WIDTH) * 0.5f, static_cast<float>(WINDOW_HEIGHT) * 0.5f});
    panel.setFillColor(sf::Color(18, 24, 34, 245));
    panel.setOutlineColor(sf::Color(220, 230, 240));
    panel.setOutlineThickness(2.0f);
    window.draw(panel);

    sf::Text title(font, activeDialogue.speakerName.empty() ? "NPC" : activeDialogue.speakerName, 28);
    title.setFillColor(sf::Color(255, 236, 170));
    title.setPosition({panel.getPosition().x - 480.0f, panel.getPosition().y - 126.0f});
    window.draw(title);

    sf::Text body(font, node->npcText, 22);
    body.setFillColor(sf::Color::White);
    body.setPosition({panel.getPosition().x - 480.0f, panel.getPosition().y - 72.0f});
    window.draw(body);

    float y = panel.getPosition().y + 8.0f;
    for (int i = 0; i < static_cast<int>(node->options.size()); ++i)
    {
        const auto& option = node->options[static_cast<std::size_t>(i)];
        const bool selected = (i == activeDialogue.selectedOptionIndex);

        sf::RectangleShape optionBg({960.0f, 34.0f});
        optionBg.setPosition({panel.getPosition().x - 480.0f, y - 4.0f});
        optionBg.setFillColor(selected ? sf::Color(56, 82, 118) : sf::Color(28, 36, 48));
        optionBg.setOutlineColor(selected ? sf::Color(154, 198, 255) : sf::Color(68, 92, 120));
        optionBg.setOutlineThickness(1.0f);
        window.draw(optionBg);

        sf::Text optionText(font, std::to_string(i) + ". " + option.text, 20);
        optionText.setFillColor(selected ? sf::Color::White : sf::Color(214, 227, 243));
        optionText.setPosition({panel.getPosition().x - 464.0f, y});
        window.draw(optionText);
        y += 40.0f;
    }

    sf::Text hint(font, "Up/Down: choose   Enter: select   Esc: close", 18);
    hint.setFillColor(sf::Color(190, 205, 222));
    hint.setPosition({panel.getPosition().x - 480.0f, panel.getPosition().y + 122.0f});
    window.draw(hint);
}

// starting the npc dialogue, returns false if no dialogue available, true if dialogue started (even if just default text)
bool GameManager::beginNpcDialogue(int entityId, const OverworldMap::EntityMetadata& metadata)
{
    activeDialogue = ActiveDialogue{};
    activeDialogue.open = true;
    activeDialogue.entityId = entityId;
    activeDialogue.speakerName = metadata.npcName.empty() ? (metadata.shopNpcName.empty() ? "NPC" : metadata.shopNpcName)
                                                          : metadata.npcName;

    if (!metadata.dialogueTree.empty())
    {
        for (const auto& node : metadata.dialogueTree)
        {
            ActiveDialogueNode copy{};
            copy.id = node.id;
            copy.npcText = node.npcText;
            for (const auto& option : node.options)
            {
                copy.options.push_back({option.text, option.nextNodeId});
            }
            activeDialogue.nodes.push_back(std::move(copy));
        }

        activeDialogue.currentNodeId = activeDialogue.nodes.front().id;
        return true;
    }

    ActiveDialogueNode node{};
    node.id = 1;
    node.npcText = !metadata.dialogue.empty() ? metadata.dialogue : metadata.npcDescription;
    if (node.npcText.empty())
    {
        node.npcText = "...";
    }

    if (!metadata.dialogue.empty())
    {
        node.options.push_back({"Continue", -1});
    }

    activeDialogue.nodes.push_back(std::move(node));
    activeDialogue.currentNodeId = 1;
    return true;
}

// helper to find active dialogue node by id, returns nullptr if not found
const GameManager::ActiveDialogueNode* GameManager::getActiveDialogueNodeById(int nodeId) const
{
    for (const auto& node : activeDialogue.nodes)
    {
        if (node.id == nodeId)
        {
            return &node;
        }
    }
    return nullptr;
}

// move dialogue selection up/down, wraps around
void GameManager::stepDialogueSelection(int delta)
{
    const ActiveDialogueNode* node = getActiveDialogueNodeById(activeDialogue.currentNodeId);
    if (!node || node->options.empty())
    {
        return;
    }

    const int optionCount = static_cast<int>(node->options.size());
    activeDialogue.selectedOptionIndex = (activeDialogue.selectedOptionIndex + delta + optionCount) % optionCount;
}

//confirm dialogue selection, handles moving to another node or triggering actions based on the values of the nextNodeId
void GameManager::confirmDialogueSelection()
{
    const ActiveDialogueNode* node = getActiveDialogueNodeById(activeDialogue.currentNodeId);
    if (!node)
    {
        closeDialogue();
        return;
    }

    if (node->options.empty())
    {
        closeDialogue();
        return;
    }

    if (activeDialogue.selectedOptionIndex < 0 ||
        activeDialogue.selectedOptionIndex >= static_cast<int>(node->options.size()))
    {
        activeDialogue.selectedOptionIndex = 0;
    }

    const auto& option = node->options[static_cast<std::size_t>(activeDialogue.selectedOptionIndex)];
    if (option.nextNodeId < -1)
    {
        int actionCode = option.nextNodeId;
        closeDialogue();
        
        if (actionCode == -2)
        {
            startWildBattle(-1); // Trigger random battle
        }
        else if (actionCode <= -100)
        {
            startWildBattle(std::abs(actionCode) - 100); // Trigger specific battle ID
        }
        return;
    }
    else if (option.nextNodeId == -1)
    {
        closeDialogue();
        return;
    }

    const ActiveDialogueNode* nextNode = getActiveDialogueNodeById(option.nextNodeId);
    if (!nextNode)
    {
        closeDialogue();
        return;
    }

    activeDialogue.currentNodeId = nextNode->id;
    activeDialogue.selectedOptionIndex = 0;
}

// closes dialogue and resets state
void GameManager::closeDialogue()
{
    activeDialogue = ActiveDialogue{};
}

// trys to interact with any entity on same tile, then 4 adjacent, shows popup if nothing to interact with
void GameManager::tryInteraction()
{
    // check tile player is on first
    const int chunkX = map.getPlayerChunkX();
    const int chunkY = map.getPlayerChunkY();
    const int tileX = map.getPlayerTileX();
    const int tileY = map.getPlayerTileY();

    auto interactWithEntity = [this](const OverworldMap::MapEntity* entity) -> bool
    {
        if (!entity)
        {
            return false;
        }

        const bool isShop = (entity->type == "shop");
        if (isShop)
        {
            if (!shopNpc)
            {
                showPopup("Shop unavailable (" + std::to_string(entity->id) + ")", 1.2f);
                return true;
            }

            if (const OverworldMap::EntityMetadata* metadata = map.getEntityMetadata(entity->id))
            {
                // Apply shop listing from metadata string format: id:qty,id:qty
                shopNpc->getInventory().clear();
                if (!metadata->shopInventory.empty())
                {
                    std::stringstream stream(metadata->shopInventory);
                    std::string entry;
                    while (std::getline(stream, entry, ','))
                    {
                        std::stringstream pairStream(entry);
                        std::string idToken;
                        std::string qtyToken;
                        if (!std::getline(pairStream, idToken, ':') || !std::getline(pairStream, qtyToken, ':'))
                        {
                            continue;
                        }

                        try
                        {
                            const int itemId = std::stoi(idToken);
                            const int quantity = std::stoi(qtyToken);
                            if (quantity > 0)
                            {
                                shopNpc->addItemToInventory(itemId, itemRegistry, quantity);
                            }
                        }
                        catch (...)
                        {
                            // ignore malformed entries and keep parsing
                        }
                    }
                }

                const std::string shopName = metadata->shopName.empty() ? "Shop" : metadata->shopName;
                shopOpen = true;
                showPopup(shopName + " opened: Press Enter to buy", 1.6f);
                return true;
            }

            shopOpen = true;
            showPopup("Shop opened: Press Enter to buy", 1.6f);
            return true;
        }

        if (entity->type == "npc")
        {
            if (const OverworldMap::EntityMetadata* metadata = map.getEntityMetadata(entity->id))
            {
                if (!metadata->dialogueTree.empty() || !metadata->dialogue.empty() || !metadata->npcDescription.empty())
                {
                    if (beginNpcDialogue(entity->id, *metadata))
                    {
                        return true;
                    }
                    return true;
                }
            }
        }

        showPopup("You interact with " + entity->type + " #" + std::to_string(entity->id));
        return true;
    };

    const OverworldMap::MapEntity* onTile = map.getEntityAtPosition(chunkX, chunkY, tileX, tileY);
    if (interactWithEntity(onTile))
    {
        return;
    }

    // then check 4 tiles around player
    const int offsets[4][2] = {
        {0, -1},
        {0, 1},
        {-1, 0},
        {1, 0}
    };

    for (const auto& offset : offsets)
    {
        const OverworldMap::MapEntity* entity =
            map.getEntityAtPosition(chunkX, chunkY, tileX + offset[0], tileY + offset[1]);
        if (interactWithEntity(entity))
        {
            return;
        }
    }

    showPopup("There is nothing to interact with.", 1.4f);
}

// helper to show a temporary popup message, overwrites old message if still active
void GameManager::showPopup(const std::string& message, float seconds)
{
    // overwrites old popup with newest one
    popupMessage = message;
    popupTimeRemaining = seconds;
}

// loads +starts music, returns true if loaded
bool GameManager::initializeBackgroundMusic()
{
    // try load + loop bg music (not fatal if this fails)
    const std::filesystem::path songPath = resolveAssetPath("Main Menu.wav");
    if (!backgroundMusic.openFromFile(songPath.string()))
    {
        std::cerr << "Warning: failed to load background music from " << songPath << std::endl;
        musicReady = false;
        return false;
    }

    backgroundMusic.setLooping(true);
    backgroundMusic.setVolume(40.0f);
    backgroundMusic.play();

    // load battle music (also not fatal)
    const std::filesystem::path battleMusicPath = resolveAssetPath("Encounter.wav");
    if (!battleMusic.openFromFile(battleMusicPath.string()))
    {
        std::cerr << "Warning: failed to load battle music from " << battleMusicPath << std::endl;
    }
    else
    {
        battleMusic.setLooping(true);
        battleMusic.setVolume(40.0f);
    }

    musicReady = true;
    return true;
}

// initializes the item registry with all items used in the game, including those sold by shops and found in battles
void GameManager::initializeMapGameplayState()
{
    // register base items used in map gameplay
    auto sword = std::make_shared<Item>(0, "Iron Sword", "Sword of Iron", 50, false);
    auto potion = std::make_shared<Item>(1, "Health Potion", "Potion of Health", 10, true);
    auto shield = std::make_shared<Item>(2, "Shield", "Blocks attacks", 100, false);
    auto waterBottle = std::make_shared<Item>(3, "Plastic Water Bottle", "", 10, false);
    auto RustySword = std::make_shared<Item>(4, "Rusty Sword", "", 10, false);
    auto SheepskinS = std::make_shared<Item>(5, "Sheepskin Shield", "", 30, false);
    auto BollD = std::make_shared<Item>(32, "Bollock Dagger", "", 50, false);
    auto cloth = std::make_shared<Item>(6, "Cloth Cloak", "", 40, false);
    auto TurkishDelight = std::make_shared<Item>(7, "Turkish Delight", "", 2, true);
    auto Bread = std::make_shared<Item>(8, "Bread", "", 6, true);
    auto CheeseC = std::make_shared<Item>(9, "Chesse and Crackers", "", 3, true);
    auto mace = std::make_shared<Item>(10, "Iron Mace", "", 80, false);
    auto breastPlate = std::make_shared<Item>(11, "Bronze breastplate", "", 80, false);
    auto Hanger = std::make_shared<Item>(12, "Hanger Sword", "", 60, false);
    auto Ring = std::make_shared<Item>(13, "Sacred Ring", "", 100, false);
    auto Salmon = std::make_shared<Item>(14, "Salmon", "", 15, true);
    auto Crab = std::make_shared<Item>(15, "Crab", "", 10, true);
    auto sushi = std::make_shared<Item>(16, "Sushi", "", 20, true);
    auto Frost = std::make_shared<Item>(17, "Frostbite", "", 200, false);
    auto chain = std::make_shared<Item>(18, "Chainmail Shirt", "", 120, false);
    auto fisher = std::make_shared<Item>(19, "Fisher hat", "", 40, false);
    auto sKey = std::make_shared<Item>(20, "Sliver Key", "", 500, false);
    auto chainMG = std::make_shared<Item>(21, "Chainmail Gauntlet", "", 100, false);
    auto Wall = std::make_shared<Item>(22, "Walleye", "", 15, true);
    auto cod = std::make_shared<Item>(23, "Cod", "", 10, true);
    auto Emb = std::make_shared<Item>(24, "EmberFang", "", 220, false);
    auto templar = std::make_shared<Item>(25, "Templar Sword", "", 200, false);
    auto IronB = std::make_shared<Item>(26, "Iron BreastPlate", "", 180, false);
    auto ChainH = std::make_shared<Item>(27, "Chainmail hood", "", 100, false);
    auto HeaterS = std::make_shared<Item>(28, "Heater Shield", "", 80, false);
    auto Watermelon = std::make_shared<Item>(29, "Watermelon", "", 5, true);
    auto Eggs = std::make_shared<Item>(30, "Eggs", "", 15, true);
    auto HighPotion = std::make_shared<Item>(31, "High Potion", "", 100, true);


    itemRegistry.setItem(0, sword);
    itemRegistry.setItem(1, potion);
    itemRegistry.setItem(2, shield);
    itemRegistry.setItem(3, waterBottle);
    itemRegistry.setItem(4, RustySword);
    itemRegistry.setItem(5, SheepskinS);
    itemRegistry.setItem(6, cloth);
    itemRegistry.setItem(7, TurkishDelight);
    itemRegistry.setItem(8, Bread);
    itemRegistry.setItem(9, CheeseC);
    itemRegistry.setItem(10, mace);
    itemRegistry.setItem(11, breastPlate);
    itemRegistry.setItem(12, Hanger);
    itemRegistry.setItem(13, Ring);
    itemRegistry.setItem(14, Salmon);
    itemRegistry.setItem(15, Crab);
    itemRegistry.setItem(16, sushi);
    itemRegistry.setItem(17, Frost);
    itemRegistry.setItem(18, chain);
    itemRegistry.setItem(19, fisher);
    itemRegistry.setItem(20, sKey);
    itemRegistry.setItem(21, chainMG);
    itemRegistry.setItem(22, Wall);
    itemRegistry.setItem(23, cod);
    itemRegistry.setItem(24, Emb);
    itemRegistry.setItem(25, templar);
    itemRegistry.setItem(26, IronB);
    itemRegistry.setItem(27, ChainH);
    itemRegistry.setItem(28, HeaterS);
    itemRegistry.setItem(29, Watermelon);
    itemRegistry.setItem(30, Eggs);
    itemRegistry.setItem(31, HighPotion);
    itemRegistry.setItem(32, BollD);
    
    mapShop = std::make_unique<store>("Map Shop", itemRegistry);
    shopNpc = std::make_unique<npc>("Shopkeeper", "A local merchant", 50, 8, 4, 3, 1000, 6, 7, false, 0);
    shopNpc->addItemToInventory(1, itemRegistry, 30);

}

// applies default player state to map player, used for new saves and as fallback if save file is missing data
void GameManager::applyDefaultPlayerState()
{
    // fallback player state for new saves / missing player data
    mapPlayer.setHp(50);
    mapPlayer.setGold(100);
    mapPlayer.setXp(0);
    mapPlayer.getInventory().clear();

    mapPlayer.addItemToInventory(0, itemRegistry, 1);
    mapPlayer.addItemToInventory(1, itemRegistry, 3);
    mapPlayer.addItemToInventory(2, itemRegistry, 1);
}

// builds a PlayerState struct from the current runtime player data, embedding into save files
OverworldMap::PlayerState GameManager::buildPlayerStateForSave()
{
    // copy runtime player + inventory into save struct
    OverworldMap::PlayerState state;
    state.hasData = true;
    state.hp = mapPlayer.getHp();
    state.gold = mapPlayer.getGold();
    state.xp = mapPlayer.getXp();
    state.equippedItemId = mapPlayer.getEquippedItemID();

    InventoryNode* current = mapPlayer.getInventory().getHead();
    while (current != nullptr)
    {
        if (current->item && current->quantity > 0)
        {
            OverworldMap::PlayerInventoryEntry entry;
            entry.id = current->item->getId();
            entry.quantity = current->quantity;
            entry.equipped = current->isEquipped;
            state.inventory.push_back(entry);
        }
        current = current->next;
    }

    return state;
}

// resets current state, resets using reset snapshot
void GameManager::resetGameState()
{
    if (selectedSaveFile.empty())
    {
        showPopup("Reset failed: no active save", 1.5f);
        return;
    }

    if (!restoreFromResetSnapshot(selectedSaveFile))
    {
        showPopup("Reset failed: missing reset snapshot", 1.8f);
        return;
    }

    if (!loadSaveFile(selectedSaveFile))
    {
        showPopup("Reset failed: could not reload save", 1.8f);
        return;
    }

    // clear ui states
    inventoryOpen = false;
    shopOpen = false;
    mapEditorEnabled = false;

    // return to playing phase
    runPhase = RunPhase::Playing;
    resetConfirmPending = false;

    showPopup("Game reset to starting state", 1.5f);
}

// helper to get path for reset snapshot
std::filesystem::path GameManager::getResetSnapshotPath(const std::filesystem::path& mapPath) const
{
    const std::string stem = mapPath.stem().string();
    return mapPath.parent_path() / (stem + "_reset.json");
}

// makes sure there's a reset snapshot
bool GameManager::ensureResetSnapshotExists(const std::filesystem::path& mapPath)
{
    const std::filesystem::path resetPath = getResetSnapshotPath(mapPath);
    if (std::filesystem::exists(resetPath))
    {
        return true;
    }

    std::error_code ec;
    std::filesystem::copy_file(mapPath, resetPath, std::filesystem::copy_options::none, ec);
    return !ec;
}

// restores map file from reset snapshot, returns false if snapshot missing or copy fails
bool GameManager::restoreFromResetSnapshot(const std::filesystem::path& mapPath)
{
    const std::filesystem::path resetPath = getResetSnapshotPath(mapPath);
    if (!std::filesystem::exists(resetPath))
    {
        return false;
    }

    std::error_code ec;
    std::filesystem::copy_file(resetPath, mapPath, std::filesystem::copy_options::overwrite_existing, ec);
    return !ec;
}

// applies the loaded player state
void GameManager::applyLoadedPlayerState(const OverworldMap::PlayerState& state)
{
    // restore player basics + inventory from loaded file
    mapPlayer.setHp(state.hp);
    mapPlayer.setGold(state.gold);
    mapPlayer.setXp(state.xp);
    
    // restore stamina so the player isn't trapped in an exhausted state
    mapPlayer.setStamina(1000);

    mapPlayer.getInventory().clear();
    for (const auto& entry : state.inventory)
    {
        if (entry.quantity > 0)
        {
            mapPlayer.addItemToInventory(entry.id, itemRegistry, entry.quantity);
        }
    }

    if (state.equippedItemId >= 0 && mapPlayer.hasItem(state.equippedItemId))
    {
        mapPlayer.equipItem(state.equippedItemId);
    }
}

// initializes available save files, creates default if none found, applies default player state if no player data in file
bool GameManager::initializeMapFromSaveFile()
{
    // make sure maps dir exists, and at least one save exists
    const std::filesystem::path mapsDir = resolveMapsDirectory();
    std::error_code ec;
    std::filesystem::create_directories(mapsDir, ec);
    if (ec)
    {
        saveSelectionMessage = "Failed to create Maps directory.";
        return false;
    }

    availableSaveFiles = getMapFiles(mapsDir);
    if (availableSaveFiles.empty())
    {
        applyDefaultPlayerState();
        map.setPlayerState(buildPlayerStateForSave());

        const std::filesystem::path defaultMapPath = mapsDir / "default.json";
        if (!map.saveToFile(defaultMapPath.string()))
        {
            saveSelectionMessage = "Failed to create default save file.";
            return false;
        }

        availableSaveFiles.push_back(defaultMapPath);
    }

    selectedSaveIndex = std::clamp(selectedSaveIndex, 0, static_cast<int>(availableSaveFiles.size()) - 1);
    saveSelectionMessage = "Select a save file to start.";
    return true;
}

// loads the currently selected save file, returns false if no files or load fails, applies embedded player state if present
bool GameManager::loadSelectedSaveFile()
{
    // load whichever file is highlighted in save list
    if (availableSaveFiles.empty())
    {
        return false;
    }

    selectedSaveFile = availableSaveFiles[static_cast<std::size_t>(selectedSaveIndex)];
    return loadSaveFile(selectedSaveFile);
}

// loads a save file by path, returns false if load fails, applies embedded player state if present
bool GameManager::loadSaveFile(const std::filesystem::path& path)
{
    // load map file and apply embedded player state if present
    if (!map.loadFromFile(path.string()))
    {
        return false;
    }

    const OverworldMap::PlayerState* loadedPlayerState = map.getPlayerState();
    if (loadedPlayerState && loadedPlayerState->hasData)
    {
        applyLoadedPlayerState(*loadedPlayerState);
    }
    else
    {
        applyDefaultPlayerState();
    }

    selectedSaveFile = path;
    return ensureResetSnapshotExists(path);
}

// saves current game state to currently selected file, returns false if no file selected or save fails
bool GameManager::saveCurrentGameToSelectedFile()
{
    // write latest player state back into selected map file
    if (selectedSaveFile.empty())
    {
        return false;
    }

    map.setPlayerState(buildPlayerStateForSave());
    return map.saveToFile(selectedSaveFile.string());
}

// helper to get list of item ids in player inventory with quantity > 0, used for rendering and interaction
std::vector<int> GameManager::getInventoryItemIds()
{
    // flattened list of inventory item ids with quantity > 0
    std::vector<int> ids;
    InventoryNode* current = mapPlayer.getInventory().getHead();
    while (current != nullptr)
    {
        if (current->item && current->quantity > 0)
        {
            ids.push_back(current->item->getId());
        }
        current = current->next;
    }
    return ids;
}

// helper get first avalible shop item
bool GameManager::getShopListing(int& itemId, std::string& itemName, int& itemPrice) const
{
    // pull first available shop item from npc inventory
    itemId = -1;
    itemName.clear();
    itemPrice = 0;

    if (!shopNpc)
    {
        return false;
    }

    InventoryNode* current = shopNpc->getInventory().getHead();
    while (current != nullptr)
    {
        if (current->item && current->quantity > 0)
        {
            itemId = current->item->getId();
            itemName = current->item->getName();
            itemPrice = current->item->getValue();
            return true;
        }
        current = current->next;
    }

    return false;
}

// tries to equip item, shows popup if failed
void GameManager::equipSelectedInventoryItem()
{
    // equip currently highlighted inventory entry
    const auto itemIds = getInventoryItemIds();
    if (itemIds.empty() || selectedInventoryIndex < 0 || selectedInventoryIndex >= static_cast<int>(itemIds.size()))
    {
        showPopup("No item selected", 1.0f);
        return;
    }

    const int itemId = itemIds[selectedInventoryIndex];
    const bool equipped = mapPlayer.equipItem(itemId);
    if (equipped)
    {
        showPopup("Equipped: " + itemRegistry.getItemName(itemId), 1.1f);
    }
    else
    {
        showPopup("Could not equip selected item", 1.1f);
    }
}

// tries to buy first available shop item, shows popup if failed or on success
void GameManager::tryBuyShopItem()
{
    if (!mapShop || !shopNpc) return;

    const auto shopItems = getShopItemIds();
    if (shopItems.empty() || selectedShopIndex >= shopItems.size()) {
        showPopup("Nothing selected", 1.2f);
        return;
    }

    int itemId = shopItems[selectedShopIndex];
    int price = 0;

    // Find the item directly in the NPC's inventory to get its value
    InventoryNode* current = shopNpc->getInventory().getHead();
    while (current != nullptr) {
        if (current->item && current->item->getId() == itemId) {
            price = current->item->getValue();
            break;
        }
        current = current->next;
    }

    if (mapPlayer.getGold() < price) {
        showPopup("Not enough gold!", 1.2f);
        return;
    }

    mapShop->buySomething(mapPlayer, *shopNpc, itemId, 1);
    showPopup("Purchased " + itemRegistry.getItemName(itemId), 1.2f);
}

//handles key input for editor
bool GameManager::handleMapEditorKeyInput(sf::Keyboard::Scancode scancode)
{
    if (!mapEditorEnabled)
    {
        return false;
    }

    if (scancode == sf::Keyboard::Scancode::Num1)
    {
        activeEditorBrush = EditorBrush::Grass;
        showPopup("Brush: Grass", 0.9f);
        return true;
    }
    if (scancode == sf::Keyboard::Scancode::Num2)
    {
        activeEditorBrush = EditorBrush::Water;
        showPopup("Brush: Water", 0.9f);
        return true;
    }
    if (scancode == sf::Keyboard::Scancode::Num3)
    {
        activeEditorBrush = EditorBrush::Sand;
        showPopup("Brush: Sand", 0.9f);
        return true;
    }
    if (scancode == sf::Keyboard::Scancode::Num4)
    {
        activeEditorBrush = EditorBrush::Rock;
        showPopup("Brush: Rock", 0.9f);
        return true;
    }
    if (scancode == sf::Keyboard::Scancode::Num5)
    {
        activeEditorBrush = EditorBrush::Wall;
        showPopup("Brush: Wall", 0.9f);
        return true;
    }
    if (scancode == sf::Keyboard::Scancode::Num6)
    {
        activeEditorBrush = EditorBrush::Shop;
        showPopup("Brush: Shop", 0.9f);
        return true;
    }
    if (scancode == sf::Keyboard::Scancode::Num7)
    {
        activeEditorBrush = EditorBrush::Npc;
        showPopup("Brush: NPC", 0.9f);
        return true;
    }
    if (scancode == sf::Keyboard::Scancode::Num8)
    {
        activeEditorBrush = EditorBrush::EnemySpawn;
        showPopup("Brush: Spawn Tile", 0.9f);
        return true;
    }

    if (scancode == sf::Keyboard::Scancode::F)
    {
        if (applyMapEditorBrushAtPlayer())
        {
            showPopup("Placed " + getEditorBrushName(), 1.0f);
        }
        else
        {
            showPopup("Could not place object", 1.0f);
        }
        return true;
    }

    if (scancode == sf::Keyboard::Scancode::R ||
        scancode == sf::Keyboard::Scancode::Delete ||
        scancode == sf::Keyboard::Scancode::Backspace)
    {
        if (eraseMapEditorSelectionAtPlayer())
        {
            showPopup("Tile cleared", 0.9f);
        }
        else
        {
            showPopup("Nothing to clear", 0.9f);
        }
        return true;
    }

    if (scancode == sf::Keyboard::Scancode::F6)
    {
        if (saveCurrentGameToSelectedFile())
        {
            showPopup("Map saved", 0.9f);
        }
        else
        {
            showPopup("Map save failed", 1.1f);
        }
        return true;
    }

    return false;
}

// helper to get editor brush name
std::string GameManager::getEditorBrushName() const
{
    switch (activeEditorBrush)
    {
    case EditorBrush::Grass:
        return "grass";
    case EditorBrush::Water:
        return "water";
    case EditorBrush::Sand:
        return "sand";
    case EditorBrush::Rock:
        return "rock";
    case EditorBrush::Wall:
        return "wall";
    case EditorBrush::Shop:
        return "shop";
    case EditorBrush::Npc:
        return "npc";
    case EditorBrush::EnemySpawn:
        return "enemy_spawn";
    default:
        return "rock";
    }
}

// applies editor brush at player pos
bool GameManager::applyMapEditorBrushAtPlayer()
{
    const int chunkX = map.getPlayerChunkX();
    const int chunkY = map.getPlayerChunkY();
    const int tileX = map.getPlayerTileX();
    const int tileY = map.getPlayerTileY();

    if (activeEditorBrush == EditorBrush::EnemySpawn)
    {
        // Prevent duplicate stacking in the JSON array
        if (!map.isEnemySpawnTileAtPosition(chunkX, chunkY, tileX, tileY))
        {
            map.addEnemySpawnTile(chunkX, chunkY, tileX, tileY);
        }
        return true; 
    }

    if (activeEditorBrush == EditorBrush::Grass ||
        activeEditorBrush == EditorBrush::Water ||
        activeEditorBrush == EditorBrush::Sand)
    {
        OverworldMap::TerrainType terrain = OverworldMap::TerrainType::Grass;
        if (activeEditorBrush == EditorBrush::Water)
        {
            terrain = OverworldMap::TerrainType::Water;
        }
        else if (activeEditorBrush == EditorBrush::Sand)
        {
            terrain = OverworldMap::TerrainType::Sand;
        }

        const bool updatedTerrain = map.setTileTerrain(chunkX, chunkY, tileX, tileY, terrain);
        const bool updatedPassability = map.setTilePassable(chunkX, chunkY, tileX, tileY, true);
        map.removeEntityAtPosition(chunkX, chunkY, tileX, tileY);
        return updatedTerrain && updatedPassability;
    }

    const std::string entityType = getEditorBrushName();
    const bool updatedPassability = map.setTilePassable(chunkX, chunkY, tileX, tileY, false);
    const bool updatedEntity = map.placeOrReplaceEntity(chunkX, chunkY, tileX, tileY, entityType);

    if (updatedEntity && (entityType == "shop" || entityType == "npc"))
    {
        const OverworldMap::MapEntity* placed = map.getEntityAtPosition(chunkX, chunkY, tileX, tileY);
        if (placed)
        {
            OverworldMap::EntityMetadata metadata;
            if (entityType == "shop")
            {
                metadata.shopName = "Map Shop";
                metadata.shopNpcName = "Shopkeeper";
                metadata.shopInventory = "1:10";
            }
            else
            {
                metadata.npcName = "Villager";
                metadata.npcDescription = "A local villager.";
                metadata.dialogue = "Hello there";
            }
            map.setEntityMetadata(placed->id, metadata);
        }
    }

    return updatedPassability && updatedEntity;
}

// erases any entity at player pos and resets terrain to grass + passable
bool GameManager::eraseMapEditorSelectionAtPlayer()
{
    const int chunkX = map.getPlayerChunkX();
    const int chunkY = map.getPlayerChunkY();
    const int tileX = map.getPlayerTileX();
    const int tileY = map.getPlayerTileY();

    const bool removedEntity = map.removeEntityAtPosition(chunkX, chunkY, tileX, tileY);
    const bool removedSpawn = map.removeEnemySpawnTile(chunkX, chunkY, tileX, tileY);
    const bool resetTerrain = map.setTileTerrain(chunkX, chunkY, tileX, tileY, OverworldMap::TerrainType::Grass);
    const bool resetPassability = map.setTilePassable(chunkX, chunkY, tileX, tileY, true);
    
    return removedEntity || removedSpawn || (resetTerrain && resetPassability);
}

// starts battle encounter with either random fodder enemy or specific scripted enemy based on passed id, shows popup if battle already active
void GameManager::startWildBattle(int enemyId)
{
    if (activeBattle)
    {
        return;
    }

    std::unique_ptr<fodder> enemy;

    if (enemyId == -1)
    {
        // Random enemy pool
        int roll = mapPlayer.diceRoll(4);
        switch (roll)
        {
            case 1: enemy = std::make_unique<fodder>("Slime", 18, 3, 1, 1, 500, 4, 1); break;
            case 2: enemy = std::make_unique<fodder>("Bat", 15, 4, 1, 2, 500, 5, 2); break;
            case 3: enemy = std::make_unique<fodder>("Goblin", 25, 5, 2, 1, 600, 6, 3); break;
            case 4: enemy = std::make_unique<fodder>("Googlie Mooglie", 20, 6, 1, 3, 600, 8, 4); break;
            default: enemy = std::make_unique<fodder>("Slime", 18, 3, 1, 1, 500, 4, 1); break;
        }
    }
    else
    {
        // Specific scripted enemies
        switch (enemyId)
        {
            //ultimate boss is Red Dragon
            case 1: enemy = std::make_unique<fodder>("Rogue Guard", 40, 8, 4, 3, 1000, 10, 5); break;
            case 2: enemy = std::make_unique<fodder>("Boss Slime", 80, 12, 5, 2, 1200, 25, 10); break;
            case 3: enemy = std::make_unique<fodder>("Final Boss", 150, 20, 10, 5, 2000, 50, 20); break;
            case 4: enemy = std::make_unique<fodder>("Red Dragon", 200, 25, 15, 6, 2500, 100, 30); break;
            default: enemy = std::make_unique<fodder>("Unknown", 10, 1, 1, 1, 100, 1, 1); break; 
        }
    }

    activeBattle = std::make_unique<BattleEncounter>(std::move(enemy), mapPlayer, itemRegistry, true, true);

    if (backgroundMusic.getStatus() == sf::SoundSource::Status::Playing)
    {
        backgroundMusic.stop();
    }
    if (battleMusic.getStatus() != sf::SoundSource::Status::Playing)
    {
        battleMusic.play();
    }
}

// renders battle overlay if active battle, does nothing if no battle or font not loaded
void GameManager::renderBattleOverlay()
{
    if (!activeBattle || font.getInfo().family.empty())
    {
        return;
    }

    activeBattle->render(window, font);
}

void GameManager::trySellSelectedItem()
{
    if (!shopOpen || !inventoryOpen)
    {
        return;
    }

    const auto itemIds = getInventoryItemIds();
    if (itemIds.empty() || selectedInventoryIndex < 0 || selectedInventoryIndex >= static_cast<int>(itemIds.size()))
    {
        showPopup("No item selected", 1.0f);
        return;
    }
    
    const int itemId = itemIds[selectedInventoryIndex];
    int sellPrice = 0;
    std::string itemName;
    
    InventoryNode* current = mapPlayer.getInventory().getHead();
    while (current != nullptr)
    {
        if (current->item && current->item->getId() == itemId)
        {
            sellPrice = current->item->getValue();
            itemName = current->item->getName();
            break;
        }
        current = current->next;
    }

    if (sellPrice <= 0)
    {
        showPopup("Item has no value", 1.2f);
        return;
    }

    // Process transaction
    mapPlayer.removeItemFromInventory(itemId, mapPlayer, 1);
    mapPlayer.setGold(mapPlayer.getGold() + sellPrice);

    // Prevent out-of-bounds index if item stack reaches 0
    const auto newItemIds = getInventoryItemIds();
    if (selectedInventoryIndex >= static_cast<int>(newItemIds.size()))
    {
        selectedInventoryIndex = std::max(0, static_cast<int>(newItemIds.size()) - 1);
    }

    showPopup("Sold " + itemName + " for " + std::to_string(sellPrice) + " gold", 1.2f);
}

std::vector<int> GameManager::getShopItemIds()
{
    std::vector<int> ids;
    if (!shopNpc) return ids;

    InventoryNode* current = shopNpc->getInventory().getHead();
    while (current != nullptr) {
        if (current->item && current->quantity > 0) {
            ids.push_back(current->item->getId());
        }
        current = current->next;
    }
    return ids;
}
#include "gameManager.h"
#include <algorithm>
#include <iostream>
#include <filesystem>
#include <vector>

#include "item.h"

// helper functions for asset paths, map file discovery, and JSON parsing
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

//checks a couple different places for the Maps directory
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


// get all .json files
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

        files.push_back(entry.path());
    }

    std::sort(files.begin(), files.end());
    return files;
}
}

// writes string to json
GameManager::GameManager()
    : window(sf::VideoMode(sf::Vector2u(WINDOW_WIDTH, WINDOW_HEIGHT)), "RPG Game"),
      mapPlayer("Ash", 50, 8, 4, 3, 1000, 6, 7, 0, 100),
      inventoryOpen(false)
{
    window.setFramerateLimit(60);
}

// i forget what this is for
GameManager::~GameManager()
{
}

bool GameManager::initialize()
{
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

void GameManager::run()
{
    sf::Clock clock;

    while (window.isOpen())
    {
        float deltaTime = clock.restart().asSeconds();

        // Handle events
        while (const auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                if (runPhase == RunPhase::SaveSelection)
                {
                    if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
                    {
                        window.close();
                    }
                    else if (keyPressed->scancode == sf::Keyboard::Scancode::Up)
                    {
                        if (!availableSaveFiles.empty())
                        {
                            selectedSaveIndex = (selectedSaveIndex - 1 + static_cast<int>(availableSaveFiles.size())) %
                                                static_cast<int>(availableSaveFiles.size());
                        }
                    }
                    else if (keyPressed->scancode == sf::Keyboard::Scancode::Down)
                    {
                        if (!availableSaveFiles.empty())
                        {
                            selectedSaveIndex = (selectedSaveIndex + 1) % static_cast<int>(availableSaveFiles.size());
                        }
                    }
                    else if (keyPressed->scancode == sf::Keyboard::Scancode::Enter)
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
                else if (runPhase == RunPhase::ConfirmExitSave)
                {
                    if (keyPressed->scancode == sf::Keyboard::Scancode::Y)
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
                    else if (keyPressed->scancode == sf::Keyboard::Scancode::N)
                    {
                        window.close();
                    }
                    else if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
                    {
                        runPhase = RunPhase::Playing;
                        exitPromptMessage.clear();
                    }
                }
                else if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
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
                        exitPromptMessage = "Save before exiting? Y = save and quit, N = quit, Esc = cancel";
                    }
                }
                else if (keyPressed->scancode == sf::Keyboard::Scancode::F5)
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
                else if (keyPressed->scancode == sf::Keyboard::Scancode::E)
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
                else if (keyPressed->scancode == sf::Keyboard::Scancode::F)
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
                else if (inventoryOpen && keyPressed->scancode == sf::Keyboard::Scancode::Up)
                {
                    if (selectedInventoryIndex > 0)
                    {
                        --selectedInventoryIndex;
                    }
                }
                else if (inventoryOpen && keyPressed->scancode == sf::Keyboard::Scancode::Down)
                {
                    const auto items = getInventoryItemIds();
                    if (!items.empty() && selectedInventoryIndex < static_cast<int>(items.size()) - 1)
                    {
                        ++selectedInventoryIndex;
                    }
                }
                else if (keyPressed->scancode == sf::Keyboard::Scancode::Enter)
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
        }

        if (runPhase == RunPhase::Playing)
        {
            handleInput();
        }
        update(deltaTime);

        // Render
        window.clear(sf::Color::Black);
        render();
        window.display();
    }
}

void GameManager::handleInput()
{
    if (inventoryOpen || shopOpen)
    {
        return;
    }

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
        mapController.movePlayer(dx, dy);
        timeSinceLastMove = 0.0f;
    }
}

void GameManager::update(float deltaTime)
{
    if (popupTimeRemaining > 0.0f)
    {
        popupTimeRemaining -= deltaTime;
        if (popupTimeRemaining <= 0.0f)
        {
            popupTimeRemaining = 0.0f;
            popupMessage.clear();
        }
    }
}

void GameManager::render()
{
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
    renderPopup();
}

void GameManager::renderMapViewport()
{
    mapController.render(window, WINDOW_WIDTH, WINDOW_HEIGHT, MAP_VIEWPORT_HEIGHT_RATIO);
}

void GameManager::renderClientViewport()
{
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

    sf::Text controls(font,
        "Move: WASD/Arrows  |  Interact: F  |  Inventory: E  |  Equip: Enter  |  Save: F5",
        18);
    controls.setFillColor(sf::Color(220, 228, 238));
    controls.setPosition({20.0f, panelTop + 10.0f});
    window.draw(controls);

    sf::Text status(font,
        "Gold: " + std::to_string(mapPlayer.getGold()) +
        "   Equipped Item ID: " + std::to_string(mapPlayer.getEquippedItemID()),
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

void GameManager::renderSaveSelectionScreen()
{
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

void GameManager::renderExitPrompt()
{
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

void GameManager::renderInventoryPanel(float panelTop, float panelHeight)
{
    sf::RectangleShape invBg({620.0f, panelHeight - 24.0f});
    invBg.setPosition({16.0f, panelTop + 12.0f});
    invBg.setFillColor(sf::Color(12, 16, 24, 240));
    invBg.setOutlineColor(sf::Color(205, 218, 234));
    invBg.setOutlineThickness(2.0f);
    window.draw(invBg);

    sf::Text title(font, "Inventory (Up/Down to select, Enter to equip, E to close)", 20);
    title.setFillColor(sf::Color::White);
    title.setPosition({28.0f, panelTop + 20.0f});
    window.draw(title);

    const auto itemIds = getInventoryItemIds();
    if (itemIds.empty())
    {
        sf::Text empty(font, "No items in inventory.", 18);
        empty.setFillColor(sf::Color(215, 215, 215));
        empty.setPosition({28.0f, panelTop + 56.0f});
        window.draw(empty);
        return;
    }

    if (selectedInventoryIndex >= static_cast<int>(itemIds.size()))
    {
        selectedInventoryIndex = static_cast<int>(itemIds.size()) - 1;
    }

    float y = panelTop + 56.0f;
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
        itemText.setPosition({28.0f, y});
        window.draw(itemText);
        y += 24.0f;
    }
}

void GameManager::renderShopPanel(float panelTop, float panelHeight)
{
    sf::RectangleShape shopBg({540.0f, panelHeight - 24.0f});
    shopBg.setPosition({WINDOW_WIDTH - 556.0f, panelTop + 12.0f});
    shopBg.setFillColor(sf::Color(24, 20, 14, 240));
    shopBg.setOutlineColor(sf::Color(255, 216, 140));
    shopBg.setOutlineThickness(2.0f);
    window.draw(shopBg);

    sf::Text title(font, "Shopkeeper", 22);
    title.setFillColor(sf::Color(255, 232, 170));
    title.setPosition({WINDOW_WIDTH - 544.0f, panelTop + 20.0f});
    window.draw(title);

    int shopItemId = -1;
    int shopItemPrice = 0;
    std::string shopItemName;
    const bool hasListing = getShopListing(shopItemId, shopItemName, shopItemPrice);

    sf::Text line1(font,
        hasListing ? ("Press Enter to buy: " + shopItemName) : "No items for sale",
        18);
    line1.setFillColor(sf::Color(240, 240, 240));
    line1.setPosition({WINDOW_WIDTH - 544.0f, panelTop + 54.0f});
    window.draw(line1);

    sf::Text line2(font,
        hasListing ? ("Price: " + std::to_string(shopItemPrice) + " gold") : "",
        18);
    line2.setFillColor(sf::Color(240, 240, 240));
    line2.setPosition({WINDOW_WIDTH - 544.0f, panelTop + 80.0f});
    window.draw(line2);

    sf::Text line3(font, "Press F to close shop", 18);
    line3.setFillColor(sf::Color(230, 210, 160));
    line3.setPosition({WINDOW_WIDTH - 544.0f, panelTop + 106.0f});
    window.draw(line3);
}

void GameManager::renderPopup()
{
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

void GameManager::tryInteraction()
{
    const int chunkX = map.getPlayerChunkX();
    const int chunkY = map.getPlayerChunkY();
    const int tileX = map.getPlayerTileX();
    const int tileY = map.getPlayerTileY();

    const OverworldMap::MapEntity* onTile = map.getEntityAtPosition(chunkX, chunkY, tileX, tileY);
    if (onTile)
    {
        if (onTile->type == "shop")
        {
            shopOpen = true;
            showPopup("Shop opened: Press Enter to buy", 1.6f);
        }
        else
        {
            showPopup("You interact with " + onTile->type + " #" + std::to_string(onTile->id));
        }
        return;
    }

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
        if (entity)
        {
            if (entity->type == "shop")
            {
                shopOpen = true;
                showPopup("Shop opened: Press Enter to buy", 1.6f);
            }
            else
            {
                showPopup("You interact with " + entity->type + " #" + std::to_string(entity->id));
            }
            return;
        }
    }

    showPopup("There is nothing to interact with.", 1.4f);
}

void GameManager::showPopup(const std::string& message, float seconds)
{
    popupMessage = message;
    popupTimeRemaining = seconds;
}

bool GameManager::initializeBackgroundMusic()
{
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
    musicReady = true;
    return true;
}

void GameManager::initializeMapGameplayState()
{
    auto sword = std::make_shared<Item>(0, "Iron Sword", "Sword of Iron", 50, false);
    auto potion = std::make_shared<Item>(1, "Health Potion", "Potion of Health", 10, true);
    auto shield = std::make_shared<Item>(2, "Shield", "Blocks attacks", 100, false);

    itemRegistry.setItem(0, sword);
    itemRegistry.setItem(1, potion);
    itemRegistry.setItem(2, shield);

    mapShop = std::make_unique<store>("Map Shop", itemRegistry);
    shopNpc = std::make_unique<npc>("Shopkeeper", "A local merchant", 50, 8, 4, 3, 1000, 6, 7, false, 0);
    shopNpc->addItemToInventory(1, itemRegistry, 30);
}

void GameManager::applyDefaultPlayerState()
{
    mapPlayer.setHp(50);
    mapPlayer.setGold(100);
    mapPlayer.setXp(0);
    mapPlayer.getInventory().clear();

    mapPlayer.addItemToInventory(0, itemRegistry, 1);
    mapPlayer.addItemToInventory(1, itemRegistry, 3);
    mapPlayer.addItemToInventory(2, itemRegistry, 1);
}

OverworldMap::PlayerState GameManager::buildPlayerStateForSave()
{
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

void GameManager::applyLoadedPlayerState(const OverworldMap::PlayerState& state)
{
    mapPlayer.setHp(state.hp);
    mapPlayer.setGold(state.gold);
    mapPlayer.setXp(state.xp);

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

bool GameManager::initializeMapFromSaveFile()
{
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

bool GameManager::loadSelectedSaveFile()
{
    if (availableSaveFiles.empty())
    {
        return false;
    }

    selectedSaveFile = availableSaveFiles[static_cast<std::size_t>(selectedSaveIndex)];
    return loadSaveFile(selectedSaveFile);
}

bool GameManager::loadSaveFile(const std::filesystem::path& path)
{
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
    return true;
}

bool GameManager::saveCurrentGameToSelectedFile()
{
    if (selectedSaveFile.empty())
    {
        return false;
    }

    map.setPlayerState(buildPlayerStateForSave());
    return map.saveToFile(selectedSaveFile.string());
}

std::vector<int> GameManager::getInventoryItemIds()
{
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

bool GameManager::getShopListing(int& itemId, std::string& itemName, int& itemPrice) const
{
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

void GameManager::equipSelectedInventoryItem()
{
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

void GameManager::tryBuyShopItem()
{
    if (!mapShop || !shopNpc)
    {
        showPopup("Shop unavailable", 1.2f);
        return;
    }

    int shopItemId = -1;
    int shopItemPrice = 0;
    std::string shopItemName;
    if (!getShopListing(shopItemId, shopItemName, shopItemPrice))
    {
        showPopup("Shop is sold out", 1.2f);
        return;
    }

    if (mapPlayer.getGold() < shopItemPrice)
    {
        showPopup("Not enough gold", 1.2f);
        return;
    }

    mapShop->buySomething(mapPlayer, *shopNpc, shopItemId, 1);
    showPopup("Bought " + shopItemName, 1.2f);
}

#include "TileRenderer.h"

namespace
{
const sf::Color kGrassColor(78, 153, 68);
const sf::Color kWaterColor(61, 129, 212);
const sf::Color kSandColor(211, 188, 122);
const sf::Color kEnemySpawnColor(237, 213, 77);

bool isBlockingEntityType(const std::string& type)
{
    return type == "rock" || type == "wall";
}

bool hasConnectedBlockingNeighbor(const OverworldMap& map,
                                  int chunkX, int chunkY,
                                  int tileX, int tileY)
{
    const OverworldMap::MapEntity* neighbor = map.getEntityAtPosition(chunkX, chunkY, tileX, tileY);
    return neighbor != nullptr && isBlockingEntityType(neighbor->type);
}

bool hasNeighborType(const OverworldMap& map,
                     int chunkX, int chunkY,
                     int tileX, int tileY,
                     const char* type)
{
    const OverworldMap::MapEntity* neighbor = map.getEntityAtPosition(chunkX, chunkY, tileX, tileY);
    return neighbor != nullptr && neighbor->type == type;
}

bool hasConnectedBlockingNeighborWrapped(const OverworldMap& map,
                                         int chunkX, int chunkY,
                                         int tileX, int tileY)
{
    int wrappedChunkX = chunkX;
    int wrappedChunkY = chunkY;
    int wrappedTileX = tileX;
    int wrappedTileY = tileY;

    while (wrappedTileX < 0)
    {
        wrappedTileX += OverworldMap::ChunkWidth;
        --wrappedChunkX;
    }
    while (wrappedTileX >= OverworldMap::ChunkWidth)
    {
        wrappedTileX -= OverworldMap::ChunkWidth;
        ++wrappedChunkX;
    }

    while (wrappedTileY < 0)
    {
        wrappedTileY += OverworldMap::ChunkHeight;
        --wrappedChunkY;
    }
    while (wrappedTileY >= OverworldMap::ChunkHeight)
    {
        wrappedTileY -= OverworldMap::ChunkHeight;
        ++wrappedChunkY;
    }

    if (wrappedChunkX < 0 || wrappedChunkX >= OverworldMap::WorldChunkWidth ||
        wrappedChunkY < 0 || wrappedChunkY >= OverworldMap::WorldChunkHeight)
    {
        return false;
    }

    return hasConnectedBlockingNeighbor(map, wrappedChunkX, wrappedChunkY, wrappedTileX, wrappedTileY);
}

bool hasNeighborTypeWrapped(const OverworldMap& map,
                            int chunkX, int chunkY,
                            int tileX, int tileY,
                            const char* type)
{
    int wrappedChunkX = chunkX;
    int wrappedChunkY = chunkY;
    int wrappedTileX = tileX;
    int wrappedTileY = tileY;

    while (wrappedTileX < 0)
    {
        wrappedTileX += OverworldMap::ChunkWidth;
        --wrappedChunkX;
    }
    while (wrappedTileX >= OverworldMap::ChunkWidth)
    {
        wrappedTileX -= OverworldMap::ChunkWidth;
        ++wrappedChunkX;
    }

    while (wrappedTileY < 0)
    {
        wrappedTileY += OverworldMap::ChunkHeight;
        --wrappedChunkY;
    }
    while (wrappedTileY >= OverworldMap::ChunkHeight)
    {
        wrappedTileY -= OverworldMap::ChunkHeight;
        ++wrappedChunkY;
    }

    if (wrappedChunkX < 0 || wrappedChunkX >= OverworldMap::WorldChunkWidth ||
        wrappedChunkY < 0 || wrappedChunkY >= OverworldMap::WorldChunkHeight)
    {
        return false;
    }

    return hasNeighborType(map, wrappedChunkX, wrappedChunkY, wrappedTileX, wrappedTileY, type);
}
}

void TileRenderer::renderChunk(sf::RenderWindow& window,
                              const OverworldMap& map,
                              int chunkX, int chunkY,
                              float mapOffsetX, float mapOffsetY,
                              float tileSize) const
{
    for (int y = 0; y < OverworldMap::ChunkHeight; ++y)
    {
        for (int x = 0; x < OverworldMap::ChunkWidth; ++x)
        {
            (void)map.getTile(chunkX, chunkY, x, y); // Validates access and preserves map API usage.
            float tileX = mapOffsetX + x * tileSize;
            float tileY = mapOffsetY + y * tileSize;

            sf::RectangleShape tileRect({tileSize - 1.f, tileSize - 1.f});
            tileRect.setPosition({tileX, tileY});
            const OverworldMap::Tile& tile = map.getTile(chunkX, chunkY, x, y);
            sf::Color groundColor = kGrassColor;
            if (tile.terrain == OverworldMap::TerrainType::Water)
            {
                groundColor = kWaterColor;
            }
            else if (tile.terrain == OverworldMap::TerrainType::Sand)
            {
                groundColor = kSandColor;
            }

            tileRect.setFillColor(groundColor);
            window.draw(tileRect);

            if (map.isEnemySpawnTileAtPosition(chunkX, chunkY, x, y))
            {
                sf::RectangleShape spawnOverlay({tileSize - 1.f, tileSize - 1.f});
                spawnOverlay.setPosition({tileX, tileY});
                spawnOverlay.setFillColor(sf::Color(kEnemySpawnColor.r, kEnemySpawnColor.g, kEnemySpawnColor.b, 86));
                window.draw(spawnOverlay);
            }
        }
    }
}

void TileRenderer::renderPlayer(sf::RenderWindow& window,
                               const OverworldMap& map,
                               float mapOffsetX, float mapOffsetY,
                               float tileSize) const
{
    sf::RectangleShape playerRect({tileSize * 0.7f, tileSize * 0.7f});
    playerRect.setOrigin({playerRect.getSize().x * 0.5f, playerRect.getSize().y * 0.5f});
    playerRect.setPosition({mapOffsetX + (static_cast<float>(map.getPlayerTileX()) + 0.5f) * tileSize,
                           mapOffsetY + (static_cast<float>(map.getPlayerTileY()) + 0.5f) * tileSize});
    playerRect.setFillColor(sf::Color(229, 66, 66));
    playerRect.setOutlineColor(sf::Color::Black);
    playerRect.setOutlineThickness(2.f);
    window.draw(playerRect);
}

void TileRenderer::renderEntities(sf::RenderWindow& window,
                                 const OverworldMap& map,
                                 float mapOffsetX, float mapOffsetY,
                                 float tileSize) const
{
    // Draw entities on map
    for (const auto& entity : map.getEntities())
    {
        if (entity.chunkX == map.getPlayerChunkX() && entity.chunkY == map.getPlayerChunkY())
        {
            if (entity.type == "wall")
            {
                const sf::Color wallColor(96, 80, 60);
                const float centerX = mapOffsetX + (static_cast<float>(entity.tileX) + 0.5f) * tileSize;
                const float centerY = mapOffsetY + (static_cast<float>(entity.tileY) + 0.5f) * tileSize;
                const float postSize = tileSize * 0.36f;
                const float connectorThickness = tileSize * 0.24f;

                const bool connectLeft = hasNeighborType(map, entity.chunkX, entity.chunkY, entity.tileX - 1, entity.tileY, "wall");
                const bool connectRight = hasNeighborType(map, entity.chunkX, entity.chunkY, entity.tileX + 1, entity.tileY, "wall");
                const bool connectUp = hasNeighborType(map, entity.chunkX, entity.chunkY, entity.tileX, entity.tileY - 1, "wall");
                const bool connectDown = hasNeighborType(map, entity.chunkX, entity.chunkY, entity.tileX, entity.tileY + 1, "wall");

                if (connectLeft)
                {
                    sf::RectangleShape connector({tileSize * 0.5f, connectorThickness});
                    connector.setPosition({centerX - tileSize * 0.5f, centerY - connectorThickness * 0.5f});
                    connector.setFillColor(wallColor);
                    window.draw(connector);
                }

                if (connectRight)
                {
                    sf::RectangleShape connector({tileSize * 0.5f, connectorThickness});
                    connector.setPosition({centerX, centerY - connectorThickness * 0.5f});
                    connector.setFillColor(wallColor);
                    window.draw(connector);
                }

                if (connectUp)
                {
                    sf::RectangleShape connector({connectorThickness, tileSize * 0.5f});
                    connector.setPosition({centerX - connectorThickness * 0.5f, centerY - tileSize * 0.5f});
                    connector.setFillColor(wallColor);
                    window.draw(connector);
                }

                if (connectDown)
                {
                    sf::RectangleShape connector({connectorThickness, tileSize * 0.5f});
                    connector.setPosition({centerX - connectorThickness * 0.5f, centerY});
                    connector.setFillColor(wallColor);
                    window.draw(connector);
                }

                sf::RectangleShape post({postSize, postSize});
                post.setOrigin({postSize * 0.5f, postSize * 0.5f});
                post.setPosition({centerX, centerY});
                post.setFillColor(wallColor);
                window.draw(post);
                continue;
            }

            sf::RectangleShape entityRect;
            if (isBlockingEntityType(entity.type))
            {
                const bool isRock = (entity.type == "rock");
                const float margin = tileSize * 0.16f;
                const float connectedInset = 0.0f;
                const bool connectLeft = hasConnectedBlockingNeighborWrapped(map, entity.chunkX, entity.chunkY, entity.tileX - 1, entity.tileY);
                const bool connectRight = hasConnectedBlockingNeighborWrapped(map, entity.chunkX, entity.chunkY, entity.tileX + 1, entity.tileY);
                const bool connectUp = hasConnectedBlockingNeighborWrapped(map, entity.chunkX, entity.chunkY, entity.tileX, entity.tileY - 1);
                const bool connectDown = hasConnectedBlockingNeighborWrapped(map, entity.chunkX, entity.chunkY, entity.tileX, entity.tileY + 1);

                float leftInset = connectLeft ? connectedInset : margin;
                float rightInset = connectRight ? connectedInset : margin;
                float topInset = connectUp ? connectedInset : margin;
                float bottomInset = connectDown ? connectedInset : margin;

                if (isRock)
                {
                    if (entity.tileX == 0)
                    {
                        leftInset = 0.0f;
                    }
                    if (entity.tileX == OverworldMap::ChunkWidth - 1)
                    {
                        rightInset = 0.0f;
                    }
                    if (entity.tileY == 0)
                    {
                        topInset = 0.0f;
                    }
                    if (entity.tileY == OverworldMap::ChunkHeight - 1)
                    {
                        bottomInset = 0.0f;
                    }
                }

                entityRect.setSize({tileSize - leftInset - rightInset, tileSize - topInset - bottomInset});
                entityRect.setPosition({mapOffsetX + static_cast<float>(entity.tileX) * tileSize + leftInset,
                                        mapOffsetY + static_cast<float>(entity.tileY) * tileSize + topInset});
            }
            else
            {
                entityRect.setSize({tileSize * 0.6f, tileSize * 0.6f});
                entityRect.setOrigin({entityRect.getSize().x * 0.5f, entityRect.getSize().y * 0.5f});
                entityRect.setPosition({mapOffsetX + (static_cast<float>(entity.tileX) + 0.5f) * tileSize,
                                        mapOffsetY + (static_cast<float>(entity.tileY) + 0.5f) * tileSize});
            }

            if (entity.type == "npc")
            {
                entityRect.setFillColor(sf::Color(100, 200, 255));
            }
            else if (entity.type == "shop")
            {
                entityRect.setFillColor(sf::Color(255, 214, 102));
            }
            else if (entity.type == "rock")
            {
                entityRect.setFillColor(sf::Color(130, 130, 130));
            }
            else if (entity.type == "wall")
            {
                entityRect.setFillColor(sf::Color(96, 80, 60));
            }
            else
            {
                entityRect.setFillColor(sf::Color(255, 100, 100));
            }
            window.draw(entityRect);
        }
    }

    const int currentChunkX = map.getPlayerChunkX();
    const int currentChunkY = map.getPlayerChunkY();
    const sf::Color rockColor(130, 130, 130);
    const float previewThickness = tileSize * 0.14f;
    const float previewInset = tileSize * 0.16f;

    for (const auto& entity : map.getEntities())
    {
        if (entity.type != "rock")
        {
            continue;
        }

        if (entity.chunkX == currentChunkX - 1 && entity.chunkY == currentChunkY && entity.tileX == OverworldMap::ChunkWidth - 1)
        {
            const bool connectUp = hasNeighborTypeWrapped(map, entity.chunkX, entity.chunkY, entity.tileX, entity.tileY - 1, "rock");
            const bool connectDown = hasNeighborTypeWrapped(map, entity.chunkX, entity.chunkY, entity.tileX, entity.tileY + 1, "rock");
            const float topInset = connectUp ? 0.0f : previewInset;
            const float bottomInset = connectDown ? 0.0f : previewInset;

            sf::RectangleShape preview({previewThickness, tileSize - topInset - bottomInset});
            preview.setPosition({mapOffsetX,
                                 mapOffsetY + static_cast<float>(entity.tileY) * tileSize + topInset});
            preview.setFillColor(rockColor);
            window.draw(preview);
        }
        else if (entity.chunkX == currentChunkX + 1 && entity.chunkY == currentChunkY && entity.tileX == 0)
        {
            const bool connectUp = hasNeighborTypeWrapped(map, entity.chunkX, entity.chunkY, entity.tileX, entity.tileY - 1, "rock");
            const bool connectDown = hasNeighborTypeWrapped(map, entity.chunkX, entity.chunkY, entity.tileX, entity.tileY + 1, "rock");
            const float topInset = connectUp ? 0.0f : previewInset;
            const float bottomInset = connectDown ? 0.0f : previewInset;

            sf::RectangleShape preview({previewThickness, tileSize - topInset - bottomInset});
            preview.setPosition({mapOffsetX + static_cast<float>(OverworldMap::ChunkWidth) * tileSize - previewThickness,
                                 mapOffsetY + static_cast<float>(entity.tileY) * tileSize + topInset});
            preview.setFillColor(rockColor);
            window.draw(preview);
        }
        else if (entity.chunkX == currentChunkX && entity.chunkY == currentChunkY - 1 && entity.tileY == OverworldMap::ChunkHeight - 1)
        {
            const bool connectLeft = hasNeighborTypeWrapped(map, entity.chunkX, entity.chunkY, entity.tileX - 1, entity.tileY, "rock");
            const bool connectRight = hasNeighborTypeWrapped(map, entity.chunkX, entity.chunkY, entity.tileX + 1, entity.tileY, "rock");
            const float leftInset = connectLeft ? 0.0f : previewInset;
            const float rightInset = connectRight ? 0.0f : previewInset;

            sf::RectangleShape preview({tileSize - leftInset - rightInset, previewThickness});
            preview.setPosition({mapOffsetX + static_cast<float>(entity.tileX) * tileSize + leftInset,
                                 mapOffsetY});
            preview.setFillColor(rockColor);
            window.draw(preview);
        }
        else if (entity.chunkX == currentChunkX && entity.chunkY == currentChunkY + 1 && entity.tileY == 0)
        {
            const bool connectLeft = hasNeighborTypeWrapped(map, entity.chunkX, entity.chunkY, entity.tileX - 1, entity.tileY, "rock");
            const bool connectRight = hasNeighborTypeWrapped(map, entity.chunkX, entity.chunkY, entity.tileX + 1, entity.tileY, "rock");
            const float leftInset = connectLeft ? 0.0f : previewInset;
            const float rightInset = connectRight ? 0.0f : previewInset;

            sf::RectangleShape preview({tileSize - leftInset - rightInset, previewThickness});
            preview.setPosition({mapOffsetX + static_cast<float>(entity.tileX) * tileSize + leftInset,
                                 mapOffsetY + static_cast<float>(OverworldMap::ChunkHeight) * tileSize - previewThickness});
            preview.setFillColor(rockColor);
            window.draw(preview);
        }
    }
}

#include "TileRenderer.h"

namespace
{
const sf::Color kGrassColor(78, 153, 68);
const sf::Color kEnemySpawnColor(237, 213, 77);
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
            tileRect.setFillColor(map.isEnemySpawnTileAtPosition(chunkX, chunkY, x, y) ? kEnemySpawnColor : kGrassColor);
            window.draw(tileRect);
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
            sf::RectangleShape entityRect({tileSize * 0.6f, tileSize * 0.6f});
            entityRect.setOrigin({entityRect.getSize().x * 0.5f, entityRect.getSize().y * 0.5f});
            entityRect.setPosition({mapOffsetX + (static_cast<float>(entity.tileX) + 0.5f) * tileSize,
                                  mapOffsetY + (static_cast<float>(entity.tileY) + 0.5f) * tileSize});
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
}

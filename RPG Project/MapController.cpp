#include "MapController.h"
#include <cmath>

void MapController::initialize(OverworldMap* mapPtr)
{
    map = mapPtr;
}

void MapController::render(sf::RenderWindow& window,
                          unsigned int windowWidth, unsigned int windowHeight,
                          float mapAreaRatio)
{
    if (!map)
        return;

    // Calculate rendering parameters
    float tileSize = calculateTileSize(windowWidth, windowHeight, mapAreaRatio);
    auto [offsetX, offsetY] = calculateMapOffset(windowWidth, windowHeight, mapAreaRatio, tileSize);

    // Render the map chunk
    renderer.renderChunk(window, *map, map->getPlayerChunkX(), map->getPlayerChunkY(), offsetX, offsetY, tileSize);

    // Render entities
    renderer.renderEntities(window, *map, offsetX, offsetY, tileSize);

    // Render player
    renderer.renderPlayer(window, *map, offsetX, offsetY, tileSize);

}

bool MapController::movePlayer(int dx, int dy)
{
    if (!map)
        return false;

    return map->tryMovePlayer(dx, dy);
}

float MapController::calculateTileSize(unsigned int windowWidth, unsigned int windowHeight, float mapAreaRatio) const
{
    float mapAreaHeight = static_cast<float>(windowHeight) * mapAreaRatio;
    float tileWidth = (static_cast<float>(windowWidth) - 24.0f) / static_cast<float>(OverworldMap::ChunkWidth);
    float tileHeight = mapAreaHeight / static_cast<float>(OverworldMap::ChunkHeight);
    return std::min(tileWidth, tileHeight);
}

std::pair<float, float> MapController::calculateMapOffset(unsigned int windowWidth, unsigned int windowHeight,
                                                         float mapAreaRatio, float tileSize) const
{
    float mapAreaHeight = static_cast<float>(windowHeight) * mapAreaRatio;
    float usedWidth = tileSize * static_cast<float>(OverworldMap::ChunkWidth);
    float usedHeight = tileSize * static_cast<float>(OverworldMap::ChunkHeight);
    float offsetX = (static_cast<float>(windowWidth) - usedWidth) * 0.5f;
    float offsetY = (mapAreaHeight - usedHeight) * 0.5f;
    return {offsetX, offsetY};
}


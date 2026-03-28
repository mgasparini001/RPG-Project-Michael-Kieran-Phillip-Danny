#pragma once

#include <SFML/Graphics.hpp>
#include "overworldMap.h"

/// TileRenderer handles all tile-based rendering for the overworld map
class TileRenderer
{
public:
    TileRenderer() = default;
    ~TileRenderer() = default;

    /// Render the entire visible chunk
    void renderChunk(sf::RenderWindow& window,
                     const OverworldMap& map,
                     int chunkX, int chunkY,
                     float mapOffsetX, float mapOffsetY,
                     float tileSize) const;

    /// Render the player on the map
    void renderPlayer(sf::RenderWindow& window,
                     const OverworldMap& map,
                     float mapOffsetX, float mapOffsetY,
                     float tileSize) const;

    /// Render entities (NPCs/Enemies) on the current chunk
    void renderEntities(sf::RenderWindow& window,
                       const OverworldMap& map,
                       float mapOffsetX, float mapOffsetY,
                       float tileSize) const;
};

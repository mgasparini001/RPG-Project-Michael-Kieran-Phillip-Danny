#pragma once

#include <SFML/Graphics.hpp>
#include "overworldMap.h"
#include "TileRenderer.h"

/// MapController orchestrates map display and movement interaction
class MapController
{
public:
    MapController() = default;
    ~MapController() = default;

    /// Initialize the controller with a map
    void initialize(OverworldMap* map);

    /// Render the map to a window
    void render(sf::RenderWindow& window, 
               unsigned int windowWidth, unsigned int windowHeight,
               float mapAreaRatio);

    /// Handle player movement with automatic chunk transitions
    bool movePlayer(int dx, int dy, bool ignoreTraversalRules = false);

    // Rendering constants
    float calculateTileSize(unsigned int windowWidth, unsigned int windowHeight, float mapAreaRatio) const;
    std::pair<float, float> calculateMapOffset(unsigned int windowWidth, unsigned int windowHeight,
                                              float mapAreaRatio, float tileSize) const;

private:
    OverworldMap* map = nullptr;
    TileRenderer renderer;
};

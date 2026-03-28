#pragma once

#include <array>
#include <vector>
#include <string>

class OverworldMap
{
public:
    struct Tile
    {
        // World is currently all grass, but we keep passable so collision can be
        // reintroduced later without changing the tile API.
        bool passable = true;
    };

    struct MapEntity
    {
        int id = -1;
        int chunkX = 0;
        int chunkY = 0;
        int tileX = 0;
        int tileY = 0;
        std::string type; // "npc" or "enemy"
    };

    static constexpr int ChunkWidth = 16;
    static constexpr int ChunkHeight = 12;
    static constexpr int WorldChunkWidth = 12;
    static constexpr int WorldChunkHeight = 12;

    OverworldMap();

    [[nodiscard]] const Tile& getTile(int chunkX, int chunkY, int tileX, int tileY) const;

    bool tryMovePlayer(int dx, int dy);

    // Persist and restore the entire map state (tiles, entities, and player position).
    bool saveToFile(const std::string& filePath) const;
    bool loadFromFile(const std::string& filePath);

    void addEntity(int id, int chunkX, int chunkY, int tileX, int tileY, const std::string& type);
    [[nodiscard]] int checkEntityAtPosition(int chunkX, int chunkY, int tileX, int tileY) const;
    [[nodiscard]] const std::vector<MapEntity>& getEntities() const;

    [[nodiscard]] int getPlayerChunkX() const;
    [[nodiscard]] int getPlayerChunkY() const;
    [[nodiscard]] int getPlayerTileX() const;
    [[nodiscard]] int getPlayerTileY() const;

private:
    using ChunkTiles = std::array<Tile, ChunkWidth * ChunkHeight>;
    using WorldChunks = std::array<ChunkTiles, WorldChunkWidth * WorldChunkHeight>;

    static int tileIndexFor(int x, int y);
    static int chunkIndexFor(int chunkX, int chunkY);
    static bool isChunkInBounds(int chunkX, int chunkY);

    WorldChunks m_chunks{};
    std::vector<MapEntity> m_entities;

    int m_playerChunkX = 0;
    int m_playerChunkY = 0;
    int m_playerTileX = ChunkWidth / 2;
    int m_playerTileY = ChunkHeight / 2;
};

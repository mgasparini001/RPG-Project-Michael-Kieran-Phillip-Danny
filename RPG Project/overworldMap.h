#pragma once

#include <array>
#include <vector>
#include <string>
#include <map>

class OverworldMap
{
public:
    enum class TerrainType
    {
        Grass,
        Water,
        Sand
    };

    struct Tile
    {
        TerrainType terrain = TerrainType::Grass;
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

    struct EntityMetadata
    {
        struct DialogueOption
        {
            std::string text;
            int nextNodeId = -1; // -1 ends conversation
        };

        struct DialogueNode
        {
            int id = 1;
            std::string npcText;
            std::vector<DialogueOption> options;
        };

        std::string npcName;
        std::string npcDescription;
        std::string dialogue; // comma-separated dialogue nodes or full text
        int xpReward = 0;
        std::string shopName;
        std::string shopNpcName;
        std::string shopInventory; // comma-separated format: "id:quantity,id:quantity"
        std::vector<DialogueNode> dialogueTree;
    };

    struct EnemySpawnTile
    {
        int chunkX = 0;
        int chunkY = 0;
        int tileX = 0;
        int tileY = 0;
    };

    struct PlayerInventoryEntry
    {
        int id = -1;
        int quantity = 0;
        bool equipped = false;
    };

    struct PlayerState
    {
        bool hasData = false;
        int hp = 0;
        int gold = 0;
        int xp = 0;
        int equippedItemId = -1;
        std::vector<PlayerInventoryEntry> inventory;
    };

    static constexpr int ChunkWidth = 16;
    static constexpr int ChunkHeight = 12;
    static constexpr int WorldChunkWidth = 12;
    static constexpr int WorldChunkHeight = 12;

    OverworldMap();

    [[nodiscard]] const Tile& getTile(int chunkX, int chunkY, int tileX, int tileY) const;
    bool setTileTerrain(int chunkX, int chunkY, int tileX, int tileY, TerrainType terrain);
    bool setTilePassable(int chunkX, int chunkY, int tileX, int tileY, bool passable);

    bool tryMovePlayer(int dx, int dy, bool ignoreTraversalRules = false);

    // Persist and restore the entire map state (tiles, entities, and player position).
    bool saveToFile(const std::string& filePath) const;
    bool loadFromFile(const std::string& filePath);

    void addEntity(int id, int chunkX, int chunkY, int tileX, int tileY, const std::string& type);
    bool placeOrReplaceEntity(int chunkX, int chunkY, int tileX, int tileY, const std::string& type);
    bool removeEntityAtPosition(int chunkX, int chunkY, int tileX, int tileY);
    [[nodiscard]] const MapEntity* getEntityAtPosition(int chunkX, int chunkY, int tileX, int tileY) const;
    [[nodiscard]] const std::vector<MapEntity>& getEntities() const;

    void addEnemySpawnTile(int chunkX, int chunkY, int tileX, int tileY);
    [[nodiscard]] bool isEnemySpawnTileAtPosition(int chunkX, int chunkY, int tileX, int tileY) const;

    bool removeEnemySpawnTile(int chunkX, int chunkY, int tileX, int tileY);

    void setPlayerState(const PlayerState& playerState);
    [[nodiscard]] const PlayerState* getPlayerState() const;

    void setEntityMetadata(int entityId, const EntityMetadata& metadata);
    [[nodiscard]] const EntityMetadata* getEntityMetadata(int entityId) const;

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
    std::vector<EnemySpawnTile> m_enemySpawnTiles;
    PlayerState m_playerState;
    std::map<int, EntityMetadata> m_entityMetadata;

    int m_playerChunkX = 0;
    int m_playerChunkY = 0;
    int m_playerTileX = ChunkWidth / 2;
    int m_playerTileY = ChunkHeight / 2;
};

#include "overworldMap.h"

#include <cassert>
#include <algorithm>
#include <cctype>
#include <fstream>
#include <limits>
#include <stdexcept>
#include <string_view>
#include <utility>

namespace
{
constexpr int kMapVersion = 2;

const char* terrainToString(OverworldMap::TerrainType terrain)
{
    switch (terrain)
    {
    case OverworldMap::TerrainType::Grass:
        return "grass";
    case OverworldMap::TerrainType::Water:
        return "water";
    case OverworldMap::TerrainType::Sand:
        return "sand";
    default:
        return "grass";
    }
}

bool tryParseTerrain(const std::string& value, OverworldMap::TerrainType& terrain)
{
    if (value == "grass")
    {
        terrain = OverworldMap::TerrainType::Grass;
        return true;
    }

    if (value == "water")
    {
        terrain = OverworldMap::TerrainType::Water;
        return true;
    }

    if (value == "sand")
    {
        terrain = OverworldMap::TerrainType::Sand;
        return true;
    }

    return false;
}

class JsonReader
{
public:
    explicit JsonReader(const std::string& source)
        : m_source(source)
    {
    }

    void expect(char expected)
    {
        skipWhitespace();
        if (m_pos >= m_source.size() || m_source[m_pos] != expected)
        {
            throw std::runtime_error("Invalid JSON format");
        }
        ++m_pos;
    }

    bool tryConsume(char value)
    {
        skipWhitespace();
        if (m_pos < m_source.size() && m_source[m_pos] == value)
        {
            ++m_pos;
            return true;
        }
        return false;
    }

    std::string parseString()
    {
        skipWhitespace();
        if (m_pos >= m_source.size() || m_source[m_pos] != '"')
        {
            throw std::runtime_error("Expected string");
        }

        ++m_pos;
        std::string result;
        while (m_pos < m_source.size())
        {
            const char current = m_source[m_pos++];
            if (current == '"')
            {
                return result;
            }

            if (current == '\\')
            {
                if (m_pos >= m_source.size())
                {
                    throw std::runtime_error("Invalid escape sequence");
                }

                const char escaped = m_source[m_pos++];
                switch (escaped)
                {
                case '"':
                case '\\':
                case '/':
                    result.push_back(escaped);
                    break;
                case 'b':
                    result.push_back('\b');
                    break;
                case 'f':
                    result.push_back('\f');
                    break;
                case 'n':
                    result.push_back('\n');
                    break;
                case 'r':
                    result.push_back('\r');
                    break;
                case 't':
                    result.push_back('\t');
                    break;
                default:
                    throw std::runtime_error("Unsupported escape sequence");
                }
                continue;
            }

            result.push_back(current);
        }

        throw std::runtime_error("Unterminated string");
    }

    int parseInt()
    {
        skipWhitespace();
        if (m_pos >= m_source.size())
        {
            throw std::runtime_error("Expected number");
        }

        std::size_t start = m_pos;
        if (m_source[m_pos] == '-')
        {
            ++m_pos;
        }

        if (m_pos >= m_source.size() || !std::isdigit(static_cast<unsigned char>(m_source[m_pos])))
        {
            throw std::runtime_error("Invalid number");
        }

        while (m_pos < m_source.size() && std::isdigit(static_cast<unsigned char>(m_source[m_pos])))
        {
            ++m_pos;
        }

        const std::string_view token(m_source.data() + start, m_pos - start);
        long long parsed = 0;
        try
        {
            parsed = std::stoll(std::string(token));
        }
        catch (...)
        {
            throw std::runtime_error("Number out of range");
        }

        if (parsed < std::numeric_limits<int>::min() || parsed > std::numeric_limits<int>::max())
        {
            throw std::runtime_error("Number out of range");
        }

        return static_cast<int>(parsed);
    }

    bool parseBool()
    {
        skipWhitespace();
        if (m_source.compare(m_pos, 4, "true") == 0)
        {
            m_pos += 4;
            return true;
        }

        if (m_source.compare(m_pos, 5, "false") == 0)
        {
            m_pos += 5;
            return false;
        }

        throw std::runtime_error("Expected boolean");
    }

    void expectKey(const char* key)
    {
        const std::string parsedKey = parseString();
        if (parsedKey != key)
        {
            throw std::runtime_error("Unexpected key");
        }
        expect(':');
    }

    void ensureFullyConsumed()
    {
        skipWhitespace();
        if (m_pos != m_source.size())
        {
            throw std::runtime_error("Trailing data in JSON");
        }
    }

private:
    void skipWhitespace()
    {
        while (m_pos < m_source.size() && std::isspace(static_cast<unsigned char>(m_source[m_pos])))
        {
            ++m_pos;
        }
    }

    const std::string& m_source;
    std::size_t m_pos = 0;
};

void writeEscapedJsonString(std::ostream& out, const std::string& value)
{
    out.put('"');
    for (const char c : value)
    {
        switch (c)
        {
        case '"':
            out << "\\\"";
            break;
        case '\\':
            out << "\\\\";
            break;
        case '\b':
            out << "\\b";
            break;
        case '\f':
            out << "\\f";
            break;
        case '\n':
            out << "\\n";
            break;
        case '\r':
            out << "\\r";
            break;
        case '\t':
            out << "\\t";
            break;
        default:
            out.put(c);
            break;
        }
    }
    out.put('"');
}
}

OverworldMap::OverworldMap()
{
    // No runtime generation: map state is expected to be loaded from a save file.
}

const OverworldMap::Tile& OverworldMap::getTile(int chunkX, int chunkY, int tileX, int tileY) const
{
    assert(isChunkInBounds(chunkX, chunkY));
    assert(tileX >= 0 && tileX < ChunkWidth);
    assert(tileY >= 0 && tileY < ChunkHeight);

    const ChunkTiles& chunk = m_chunks[static_cast<std::size_t>(chunkIndexFor(chunkX, chunkY))];
    return chunk[static_cast<std::size_t>(tileIndexFor(tileX, tileY))];
}

bool OverworldMap::setTileTerrain(int chunkX, int chunkY, int tileX, int tileY, TerrainType terrain)
{
    if (!isChunkInBounds(chunkX, chunkY) ||
        tileX < 0 || tileX >= ChunkWidth ||
        tileY < 0 || tileY >= ChunkHeight)
    {
        return false;
    }

    ChunkTiles& chunk = m_chunks[static_cast<std::size_t>(chunkIndexFor(chunkX, chunkY))];
    chunk[static_cast<std::size_t>(tileIndexFor(tileX, tileY))].terrain = terrain;
    return true;
}

bool OverworldMap::setTilePassable(int chunkX, int chunkY, int tileX, int tileY, bool passable)
{
    if (!isChunkInBounds(chunkX, chunkY) ||
        tileX < 0 || tileX >= ChunkWidth ||
        tileY < 0 || tileY >= ChunkHeight)
    {
        return false;
    }

    ChunkTiles& chunk = m_chunks[static_cast<std::size_t>(chunkIndexFor(chunkX, chunkY))];
    chunk[static_cast<std::size_t>(tileIndexFor(tileX, tileY))].passable = passable;
    return true;
}

bool OverworldMap::tryMovePlayer(int dx, int dy, bool ignoreTraversalRules)
{
    if ((dx == 0 && dy == 0) || (dx != 0 && dy != 0))
    {
        return false;
    }

    int nextChunkX = m_playerChunkX;
    int nextChunkY = m_playerChunkY;
    int nextTileX = m_playerTileX + dx;
    int nextTileY = m_playerTileY + dy;

    if (nextTileX < 0)
    {
        --nextChunkX;
        nextTileX = ChunkWidth - 1;
    }
    else if (nextTileX >= ChunkWidth)
    {
        ++nextChunkX;
        nextTileX = 0;
    }

    if (nextTileY < 0)
    {
        --nextChunkY;
        nextTileY = ChunkHeight - 1;
    }
    else if (nextTileY >= ChunkHeight)
    {
        ++nextChunkY;
        nextTileY = 0;
    }

    if (!isChunkInBounds(nextChunkX, nextChunkY))
    {
        return false;
    }

    if (ignoreTraversalRules)
    {
        m_playerChunkX = nextChunkX;
        m_playerChunkY = nextChunkY;
        m_playerTileX = nextTileX;
        m_playerTileY = nextTileY;
        return true;
    }

    const Tile& currentTile = getTile(m_playerChunkX, m_playerChunkY, m_playerTileX, m_playerTileY);
    const Tile& nextTile = getTile(nextChunkX, nextChunkY, nextTileX, nextTileY);
    const MapEntity* currentEntity = getEntityAtPosition(m_playerChunkX, m_playerChunkY, m_playerTileX, m_playerTileY);
    const MapEntity* nextEntity = getEntityAtPosition(nextChunkX, nextChunkY, nextTileX, nextTileY);
    const bool standingOnRock = currentEntity != nullptr && currentEntity->type == "rock";
    const bool steppingToRock = nextEntity != nullptr && nextEntity->type == "rock";
    const bool steppingToWall = nextEntity != nullptr && nextEntity->type == "wall";

    if (steppingToWall)
    {
        return false;
    }

    if (currentTile.terrain == TerrainType::Water &&
        nextTile.terrain != TerrainType::Sand &&
        nextTile.terrain != TerrainType::Water)
    {
        return false;
    }

    if (steppingToRock && currentTile.terrain != TerrainType::Sand && !standingOnRock)
    {
        return false;
    }

    if (!nextTile.passable && !steppingToRock)
    {
        return false;
    }

    m_playerChunkX = nextChunkX;
    m_playerChunkY = nextChunkY;
    m_playerTileX = nextTileX;
    m_playerTileY = nextTileY;
    return true;
}

bool OverworldMap::saveToFile(const std::string& filePath) const
{
    std::ofstream file(filePath);
    if (!file)
    {
        return false;
    }

    file << "{\n";
    file << "  \"version\": " << kMapVersion << ",\n";
    file << "  \"player\": {\n";
    file << "    \"chunkX\": " << m_playerChunkX << ",\n";
    file << "    \"chunkY\": " << m_playerChunkY << ",\n";
    file << "    \"tileX\": " << m_playerTileX << ",\n";
    file << "    \"tileY\": " << m_playerTileY << "\n";
    file << "  },\n";
    file << "  \"tiles\": [";

    bool firstTile = true;
    for (int chunkY = 0; chunkY < WorldChunkHeight; ++chunkY)
    {
        for (int chunkX = 0; chunkX < WorldChunkWidth; ++chunkX)
        {
            const ChunkTiles& chunk = m_chunks[static_cast<std::size_t>(chunkIndexFor(chunkX, chunkY))];
            for (const Tile& tile : chunk)
            {
                if (!firstTile)
                {
                    file << ",";
                }
                file << (tile.passable ? "true" : "false");
                firstTile = false;
            }
        }
    }

    file << "],\n";
    file << "  \"terrain\": [";

    bool firstTerrain = true;
    for (int chunkY = 0; chunkY < WorldChunkHeight; ++chunkY)
    {
        for (int chunkX = 0; chunkX < WorldChunkWidth; ++chunkX)
        {
            const ChunkTiles& chunk = m_chunks[static_cast<std::size_t>(chunkIndexFor(chunkX, chunkY))];
            for (const Tile& tile : chunk)
            {
                if (!firstTerrain)
                {
                    file << ",";
                }
                writeEscapedJsonString(file, terrainToString(tile.terrain));
                firstTerrain = false;
            }
        }
    }

    file << "],\n";
    file << "  \"entities\": [";
    for (std::size_t i = 0; i < m_entities.size(); ++i)
    {
        const MapEntity& entity = m_entities[i];
        if (i > 0)
        {
            file << ",";
        }
        file << "\n    {\"id\": " << entity.id
             << ", \"chunkX\": " << entity.chunkX
             << ", \"chunkY\": " << entity.chunkY
             << ", \"tileX\": " << entity.tileX
             << ", \"tileY\": " << entity.tileY
             << ", \"type\": ";
        writeEscapedJsonString(file, entity.type);
        file << "}";
    }

    if (!m_entities.empty())
    {
        file << "\n  ";
    }
    file << "],\n";

    file << "  \"playerState\": {\n";
    file << "    \"hasData\": " << (m_playerState.hasData ? "true" : "false") << ",\n";
    file << "    \"hp\": " << m_playerState.hp << ",\n";
    file << "    \"gold\": " << m_playerState.gold << ",\n";
    file << "    \"xp\": " << m_playerState.xp << ",\n";
    file << "    \"equippedItemId\": " << m_playerState.equippedItemId << ",\n";
    file << "    \"inventory\": [";

    for (std::size_t i = 0; i < m_playerState.inventory.size(); ++i)
    {
        const PlayerInventoryEntry& entry = m_playerState.inventory[i];
        if (i > 0)
        {
            file << ",";
        }

        file << "\n      {\"id\": " << entry.id
             << ", \"quantity\": " << entry.quantity
             << ", \"equipped\": " << (entry.equipped ? "true" : "false")
             << "}";
    }

    if (!m_playerState.inventory.empty())
    {
        file << "\n    ";
    }
    file << "]\n";
    file << "  }\n";

    file << ",\n";
    file << "  \"enemySpawnTiles\": [";
    for (std::size_t i = 0; i < m_enemySpawnTiles.size(); ++i)
    {
        const EnemySpawnTile& spawnTile = m_enemySpawnTiles[i];
        if (i > 0)
        {
            file << ",";
        }

        file << "\n    {\"chunkX\": " << spawnTile.chunkX
             << ", \"chunkY\": " << spawnTile.chunkY
             << ", \"tileX\": " << spawnTile.tileX
             << ", \"tileY\": " << spawnTile.tileY << "}";
    }

    if (!m_enemySpawnTiles.empty())
    {
        file << "\n  ";
    }
    file << "]\n";

    file << ",\n";
    file << "  \"entityMetadata\": [";
    
    bool firstMetadata = true;
    for (const auto& [entityId, metadata] : m_entityMetadata)
    {
        if (!firstMetadata)
        {
            file << ",";
        }
        firstMetadata = false;

        file << "\n    {\"entityId\": " << entityId << ", \"data\": {";
        file << "\"npcName\": ";
        writeEscapedJsonString(file, metadata.npcName);
        file << ", \"npcDescription\": ";
        writeEscapedJsonString(file, metadata.npcDescription);
        file << ", \"dialogue\": ";
        writeEscapedJsonString(file, metadata.dialogue);
        file << ", \"xpReward\": " << metadata.xpReward;
        file << ", \"shopName\": ";
        writeEscapedJsonString(file, metadata.shopName);
        file << ", \"shopNpcName\": ";
        writeEscapedJsonString(file, metadata.shopNpcName);
        file << ", \"shopInventory\": ";
        writeEscapedJsonString(file, metadata.shopInventory);
        file << ", \"dialogueTree\": [";
        for (std::size_t i = 0; i < metadata.dialogueTree.size(); ++i)
        {
            const auto& node = metadata.dialogueTree[i];
            if (i > 0)
            {
                file << ",";
            }

            file << "{\"id\": " << node.id << ", \"npcText\": ";
            writeEscapedJsonString(file, node.npcText);
            file << ", \"options\": [";

            for (std::size_t optionIndex = 0; optionIndex < node.options.size(); ++optionIndex)
            {
                const auto& option = node.options[optionIndex];
                if (optionIndex > 0)
                {
                    file << ",";
                }

                file << "{\"text\": ";
                writeEscapedJsonString(file, option.text);
                file << ", \"nextNodeId\": " << option.nextNodeId << "}";
            }

            file << "]}";
        }
        file << "]";
        file << "}}";
    }
    
    if (!m_entityMetadata.empty())
    {
        file << "\n  ";
    }
    file << "]\n";
    file << "}\n";

    return file.good();
}

bool OverworldMap::loadFromFile(const std::string& filePath)
{
    std::ifstream file(filePath);
    if (!file)
    {
        return false;
    }

    std::string source((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    try
    {
        JsonReader reader(source);

        reader.expect('{');

        reader.expectKey("version");
        const int version = reader.parseInt();
        if (version < 1 || version > kMapVersion)
        {
            return false;
        }
        reader.expect(',');

        reader.expectKey("player");
        reader.expect('{');
        reader.expectKey("chunkX");
        const int loadedPlayerChunkX = reader.parseInt();
        reader.expect(',');
        reader.expectKey("chunkY");
        const int loadedPlayerChunkY = reader.parseInt();
        reader.expect(',');
        reader.expectKey("tileX");
        const int loadedPlayerTileX = reader.parseInt();
        reader.expect(',');
        reader.expectKey("tileY");
        const int loadedPlayerTileY = reader.parseInt();
        reader.expect('}');

        if (!isChunkInBounds(loadedPlayerChunkX, loadedPlayerChunkY) ||
            loadedPlayerTileX < 0 || loadedPlayerTileX >= ChunkWidth ||
            loadedPlayerTileY < 0 || loadedPlayerTileY >= ChunkHeight)
        {
            return false;
        }

        reader.expect(',');

        reader.expectKey("tiles");
        reader.expect('[');
        WorldChunks loadedChunks{};
        constexpr int kTotalTileCount = WorldChunkWidth * WorldChunkHeight * ChunkWidth * ChunkHeight;
        for (int tileIndex = 0; tileIndex < kTotalTileCount; ++tileIndex)
        {
            const bool passable = reader.parseBool();

            const int chunkArea = ChunkWidth * ChunkHeight;
            const int chunkFlatIndex = tileIndex / chunkArea;
            const int tileFlatIndex = tileIndex % chunkArea;
            loadedChunks[static_cast<std::size_t>(chunkFlatIndex)][static_cast<std::size_t>(tileFlatIndex)].passable =
                passable;

            if (tileIndex < kTotalTileCount - 1)
            {
                reader.expect(',');
            }
        }
        reader.expect(']');

        reader.expect(',');

        bool hasTerrainData = false;
        std::string nextKey = reader.parseString();
        reader.expect(':');

        if (nextKey == "terrain")
        {
            hasTerrainData = true;
            reader.expect('[');
            for (int tileIndex = 0; tileIndex < kTotalTileCount; ++tileIndex)
            {
                TerrainType terrain = TerrainType::Grass;
                if (!tryParseTerrain(reader.parseString(), terrain))
                {
                    return false;
                }

                const int chunkArea = ChunkWidth * ChunkHeight;
                const int chunkFlatIndex = tileIndex / chunkArea;
                const int tileFlatIndex = tileIndex % chunkArea;
                loadedChunks[static_cast<std::size_t>(chunkFlatIndex)][static_cast<std::size_t>(tileFlatIndex)].terrain =
                    terrain;

                if (tileIndex < kTotalTileCount - 1)
                {
                    reader.expect(',');
                }
            }
            reader.expect(']');

            reader.expect(',');
            reader.expectKey("entities");
            reader.expect('[');
        }
        else if (nextKey == "entities")
        {
            reader.expect('[');
        }
        else
        {
            return false;
        }

        std::vector<MapEntity> loadedEntities;
        if (!reader.tryConsume(']'))
        {
            while (true)
            {
                MapEntity entity{};
                reader.expect('{');
                reader.expectKey("id");
                entity.id = reader.parseInt();
                reader.expect(',');
                reader.expectKey("chunkX");
                entity.chunkX = reader.parseInt();
                reader.expect(',');
                reader.expectKey("chunkY");
                entity.chunkY = reader.parseInt();
                reader.expect(',');
                reader.expectKey("tileX");
                entity.tileX = reader.parseInt();
                reader.expect(',');
                reader.expectKey("tileY");
                entity.tileY = reader.parseInt();
                reader.expect(',');
                reader.expectKey("type");
                entity.type = reader.parseString();
                reader.expect('}');

                if (!isChunkInBounds(entity.chunkX, entity.chunkY) ||
                    entity.tileX < 0 || entity.tileX >= ChunkWidth ||
                    entity.tileY < 0 || entity.tileY >= ChunkHeight)
                {
                    return false;
                }

                loadedEntities.push_back(std::move(entity));

                if (reader.tryConsume(','))
                {
                    continue;
                }

                reader.expect(']');
                break;
            }
        }

        if (!hasTerrainData && version >= 2)
        {
            // Version 2 files should always include terrain data.
            return false;
        }

        PlayerState loadedPlayerState{};
        if (reader.tryConsume(','))
        {
            reader.expectKey("playerState");
            reader.expect('{');

            reader.expectKey("hasData");
            loadedPlayerState.hasData = reader.parseBool();
            reader.expect(',');

            reader.expectKey("hp");
            loadedPlayerState.hp = reader.parseInt();
            reader.expect(',');

            reader.expectKey("gold");
            loadedPlayerState.gold = reader.parseInt();
            reader.expect(',');

            reader.expectKey("xp");
            loadedPlayerState.xp = reader.parseInt();
            reader.expect(',');

            reader.expectKey("equippedItemId");
            loadedPlayerState.equippedItemId = reader.parseInt();
            reader.expect(',');

            reader.expectKey("inventory");
            reader.expect('[');
            if (!reader.tryConsume(']'))
            {
                while (true)
                {
                    PlayerInventoryEntry entry{};
                    reader.expect('{');
                    reader.expectKey("id");
                    entry.id = reader.parseInt();
                    reader.expect(',');
                    reader.expectKey("quantity");
                    entry.quantity = reader.parseInt();
                    reader.expect(',');
                    reader.expectKey("equipped");
                    entry.equipped = reader.parseBool();
                    reader.expect('}');

                    if (entry.quantity < 0)
                    {
                        return false;
                    }

                    loadedPlayerState.inventory.push_back(entry);

                    if (reader.tryConsume(','))
                    {
                        continue;
                    }

                    reader.expect(']');
                    break;
                }
            }

            reader.expect('}');
        }

        std::vector<EnemySpawnTile> loadedEnemySpawnTiles;
        if (reader.tryConsume(','))
        {
            reader.expectKey("enemySpawnTiles");
            reader.expect('[');
            if (!reader.tryConsume(']'))
            {
                while (true)
                {
                    EnemySpawnTile spawnTile{};
                    reader.expect('{');
                    reader.expectKey("chunkX");
                    spawnTile.chunkX = reader.parseInt();
                    reader.expect(',');
                    reader.expectKey("chunkY");
                    spawnTile.chunkY = reader.parseInt();
                    reader.expect(',');
                    reader.expectKey("tileX");
                    spawnTile.tileX = reader.parseInt();
                    reader.expect(',');
                    reader.expectKey("tileY");
                    spawnTile.tileY = reader.parseInt();
                    reader.expect('}');

                    if (!isChunkInBounds(spawnTile.chunkX, spawnTile.chunkY) ||
                        spawnTile.tileX < 0 || spawnTile.tileX >= ChunkWidth ||
                        spawnTile.tileY < 0 || spawnTile.tileY >= ChunkHeight)
                    {
                        return false;
                    }

                    loadedEnemySpawnTiles.push_back(spawnTile);

                    if (reader.tryConsume(','))
                    {
                        continue;
                    }

                    reader.expect(']');
                    break;
                }
            }
        }

        std::map<int, EntityMetadata> loadedEntityMetadata;
        if (reader.tryConsume(','))
        {
            reader.expectKey("entityMetadata");
            reader.expect('[');
            if (!reader.tryConsume(']'))
            {
                while (true)
                {
                    EntityMetadata metadata{};
                    reader.expect('{');

                    reader.expectKey("entityId");
                    int entityId = reader.parseInt();
                    reader.expect(',');

                    reader.expectKey("data");
                    reader.expect('{');

                    if (!reader.tryConsume('}'))
                    {
                        while (true)
                        {
                            const std::string key = reader.parseString();
                            reader.expect(':');

                            if (key == "npcName")
                            {
                                metadata.npcName = reader.parseString();
                            }
                            else if (key == "npcDescription")
                            {
                                metadata.npcDescription = reader.parseString();
                            }
                            else if (key == "dialogue")
                            {
                                metadata.dialogue = reader.parseString();
                            }
                            else if (key == "xpReward")
                            {
                                metadata.xpReward = reader.parseInt();
                            }
                            else if (key == "shopName" || key == "storeName")
                            {
                                metadata.shopName = reader.parseString();
                            }
                            else if (key == "shopNpcName" || key == "storeNpcName")
                            {
                                metadata.shopNpcName = reader.parseString();
                            }
                            else if (key == "shopInventory" || key == "storeInventory")
                            {
                                metadata.shopInventory = reader.parseString();
                            }
                            else if (key == "dialogueTree")
                            {
                                reader.expect('[');
                                if (!reader.tryConsume(']'))
                                {
                                    while (true)
                                    {
                                        EntityMetadata::DialogueNode node{};
                                        reader.expect('{');
                                        reader.expectKey("id");
                                        node.id = reader.parseInt();
                                        reader.expect(',');
                                        reader.expectKey("npcText");
                                        node.npcText = reader.parseString();
                                        reader.expect(',');
                                        reader.expectKey("options");
                                        reader.expect('[');

                                        if (!reader.tryConsume(']'))
                                        {
                                            while (true)
                                            {
                                                EntityMetadata::DialogueOption option{};
                                                reader.expect('{');
                                                reader.expectKey("text");
                                                option.text = reader.parseString();
                                                reader.expect(',');
                                                reader.expectKey("nextNodeId");
                                                option.nextNodeId = reader.parseInt();
                                                reader.expect('}');

                                                node.options.push_back(std::move(option));

                                                if (reader.tryConsume(','))
                                                {
                                                    continue;
                                                }

                                                reader.expect(']');
                                                break;
                                            }
                                        }

                                        reader.expect('}');
                                        metadata.dialogueTree.push_back(std::move(node));

                                        if (reader.tryConsume(','))
                                        {
                                            continue;
                                        }

                                        reader.expect(']');
                                        break;
                                    }
                                }
                            }
                            else
                            {
                                throw std::runtime_error("Unknown entity metadata key");
                            }

                            if (reader.tryConsume(','))
                            {
                                continue;
                            }

                            reader.expect('}');
                            break;
                        }
                    }

                    loadedEntityMetadata[entityId] = metadata;
                    reader.expect('}');

                    if (reader.tryConsume(','))
                    {
                        continue;
                    }

                    reader.expect(']');
                    break;
                }
            }
        }

        reader.expect('}');
        reader.ensureFullyConsumed();

        m_chunks = loadedChunks;
        m_entities = std::move(loadedEntities);
        m_playerChunkX = loadedPlayerChunkX;
        m_playerChunkY = loadedPlayerChunkY;
        m_playerTileX = loadedPlayerTileX;
        m_playerTileY = loadedPlayerTileY;
        m_playerState = std::move(loadedPlayerState);
        m_enemySpawnTiles = std::move(loadedEnemySpawnTiles);
        m_entityMetadata = std::move(loadedEntityMetadata);
        return true;
    }
    catch (...)
    {
        return false;
    }
}

int OverworldMap::getPlayerChunkX() const
{
    return m_playerChunkX;
}

int OverworldMap::getPlayerChunkY() const
{
    return m_playerChunkY;
}

int OverworldMap::getPlayerTileX() const
{
    return m_playerTileX;
}

int OverworldMap::getPlayerTileY() const
{
    return m_playerTileY;
}

void OverworldMap::addEntity(int id, int chunkX, int chunkY, int tileX, int tileY, const std::string& type)
{
    m_entities.push_back({id, chunkX, chunkY, tileX, tileY, type});
}

bool OverworldMap::placeOrReplaceEntity(int chunkX, int chunkY, int tileX, int tileY, const std::string& type)
{
    if (!isChunkInBounds(chunkX, chunkY) ||
        tileX < 0 || tileX >= ChunkWidth ||
        tileY < 0 || tileY >= ChunkHeight)
    {
        return false;
    }

    std::string normalizedType = type;
    for (char& c : normalizedType)
    {
        c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    }

    if (normalizedType.empty())
    {
        return false;
    }

    int nextEntityId = 1;
    for (const auto& entity : m_entities)
    {
        nextEntityId = std::max(nextEntityId, entity.id + 1);
    }

    for (auto& entity : m_entities)
    {
        if (entity.chunkX == chunkX && entity.chunkY == chunkY &&
            entity.tileX == tileX && entity.tileY == tileY)
        {
            entity.type = normalizedType;
            return true;
        }
    }

    m_entities.push_back({nextEntityId, chunkX, chunkY, tileX, tileY, normalizedType});
    return true;
}

bool OverworldMap::removeEntityAtPosition(int chunkX, int chunkY, int tileX, int tileY)
{
    const auto originalSize = m_entities.size();
    m_entities.erase(
        std::remove_if(m_entities.begin(), m_entities.end(),
            [chunkX, chunkY, tileX, tileY](const MapEntity& entity)
            {
                return entity.chunkX == chunkX && entity.chunkY == chunkY &&
                       entity.tileX == tileX && entity.tileY == tileY;
            }),
        m_entities.end());

    return m_entities.size() != originalSize;
}

const OverworldMap::MapEntity* OverworldMap::getEntityAtPosition(int chunkX, int chunkY, int tileX, int tileY) const
{
    for (const auto& entity : m_entities)
    {
        if (entity.chunkX == chunkX && entity.chunkY == chunkY &&
            entity.tileX == tileX && entity.tileY == tileY)
        {
            return &entity;
        }
    }

    return nullptr;
}

const std::vector<OverworldMap::MapEntity>& OverworldMap::getEntities() const
{
    return m_entities;
}

void OverworldMap::addEnemySpawnTile(int chunkX, int chunkY, int tileX, int tileY)
{
    m_enemySpawnTiles.push_back({chunkX, chunkY, tileX, tileY});
}

bool OverworldMap::removeEnemySpawnTile(int chunkX, int chunkY, int tileX, int tileY)
{
    const auto originalSize = m_enemySpawnTiles.size();
    m_enemySpawnTiles.erase(
        std::remove_if(m_enemySpawnTiles.begin(), m_enemySpawnTiles.end(),
            [chunkX, chunkY, tileX, tileY](const EnemySpawnTile& tile)
            {
                return tile.chunkX == chunkX && tile.chunkY == chunkY &&
                       tile.tileX == tileX && tile.tileY == tileY;
            }),
        m_enemySpawnTiles.end());

    return m_enemySpawnTiles.size() != originalSize;
}

bool OverworldMap::isEnemySpawnTileAtPosition(int chunkX, int chunkY, int tileX, int tileY) const
{
    for (const auto& spawnTile : m_enemySpawnTiles)
    {
        if (spawnTile.chunkX == chunkX && spawnTile.chunkY == chunkY &&
            spawnTile.tileX == tileX && spawnTile.tileY == tileY)
        {
            return true;
        }
    }

    return false;
}

void OverworldMap::setPlayerState(const PlayerState& playerState)
{
    m_playerState = playerState;
}

const OverworldMap::PlayerState* OverworldMap::getPlayerState() const
{
    return m_playerState.hasData ? &m_playerState : nullptr;
}

void OverworldMap::setEntityMetadata(int entityId, const EntityMetadata& metadata)
{
    m_entityMetadata[entityId] = metadata;
}

const OverworldMap::EntityMetadata* OverworldMap::getEntityMetadata(int entityId) const
{
    auto it = m_entityMetadata.find(entityId);
    if (it != m_entityMetadata.end())
    {
        return &it->second;
    }
    return nullptr;
}

int OverworldMap::tileIndexFor(int x, int y)
{
    return y * ChunkWidth + x;
}

int OverworldMap::chunkIndexFor(int chunkX, int chunkY)
{
    return chunkY * WorldChunkWidth + chunkX;
}

bool OverworldMap::isChunkInBounds(int chunkX, int chunkY)
{
    return chunkX >= 0 && chunkX < WorldChunkWidth &&
           chunkY >= 0 && chunkY < WorldChunkHeight;
}

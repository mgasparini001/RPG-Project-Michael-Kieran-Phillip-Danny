#pragma once

#include <cstdint>

// current mode of the game
enum class GameMode : std::uint8_t
{
    Exploring = 0,
    InBattle = 1,
    InShop = 2,
    InDialogue = 3
};

struct GameState
{
    GameMode mode = GameMode::Exploring;
    int battleEnemyId = -1;
    int shopNpcId = -1;
    int dialogueNpcId = -1;
};

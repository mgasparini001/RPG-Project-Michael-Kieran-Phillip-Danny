#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include <vector>
class enemy;
class player;
class ItemRegistry;
class Inventory;

// same battle text as console version, but returned as a single string for on-screen ui
std::string buildBattleDisplayText(enemy& enemy, player& p1, const ItemRegistry& registry);

class BattleEncounter
{
public:
	BattleEncounter(std::unique_ptr<enemy> enemy,
					player& playerRef,
					ItemRegistry& registry,
					bool canRun,
					bool isWild);

	void handleKey(sf::Keyboard::Scancode scancode);
	void render(sf::RenderWindow& window, const sf::Font& font) const;
	bool shouldClose() const;
	std::string getEndMessage() const;

private:
	enum class Step
	{
		Intro,
		MainChoice,
		AttackChoice,
		ItemChoice,
		ResolveMessage
	};

	void resolvePlayerAttack(bool meleeAttack);
	void resolvePlayerRest();
	void resolvePlayerRunAttempt();
	void resolvePlayerItemUse();
	void resolveEnemyTurn();
	void finishBattle(const std::string& endMessage);
	std::vector<int> getItemIds() const;
	std::string buildOverlayText() const;

	std::unique_ptr<enemy> m_enemy;
	player& m_player;
	ItemRegistry& m_registry;
	bool m_canRun = true;
	bool m_isWild = true;
	bool m_finished = false;
	bool m_closeRequested = false;
	int m_healRuns = 1;
	int m_itemSelectionIndex = 0;
	Step m_step = Step::Intro;
	std::string m_statusMessage;
	std::string m_promptMessage;
	std::string m_endMessage;
};

// Input: an enemy and a player    Output: an interactive battle loop that reverts to main menu upon ending
// desc: displays enemy name and hp, and starts battle sequence loop, letting user either fight, rest, or flee. uses Character hp, stamina, dmg, and armor penetration (strength)
// lets player decide on melee or ranged attack, with ranged having a chance to miss but taking less stamina

void enterBattle(enemy& enemy, player& p1, ItemRegistry& registry, Inventory& inv, bool canRun, bool isWild);
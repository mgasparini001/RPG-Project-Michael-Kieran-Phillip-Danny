#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
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
					bool isWild, bool enemyRun);

	void handleKey(sf::Keyboard::Scancode scancode);
	void render(sf::RenderWindow& window, const sf::Font& font) const;
	bool shouldClose() const;
	std::string getEndMessage() const;
	bool hasCombatStarted() const;
	bool consumeCombatStartSignal();

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
	bool m_enemyRun = true;
	bool m_finished = false;
	bool m_closeRequested = false;
	bool m_combatStarted = false;
	bool m_combatStartSignal = false;
	int m_healRuns = 1;
	int m_itemSelectionIndex = 0;
	Step m_step = Step::Intro;
	std::string m_statusMessage;
	std::string m_promptMessage;
	std::string m_endMessage;

	// sound effects
	sf::SoundBuffer m_attackBuffer;
	sf::SoundBuffer m_restBuffer;
	sf::SoundBuffer m_fleeBuffer;
	std::unique_ptr<sf::Sound> m_attackSound;
	std::unique_ptr<sf::Sound> m_restSound;
	std::unique_ptr<sf::Sound> m_fleeSound;
};
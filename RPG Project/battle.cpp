#include "battle.h"
#include <iostream>
#include "Inventory.h"
#include <SFML/Audio.hpp>
#include "character.h"
#include "enemy.h"
#include "player.h"
#include "ItemRegistry.h"
#include "errorChecking.h"
#include <filesystem>
#include <algorithm>
#include <sstream>
using namespace std;


//copy pasted fix from client.cpp since the music and everything needs to be reloaded

namespace
{
std::filesystem::path resolveBattleAssetPath(const std::string& fileName)
{
	const std::filesystem::path cwd = std::filesystem::current_path();
	const std::filesystem::path candidates[] = {
		cwd / fileName,
		cwd / "RPG Project" / fileName,
		cwd / "x64" / "Debug" / fileName,
		cwd.parent_path() / "RPG Project" / fileName,
		cwd.parent_path() / fileName
	};

	for (const auto& candidate : candidates)
	{
		if (std::filesystem::exists(candidate))
		{
			return candidate;
		}
	}

	return cwd / fileName;
}
}


// Input: an enemy and a player     Output: Player and enemy stats
// desc: prints out player and enemy in battle and an action prompt for player

std::string buildBattleDisplayText(enemy& enemy, player& p1, const ItemRegistry& registry)
{
	std::ostringstream out;
	out << endl << "ENEMY:" << endl;
	out << enemy.getName() << endl;
	out << "\nHP: " << enemy.getHp() << endl;
	out << R"(



---------------------------------------------
)";

	out << "\n\t\t\t" << "PLAYER:";
	out << "\n\t\t\t" << p1.getName() << endl << endl;
	out << "\t\t\t" << "HP: " << p1.getHp() << endl;
	out << "\t\t\t" << "SP: " << p1.getStamina() << endl;
	out << "\t\t\t" << "AP: " << p1.getAp() << endl;
	out << "\t\t\t" << "DMG: " << p1.getDmg() << endl;
	out <<
		R"(
                       ---------------------
		      | 1. FIGHT   3. RUN   |
		      | 2. REST    4. ITEM  |
                       ---------------------
)";
	if (p1.getHasItemEquipped() == true)
	{
		out << "\t\t\tEquipped: " << registry.getItemName(p1.getEquippedItemID()) << std::endl << std::endl;
	}

	return out.str();
}

BattleEncounter::BattleEncounter(std::unique_ptr<enemy> enemy,
                                 player& playerRef,
                                 ItemRegistry& registry,
                                 bool canRun,
                                 bool isWild)
	: m_enemy(std::move(enemy)),
	  m_player(playerRef),
	  m_registry(registry),
	  m_canRun(canRun),
	  m_isWild(isWild)
{
	m_statusMessage = m_isWild
	    ? ("A wild " + m_enemy->getName() + " appears!")
	    : (m_enemy->getName() + " challenges you to a duel!");
	m_promptMessage = "Press Enter to start battle";

	// load sound effects
	const std::filesystem::path attackPath = resolveBattleAssetPath("Attack.wav");
	const std::filesystem::path restPath = resolveBattleAssetPath("eppy.wav");
	const std::filesystem::path fleePath = resolveBattleAssetPath("Scream.wav");

	if (m_attackBuffer.loadFromFile(attackPath.string()))
	{
		m_attackSound = std::make_unique<sf::Sound>(m_attackBuffer);
	}
	if (m_restBuffer.loadFromFile(restPath.string()))
	{
		m_restSound = std::make_unique<sf::Sound>(m_restBuffer);
	}
	if (m_fleeBuffer.loadFromFile(fleePath.string()))
	{
		m_fleeSound = std::make_unique<sf::Sound>(m_fleeBuffer);
	}
}

void BattleEncounter::handleKey(sf::Keyboard::Scancode scancode)
{
	if (m_closeRequested || !m_enemy)
	{
		return;
	}

	if (m_step == Step::Intro)
	{
		if (scancode == sf::Keyboard::Scancode::Enter)
		{
			m_step = Step::MainChoice;
			m_promptMessage = "1 Fight  2 Rest  3 Run  4 Item";
		}
		return;
	}

	if (m_step == Step::ResolveMessage)
	{
		if (scancode == sf::Keyboard::Scancode::Enter)
		{
			if (m_finished)
			{
				m_closeRequested = true;
			}
			else
			{
				m_step = Step::MainChoice;
				m_promptMessage = "1 Fight  2 Rest  3 Run  4 Item";
			}
		}
		return;
	}

	if (m_step == Step::AttackChoice)
	{
		if (scancode == sf::Keyboard::Scancode::Num1 || scancode == sf::Keyboard::Scancode::Numpad1)
		{
			resolvePlayerAttack(true);
		}
		else if (scancode == sf::Keyboard::Scancode::Num2 || scancode == sf::Keyboard::Scancode::Numpad2)
		{
			resolvePlayerAttack(false);
		}
		else if (scancode == sf::Keyboard::Scancode::Escape)
		{
			m_step = Step::MainChoice;
			m_promptMessage = "1 Fight  2 Rest  3 Run  4 Item";
		}
		return;
	}

	if (m_step == Step::ItemChoice)
	{
		const auto ids = getItemIds();
		if (scancode == sf::Keyboard::Scancode::Escape)
		{
			m_step = Step::MainChoice;
			m_promptMessage = "1 Fight  2 Rest  3 Run  4 Item";
			return;
		}

		if (ids.empty())
		{
			m_statusMessage = "No items in inventory";
			m_promptMessage = "Press Enter";
			m_step = Step::ResolveMessage;
			return;
		}

		if (scancode == sf::Keyboard::Scancode::Up && m_itemSelectionIndex > 0)
		{
			--m_itemSelectionIndex;
		}
		else if (scancode == sf::Keyboard::Scancode::Down && m_itemSelectionIndex < static_cast<int>(ids.size()) - 1)
		{
			++m_itemSelectionIndex;
		}
		else if (scancode == sf::Keyboard::Scancode::Enter)
		{
			resolvePlayerItemUse();
		}
		return;
	}

	if (m_step == Step::MainChoice)
	{
		if (scancode == sf::Keyboard::Scancode::Num1 || scancode == sf::Keyboard::Scancode::Numpad1)
		{
			m_step = Step::AttackChoice;
			m_promptMessage = "Choose attack style (Esc back)";
		}
		else if (scancode == sf::Keyboard::Scancode::Num2 || scancode == sf::Keyboard::Scancode::Numpad2)
		{
			resolvePlayerRest();
		}
		else if (scancode == sf::Keyboard::Scancode::Num3 || scancode == sf::Keyboard::Scancode::Numpad3)
		{
			resolvePlayerRunAttempt();
		}
		else if (scancode == sf::Keyboard::Scancode::Num4 || scancode == sf::Keyboard::Scancode::Numpad4)
		{
			m_step = Step::ItemChoice;
			m_promptMessage = "Up/Down pick item, Enter equip/use, Esc back";
		}
	}
}

void BattleEncounter::render(sf::RenderWindow& window, const sf::Font& font) const
{
	if (!m_enemy || font.getInfo().family.empty())
	{
		return;
	}

	const sf::Vector2u size = window.getSize();
	sf::RectangleShape overlay({static_cast<float>(size.x), static_cast<float>(size.y)});
	overlay.setFillColor(sf::Color(0, 0, 0, 170));
	window.draw(overlay);

	sf::RectangleShape panel({static_cast<float>(size.x) - 140.0f, static_cast<float>(size.y) - 150.0f});
	panel.setPosition({70.0f, 70.0f});
	panel.setFillColor(sf::Color(15, 20, 27, 242));
	panel.setOutlineColor(sf::Color(210, 222, 236));
	panel.setOutlineThickness(2.0f);
	window.draw(panel);

	float y = panel.getPosition().y + 20.0f;
	const std::string text = buildOverlayText();
	std::size_t start = 0;
	while (start <= text.size())
	{
		const std::size_t end = text.find('\n', start);
		const std::string line = (end == std::string::npos) ? text.substr(start) : text.substr(start, end - start);
		if (!line.empty())
		{
			sf::Text drawText(font, line, 21);
			drawText.setFillColor(sf::Color(230, 236, 242));
			drawText.setPosition({panel.getPosition().x + 22.0f, y});
			window.draw(drawText);
			y += 28.0f;
		}
		else
		{
			y += 10.0f;
		}

		if (end == std::string::npos)
		{
			break;
		}
		start = end + 1;
	}
}

bool BattleEncounter::shouldClose() const
{
	return m_closeRequested;
}

std::string BattleEncounter::getEndMessage() const
{
	return m_endMessage;
}

void BattleEncounter::resolvePlayerAttack(bool meleeAttack)
{
	if (!m_enemy)
	{
		return;
	}

	const int enemyHpBefore = m_enemy->getHp();
	const int staminaBefore = m_player.getStamina();
	const bool attackWorked = m_player.attack(*m_enemy, meleeAttack);
	const int enemyHpAfter = m_enemy->getHp();
	const int staminaAfter = m_player.getStamina();

	if (!attackWorked)
	{
		if (staminaAfter == staminaBefore)
		{
			m_statusMessage = "You are too exhausted for that attack";
		}
		else
		{
			m_statusMessage = meleeAttack ? "Qwaping, the attack did nothing" : "The attack missed";
		}
	}
	else
	{
		if (m_attackSound)
		{
			m_attackSound->play();
		}
		m_statusMessage = "You hit " + m_enemy->getName() + " for " +
		                  std::to_string(std::max(0, enemyHpBefore - enemyHpAfter)) + " damage";
	}

	if (m_enemy->getHp() > 0)
	{
		resolveEnemyTurn();
	}
	else
	{
		finishBattle("Enemy defeated");
		return;
	}

	if (m_finished)
	{
		return;
	}

	m_promptMessage = "Press Enter";
	m_step = Step::ResolveMessage;
}

void BattleEncounter::resolvePlayerRest()
{
	m_player.rest();
	if (m_restSound)
	{
		m_restSound->play();
	}
	m_statusMessage = m_player.getName() + " rests and restores some stamina";

	if (m_enemy && m_enemy->getHp() > 0)
	{
		resolveEnemyTurn();
	}

	if (m_finished)
	{
		return;
	}

	m_promptMessage = "Press Enter";
	m_step = Step::ResolveMessage;
}

void BattleEncounter::resolvePlayerRunAttempt()
{
	if (!m_canRun)
	{
		m_statusMessage = "You can't run from a fight like this";
	}
	else
	{
		const bool escaped = m_player.flee();
		if (m_fleeSound)
		{
			m_fleeSound->play();
		}
		m_statusMessage = escaped ? "You successfully escaped" : "You failed to escape";
		if (escaped)
		{
			finishBattle("You escaped the battle");
			return;
		}
	}

	if (m_enemy && m_enemy->getHp() > 0)
	{
		resolveEnemyTurn();
	}

	if (m_finished)
	{
		return;
	}

	m_promptMessage = "Press Enter";
	m_step = Step::ResolveMessage;
}

void BattleEncounter::resolvePlayerItemUse()
{
	const auto ids = getItemIds();
	if (ids.empty())
	{
		m_statusMessage = "No items in inventory";
		m_promptMessage = "Press Enter";
		m_step = Step::ResolveMessage;
		return;
	}

	m_itemSelectionIndex = std::clamp(m_itemSelectionIndex, 0, static_cast<int>(ids.size()) - 1);
	const int itemId = ids[static_cast<std::size_t>(m_itemSelectionIndex)];

	if (!m_player.equipItem(itemId))
	{
		m_statusMessage = "you cant equip an item you dont have, dingus";
	}
	else if (m_registry.getItemisConsumable(itemId))
	{
		m_player.removeItemFromInventory(itemId, m_player, 1);
		m_statusMessage = "Consumed " + m_registry.getItemName(itemId);
	}
	else
	{
		m_statusMessage = "Equipped " + m_registry.getItemName(itemId);
	}

	if (m_enemy && m_enemy->getHp() > 0)
	{
		resolveEnemyTurn();
	}

	if (m_finished)
	{
		return;
	}

	m_promptMessage = "Press Enter";
	m_step = Step::ResolveMessage;
}

void BattleEncounter::resolveEnemyTurn()
{
	if (!m_enemy || m_enemy->getHp() <= 0)
	{
		return;
	}

	const int eRoll = m_enemy->diceRoll(10);
	if (eRoll <= 5)
	{
		const int hpBefore = m_player.getHp();
		if (m_enemy->attack(m_player, true))
		{
			if (m_attackSound)
			{
				m_attackSound->play();
			}
			m_statusMessage += "\n" + m_enemy->getName() + " hits you for " +
			                   std::to_string(std::max(0, hpBefore - m_player.getHp())) + " damage";
		}
		else
		{
			m_statusMessage += "\n" + m_enemy->getName() + " attack missed";
		}
	}
	else if (eRoll <= 7)
	{
		m_enemy->rest();
		if (m_restSound)
		{
			m_restSound->play();
		}
		m_statusMessage += "\n" + m_enemy->getName() + " rests and restores stamina";
	}
	else if (eRoll <= 9 && m_healRuns <= 3)
	{
		m_enemy->heal();
		++m_healRuns;
		if (m_restSound)
		{
			m_restSound->play();
		}
		m_statusMessage += "\n" + m_enemy->getName() + " rests and restores health";
	}
	else
	{
		const bool escaped = m_enemy->flee();
		if (escaped)
		{
			if (m_fleeSound)
			{
				m_fleeSound->play();
			}
			finishBattle(m_enemy->getName() + " ran away");
		}
		m_statusMessage += escaped
		    ? ("\n" + m_enemy->getName() + " escaped")
		    : ("\n" + m_enemy->getName() + " failed to escape");
	}

	if (m_player.getHp() <= 0)
	{
		finishBattle("You were defeated");
	}
}

void BattleEncounter::finishBattle(const std::string& endMessage)
{
	m_finished = true;
	m_endMessage = endMessage;
	m_step = Step::ResolveMessage;
	m_promptMessage = "Press Enter";
}

std::vector<int> BattleEncounter::getItemIds() const
{
	std::vector<int> ids;
	InventoryNode* current = m_player.getInventory().getHead();
	while (current != nullptr)
	{
		if (current->item && current->quantity > 0)
		{
			ids.push_back(current->item->getId());
		}
		current = current->next;
	}
	return ids;
}

std::string BattleEncounter::buildOverlayText() const
{
	if (!m_enemy)
	{
		return {};
	}

	std::ostringstream out;
	out << buildBattleDisplayText(*m_enemy, m_player, m_registry);
	if (!m_statusMessage.empty())
	{
		out << "\n" << m_statusMessage;
	}
	if (!m_promptMessage.empty())
	{
		out << "\n" << m_promptMessage;
	}

	if (m_step == Step::AttackChoice)
	{
		out << "\n1. Melee Attack (Costs 100 stamina)";
		out << "\n2. Ranged Attack (Costs 50 stamina)";
	}
	else if (m_step == Step::ItemChoice)
	{
		out << "\nITEMS";
		const auto ids = getItemIds();
		if (ids.empty())
		{
			out << "\nNo items in inventory";
		}
		else
		{
			for (int i = 0; i < static_cast<int>(ids.size()); ++i)
			{
				const int itemId = ids[static_cast<std::size_t>(i)];
				const int qty = m_player.getInventory().getQuantity(itemId);
				out << "\n" << ((i == m_itemSelectionIndex) ? "> " : "  ")
				    << itemId << ") " << m_registry.getItemName(itemId) << " x" << qty;
			}
		}
	}

	return out.str();
}

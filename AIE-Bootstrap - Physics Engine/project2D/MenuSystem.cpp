#include "MenuSystem.h"
#include <string>
#include "Player.h"
#include "Application2D.h"
#include <sstream>
#include "Texture.h"

MenuSystem::MenuSystem(Application2D* app, Player* player, UpgradeData* upgradeData, aie::Texture* tutorialImage) : m_app(app), m_player(player), m_upgradeData(upgradeData), m_tutorialImage(tutorialImage)
{
	if (m_upgradeData != nullptr && m_player != nullptr)
	{
		m_upgradeData->m_currentForce = m_player->GetMaxForce();
		m_upgradeData->m_currentHealth = m_player->GetMaxHealth();
		m_upgradeData->m_currentEnergy = m_player->GetMaxEnergy();
	}
}

void MenuSystem::UpdateMenu()
{
	if (m_currentGameState == GameState::MENU || m_currentGameState == GameState::TUTORIAL)
	{
		MainMenu();
	}
}

void MenuSystem::MainMenu()
{
	ImGui::Begin("Main Menu");
	
	std::string text = "Play Game";
	if (ImGui::Button(text.c_str(), ImVec2(200, 20))) // Button to start the game
	{
		m_currentGameState = GameState::GAME;
		m_app->InitialiseGame();
	}

	text = "Tutorial";
	if (ImGui::Button(text.c_str(), ImVec2(200, 20))) // Button to show the tutorial screen
	{
		m_currentGameState = GameState::TUTORIAL;
		aie::Gizmos::clear();
	}

	if (m_upgradeData != nullptr)
	{
		std::stringstream t;

		t << "Upgrade Health: " << m_upgradeData->m_healthCost;
		text = t.str();
		if (ImGui::Button(text.c_str(), ImVec2(200, 20)) && m_currentScore >= m_upgradeData->m_healthCost) // Button to upgrade player health
		{
			m_currentScore -= m_upgradeData->m_healthCost; // Decrease current score

			m_upgradeData->m_currentHealth += m_upgradeData->m_currentHealth * 1 / (100 / m_upgradeData->m_upgradeIncrease); // Increase current player health

			m_upgradeData->m_healthCost += (int)(m_upgradeData->m_healthStartCost * ((float)m_upgradeData->m_costIncrease / 100)); // Increase the cost of health upgrade
		}

		t.str("");
		t << "Upgrade Force: " << m_upgradeData->m_forceCost;
		text = t.str();
		if (ImGui::Button(text.c_str(), ImVec2(200, 20)) && m_currentScore >= m_upgradeData->m_forceCost) // Button to upgrade player force
		{
			m_currentScore -= m_upgradeData->m_forceCost; // Decrease current score

			m_upgradeData->m_currentForce += m_upgradeData->m_currentForce * 1 / (100 / m_upgradeData->m_upgradeIncrease); // Increase current player force

			m_upgradeData->m_forceCost += (int)(m_upgradeData->m_forceStartCost * ((float)m_upgradeData->m_costIncrease / 100)); // Increase the cost of force upgrade
		}

		t.str("");
		t << "Upgrade Energy: " << m_upgradeData->m_energyCost;
		text = t.str();
		if (ImGui::Button(text.c_str(), ImVec2(200, 20)) && m_currentScore >= m_upgradeData->m_energyCost) // Button to upgrade player energy
		{
			m_currentScore -= m_upgradeData->m_energyCost; // Decease current score

			m_upgradeData->m_currentEnergy += m_upgradeData->m_currentEnergy * 1 / (100 / m_upgradeData->m_upgradeIncrease); // Increase current player energy

			m_upgradeData->m_energyCost += (int)(m_upgradeData->m_energyStartCost * ((float)m_upgradeData->m_costIncrease / 100)); // Increase the cost of energy upgrade 
		}
	}

	ImGui::End();
}

void MenuSystem::UpdatePlayer(Player* player)
{
	if (m_upgradeData != nullptr && player != nullptr)
	{
		player->SetMaxHealth(m_upgradeData->m_currentHealth);
		player->SetMaxForce(m_upgradeData->m_currentForce);
		player->SetMaxEnergy(m_upgradeData->m_currentEnergy);
	}
}

aie::Texture* MenuSystem::GetTutorialTexture()
{
	if (m_currentGameState == GameState::TUTORIAL)
	{
		return m_tutorialImage;
	}
	return nullptr;
}

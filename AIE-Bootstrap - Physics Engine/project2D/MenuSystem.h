#pragma once
#include <imgui.h>
#include "Application2D.h"

enum GameState
{
	MENU,
	TUTORIAL,
	GAME
};

class MenuSystem
{
public:
	// Data for upgrading player skills
	class UpgradeData
	{
	public:
		int m_healthStartCost = 0; // How much health upgrades will cost to start with
		int m_forceStartCost = 0; // How much force upgrades will cost to start with
		int m_energyStartCost = 0; // How much energy upgrades will cost to start with

		int m_healthCost = 0; // Current cost of health upgrades
		int m_forceCost = 0; // Current cost of force upgrades
		int m_energyCost = 0; // Current cost of energy upgrades 

		int m_currentHealth = 10; // Current player health
		float m_currentForce = 50; // Current player force
		float m_currentEnergy = 100; // Current player energy

		int m_upgradeIncrease = 0; // How much attributes will increasde by with an upgrade 
		int m_costIncrease = 0; // How much more each upgrade will cost

		UpgradeData(int healthStartCost, int forceStartCost, int energyStartCost, int upgradeIncrease, int costIncrease)
		{
			m_healthStartCost = healthStartCost;
			m_forceStartCost = forceStartCost;
			m_energyStartCost = energyStartCost;

			m_healthCost = m_healthStartCost;
			m_forceCost = m_forceStartCost;
			m_energyCost = m_energyStartCost;

			m_upgradeIncrease = upgradeIncrease;
			m_costIncrease = costIncrease;
		}
	};

	MenuSystem(Application2D* app, Player* player, UpgradeData* upgradeData, aie::Texture* tutorialImage);

	void UpdateMenu();

	void MainMenu();

	void UpdatePlayer(Player* player);

	aie::Texture* GetTutorialTexture();

	GameState m_currentGameState = GameState::MENU;
	int m_currentScore = 0;

protected:
	Application2D* m_app = nullptr;
	Player* m_player = nullptr;
	UpgradeData* m_upgradeData = nullptr;

	aie::Texture* m_tutorialImage = nullptr;
};


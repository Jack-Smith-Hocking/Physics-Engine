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
	class UpgradeData
	{
	public:
		int m_healthStartCost = 0;
		int m_forceStartCost = 0;
		int m_energyStartCost = 0;

		int m_healthCost = 0;
		int m_forceCost = 0;
		int m_energyCost = 0;

		int m_currentHealth = 10;
		float m_currentForce = 50;
		float m_currentEnergy = 100;

		int m_upgradeIncrease = 0;
		int m_costIncrease = 0;

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


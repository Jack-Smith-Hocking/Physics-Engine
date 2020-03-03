#include "Application2D.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include "Gizmos.h"
#include "glm/ext.hpp"
#include <imgui.h>

#include <iostream>
#include <sstream>

#include "PhysicsScene.h"
#include "LevelManage.h"
#include "MenuSystem.h"

#include "Circle.h"
#include "Plane.h"
#include "Spring.h"

#include "OBB.h"
#include "AABB.h"

#include "Player.h"
#include "CollectibleCircle.h"

glm::vec2 g_windowDimensions;

Application2D::Application2D() 
{

}

Application2D::~Application2D() 
{

}

bool Application2D::startup() 
{
	srand((unsigned int)time(NULL));

	g_windowDimensions.x = (float)getWindowWidth();
	g_windowDimensions.y = (float)getWindowHeight();

	setVSync(false);
	m_2dRenderer = new aie::Renderer2D();

	m_menu = new MenuSystem(this, m_player, new MenuSystem::UpgradeData(1000, 1000, 1000, 15, 25), new aie::Texture("./textures/Tutorial Screen.PNG"));

	m_texture = new aie::Texture("./textures/numbered_grid.tga");

	m_font = new aie::Font("./font/consolas.ttf", 32);
	
	m_timer = 0;

	aie::Gizmos::create(255U, 255U, 65535U, 65535U);

	InitialiseGame();

	return true;
}

void Application2D::shutdown() 
{
	if (m_menu != nullptr)
	{
		delete m_menu;
	}
	if (m_levelManager != nullptr)
	{
		delete m_levelManager;
	}
	if (m_physicsScene != nullptr)
	{
		delete m_physicsScene;
	}

	delete m_font;
	delete m_texture;
	delete m_shipTexture;
	delete m_2dRenderer;
}

void Application2D::update(float deltaTime) 
{
	g_windowDimensions.x = (float)getWindowWidth();
	g_windowDimensions.y = (float)getWindowHeight();

	m_timer += deltaTime;

	// input example
	aie::Input* input = aie::Input::getInstance();

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();

	if (input->isKeyDown(aie::INPUT_KEY_R) && m_menu->m_currentGameState == GameState::GAME)
	{
		m_menu->m_currentGameState = GameState::MENU;
		m_menu->m_currentScore += m_player->GetCurrentScore();
	}

	//aie::Gizmos::clear();

	GameState gs = m_menu->m_currentGameState;

	switch (gs)
	{
	case MENU:
		m_menu->UpdateMenu();
		break;
	
	case GAME:
		m_levelManager->Update(deltaTime);
		break;
	
	default:
		break;
	}

	if (m_player->m_destroyObject && m_menu->m_currentGameState != GameState::MENU && m_menu->m_currentGameState != GameState::TUTORIAL)
	{
		m_menu->m_currentGameState = GameState::MENU;
		m_menu->m_currentScore += m_player->GetCurrentScore();
	}
}

void Application2D::draw() 
{
	// wipe the screen to the background colour
	clearScreen();

	// begin drawing sprites
	m_2dRenderer->begin();

	if (m_menu != nullptr)
	{
		aie::Texture* tut = m_menu->GetTutorialTexture();
		if (tut != nullptr)
		{
			m_2dRenderer->drawSprite(tut, getWindowWidth() / 2, getWindowHeight() / 2, tut->getWidth(), tut->getHeight());
			m_menu->UpdateMenu();
		}
	}

	static float aspectRatio = 16 / 9.0f;
	aie::Gizmos::draw2D(glm::ortho<float>(-100, 100, -100 / aspectRatio, 100 / aspectRatio, -1.0f, 1.0f));

	// output some text, uses the last used colour
	//char fps[32];
	//sprintf_s(fps, 32, "FPS: %i", getFPS());
	//m_2dRenderer->drawText(m_font, fps, 0, getWindowHeight() - 32);
	
	m_2dRenderer->drawText(m_font, "Press ESC to quit!", 0, getWindowHeight() - 32);

	if (m_player != nullptr && m_menu->m_currentGameState == GameState::GAME)
	{
		float xOffset = 20;

		//std::stringstream h;
		//h << "Health: " << m_player->GetCurrentHealth() << " / " << m_player->GetMaxHealth();
		//std::string health = h.str();

		//m_2dRenderer->drawText(m_font, health.c_str(), (float)getWindowWidth() - m_font->getStringWidth(health.c_str()) - xOffset, (float)getWindowHeight() - 50);

		//std::stringstream e;
		//e << "Energy: " << (int)m_player->GetCurrentEnergy() << " / " << (int)m_player->GetMaxEnergy();
		//std::string energy = e.str();

		//m_2dRenderer->drawText(m_font, energy.c_str(), (float)getWindowWidth() - m_font->getStringWidth(energy.c_str()) - xOffset, (float)getWindowHeight() - 75);

		std::stringstream s;
		s << "Score: " << m_player->GetCurrentScore();
		std::string score = s.str();

		m_2dRenderer->drawText(m_font, score.c_str(), (float)getWindowWidth() - m_font->getStringWidth(score.c_str()) - xOffset, (float)getWindowHeight() - 40);

		m_2dRenderer->drawText(m_font, "Press 'R' to restart!", 0, getWindowHeight() - 64);

		//m_player->DrawLine(m_2dRenderer);
	}
	else if (m_menu->m_currentGameState == GameState::MENU)
	{
		std::stringstream s;
		s << "Total Score: " << m_menu->m_currentScore;
		std::string score = s.str();

		m_2dRenderer->drawText(m_font, score.c_str(), (float)getWindowWidth() / 2 - 100, (float)getWindowHeight() - 50);
	}

	// done drawing sprites
	m_2dRenderer->end();
}

void Application2D::InitialiseGame()
{
	m_physicsScene = new PhysicsScene();
	m_physicsScene->SetTimeStep(0.01f);
	m_physicsScene->SetGravity(glm::vec2(0, -50));

	m_levelManager = new LevelManage(m_physicsScene, 1.0f, 50, 10);

	m_player = new Player(glm::vec2(0, 0), glm::vec2(-20, 0), 1, 2, glm::vec4(1, 1, 1, 1));
	m_player->SetEnergyLoss(50);
	m_player->SetMaxEnergy(100);
	m_player->SetMaxForce(50);
	m_player->SetLinearDrag(0.25f);

	m_menu->UpdatePlayer(m_player);
	m_physicsScene->AddActor(m_player);

	m_levelManager->AddBallData(new LevelManage::BallData(CollectibleType::NORMAL, 300, 1, 0, 10, 10, glm::vec4(1, 1, 0, 1)));
	m_levelManager->AddBallData(new LevelManage::BallData(CollectibleType::NORMAL, 75, 1.5f, 0, 35, 15, glm::vec4(0, 1, 0, 1)));

	m_levelManager->AddBallData(new LevelManage::BallData(CollectibleType::DANGEROUS, 50, 0.75f, -1, 10, 10, glm::vec4(1, 1, 1, 1)));
	
	m_levelManager->AddBallData(new LevelManage::BallData(CollectibleType::DANGEROUS, 450, 1.5f, 10, 2, -25, glm::vec4(1, 0, 0, 1)));
	m_levelManager->AddBallData(new LevelManage::BallData(CollectibleType::DANGEROUS, -50, 1.5f, 1, 20, -5, glm::vec4(1, 0, 1, 1)));

	m_levelManager->SortBallData();
	m_levelManager->SpawnRandom(m_levelManager->GetMaxSpawns());
}

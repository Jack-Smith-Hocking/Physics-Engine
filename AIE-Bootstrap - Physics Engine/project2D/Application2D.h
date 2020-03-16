#pragma once
#include "Application.h"
#include "Renderer2D.h"
#include <glm/ext.hpp>

class PhysicsScene;
class LevelManage;
class Player;
class MenuSystem;

class Application2D : public aie::Application {
public:

	Application2D();
	virtual ~Application2D();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

	// Initialise a game map
	void InitialiseGame();

protected:
	PhysicsScene* m_physicsScene = nullptr; // Physics scene that hold all relevant physics objects
	LevelManage* m_levelManager = nullptr; // Manages loading in levels and spawning balls
	Player* m_player = nullptr; // The player
	MenuSystem* m_menu = nullptr; // Handles GUI in the form of button menus

	aie::Renderer2D*	m_2dRenderer = nullptr;
	aie::Texture*		m_texture = nullptr;
	aie::Texture*		m_shipTexture = nullptr;
	aie::Font*			m_font = nullptr;

	float m_timer = 0;
};
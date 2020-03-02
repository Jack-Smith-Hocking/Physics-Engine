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

	void InitialiseGame();

protected:
	PhysicsScene* m_physicsScene = nullptr;
	LevelManage* m_levelManager = nullptr;
	Player* m_player = nullptr;
	MenuSystem* m_menu = nullptr;

	aie::Renderer2D*	m_2dRenderer = nullptr;
	aie::Texture*		m_texture = nullptr;
	aie::Texture*		m_shipTexture = nullptr;
	aie::Font*			m_font = nullptr;

	float m_timer = 0;
};
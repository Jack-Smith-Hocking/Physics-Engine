#pragma once
#include "Circle.h"
#include "Input.h"
#include "Renderer2D.h"

class Player : public Circle
{
public:
	Player(glm::vec2 position, glm::vec2 velocity, float mass, float radius, glm::vec4 colour);

	virtual void ResolveCollision(RigidBody* actor2, glm::vec2 contact, glm::vec2* collisionNormal = nullptr) override;

	int GetMaxHealth();
	void SetMaxHealth(int maxHealth);
	int GetCurrentHealth();

	float GetMaxEnergy();
	float GetCurrentEnergy();
	void SetMaxEnergy(float maxEnergy);
	void SetEnergyLoss(float lossPerSecond);

	float GetMaxForce();
	void SetMaxForce(float maxForce);

	int GetCurrentScore();

	virtual void MakeGizmo() override;
	virtual void FixedUpdate(glm::vec2 gravity, float dt) override;
	virtual void SafeToRemove() override;

	void TakeDamage(int damage);
	void IncreaseScore(int increase);
	void GainEnergy(float energyGained);

protected:
	int m_maxHealth = 10; // Player's max health
	int m_currentHealth = 0; // Player's current health

	float m_maxEnergy = 0; // Player's max energy
	float m_currentEnergy = 0; // Player's current energy
	float m_energyLossPerSecond = 0; // How much energy is lost per second

	bool m_startDelay = false;
	float m_currentDelayTimer = 0;

	float m_maxForce = 10; // Player's max force

	int m_currentScore = 0; // Player's current score

	aie::Input* m_playerInput = nullptr;

	glm::vec2* m_mouseDownBegin = nullptr; // Screen position when player first presses down
	glm::vec2* m_mouseDownEnd = nullptr; // Screen position when player realeases button
};


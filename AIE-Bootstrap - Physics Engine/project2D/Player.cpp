#include "Player.h"
#include "CollectibleCircle.h"
#include "KillerBox.h"
#include "Input.h"
#include "Renderer2D.h"

#include <iostream>
#include <sstream>

extern glm::vec2 g_windowDimensions;

Player::Player(glm::vec2 position, glm::vec2 velocity, float mass, float radius, glm::vec4 colour) : Circle(position, velocity, mass, radius, colour)
{
	SetMaxHealth(10);
	m_playerInput = aie::Input::getInstance();
}

void Player::ResolveCollision(RigidBody* actor2, glm::vec2 contact, glm::vec2* collisionNormal)
{
	CollectibleCircle* cc = dynamic_cast<CollectibleCircle*>(actor2);
	if (cc != nullptr)
	{
		cc->ResolveCollision(this, contact, collisionNormal);
		return;
	}
	else
	{
		KillerBox* kb = dynamic_cast<KillerBox*>(actor2);
		if (kb != nullptr)
		{
			kb->ResolveCollision(this, contact, collisionNormal);
		}
	}

	RigidBody::ResolveCollision(actor2, contact, collisionNormal);
}

void Player::SetMaxHealth(int maxHealth)
{
	m_maxHealth = maxHealth;
	m_currentHealth = maxHealth;
}

int Player::GetMaxHealth()
{
	return m_maxHealth;
}

int Player::GetCurrentHealth()
{
	return m_currentHealth;
}

float Player::GetMaxEnergy()
{
	return m_maxEnergy;
}

float Player::GetCurrentEnergy()
{
	return m_currentEnergy;
}

void Player::SetMaxEnergy(float maxEnergy)
{
	m_maxEnergy = maxEnergy;
	m_currentEnergy = maxEnergy;
}

void Player::SetEnergyLoss(float lossPerSecond)
{
	m_energyLossPerSecond = lossPerSecond;
}

int Player::GetCurrentScore()
{
	return m_currentScore;
}

float Player::GetMaxForce()
{
	return m_maxForce;
}

void Player::SetMaxForce(float maxForce)
{
	m_maxForce = maxForce;
}

void Player::MakeGizmo()
{
	aie::Gizmos::add2DCircle(m_position, m_radius, 12, m_colour, nullptr); // Draw player
	aie::Gizmos::add2DCircle(m_position + glm::normalize(m_velocity), m_radius / 2, 12, glm::vec4(0, 0, 0, 1), nullptr); // Draw player eye

	aie::Gizmos::add2DAABBFilled(glm::vec2(-20, 50), glm::vec2(15 * ((float)m_currentHealth / (float)m_maxHealth), 2), glm::vec4(1, 0, 0, 1), nullptr); // Display red bar for health
	aie::Gizmos::add2DAABBFilled(glm::vec2(20, 50), glm::vec2(15 * ((float)m_currentEnergy / (float)m_maxEnergy), 2), glm::vec4(0, 0, 1, 1), nullptr); // Display blue bar for energy

	if (m_mouseDownBegin != nullptr && m_mouseDownEnd != nullptr) // Draw line for user input
	{
		glm::vec2 dir = *m_mouseDownBegin - *m_mouseDownEnd;
		dir = glm::clamp(dir, -m_maxForce, m_maxForce) / 5.0f;

		aie::Gizmos::add2DLine(GetPosition(), GetPosition() + dir, glm::vec4(1, 1, 1, 1));
	}
}

void Player::FixedUpdate(glm::vec2 gravity, float dt)
{
	if (m_currentEnergy <= 0.5f) // Put a delay on if the player's energy is too low
	{
		m_startDelay = true;
	}

	if (!m_startDelay)
	{
		if (m_playerInput->isMouseButtonDown(0)) // Check if left mouse button is down
		{
			if (m_mouseDownBegin == nullptr) // Set the first mouse down position
			{
				m_mouseDownBegin = new glm::vec2(m_playerInput->getMouseX(), m_playerInput->getMouseY());
			}
			else // Set the mouse up position
			{
				m_mouseDownEnd = new glm::vec2(m_playerInput->getMouseX(), m_playerInput->getMouseY());
			}

			m_currentEnergy -= m_energyLossPerSecond * dt; // Decrease player energy
		}
		else if (m_playerInput->isMouseButtonUp(0)) // Set the mouse up position
		{
			m_mouseDownEnd = new glm::vec2(m_playerInput->getMouseX(), m_playerInput->getMouseY());
		}
	}
	else // Update timer
	{
		m_currentDelayTimer += dt;

		if (m_currentDelayTimer >= 1.0f)
		{
			m_startDelay = false;
			m_currentDelayTimer = 0;
		}
		else
		{
			m_mouseDownBegin = nullptr;
			m_mouseDownEnd = nullptr;
		}
	}

	if (!m_playerInput->isMouseButtonDown(0)|| m_startDelay)
	{
		if (m_mouseDownBegin != nullptr && m_mouseDownEnd != nullptr)
		{
			float x = m_mouseDownBegin->x - m_mouseDownEnd->x;
			float y = m_mouseDownBegin->y - m_mouseDownEnd->y;

			glm::vec2 force = glm::vec2(x, y); // Direction of force to be applied 
			force = glm::clamp(force, -m_maxForce, m_maxForce);

			if (glm::length(force) > 0.01f)
			{
				this->SetVelocity(force);
			}

			std::cout << "x: " << x << " Y: " << y << std::endl;
			std::cout << "ForceX: " << force.x << " ForceY: " << force.y << std::endl;

			m_mouseDownBegin = nullptr;
			m_mouseDownEnd = nullptr;
		}

		RigidBody::FixedUpdate(gravity, dt);
		m_currentEnergy += (m_energyLossPerSecond * dt) / 5; // Increase energy
	}

	if (m_currentHealth <= 0)
	{
		m_destroyObject = true;
	}

	m_currentEnergy = glm::clamp(m_currentEnergy, 0.0f, m_maxEnergy); // Clamp energy
}

void Player::SafeToRemove()
{
	//delete this;
}

void Player::TakeDamage(int damage)
{
	m_currentHealth -= damage;

	m_currentHealth = glm::clamp(m_currentHealth, 0, m_maxHealth);
}

void Player::IncreaseScore(int increase)
{
	m_currentScore += increase;

	m_currentScore = glm::clamp(m_currentScore, 0, 1000000);

	if (m_currentHealth <= 0)
	{
		m_currentEnergy = 0;
	}
}

void Player::GainEnergy(float energyGained)
{
	m_currentEnergy += energyGained;

	m_currentEnergy = glm::clamp(m_currentEnergy, 0.0f, m_maxEnergy);
}

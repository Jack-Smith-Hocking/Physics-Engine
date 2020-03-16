#pragma once
#include "Circle.h"

class Spring;

// Type of collectible item
enum CollectibleType
{
	NORMAL,
	DANGEROUS	
};

class CollectibleCircle : public Circle
{
public:
	CollectibleCircle(glm::vec2 position, glm::vec2 velocity, float mass, float radius, glm::vec4 colour, CollectibleCircle* springHolder = nullptr);

	// Set up values for the class
	void CollectibleValues(CollectibleType type, int score, int damage = 0, float energy = 0, Spring* spring = nullptr);

	virtual void MakeGizmo() override;
	virtual void FixedUpdate(glm::vec2 gravity, float dt) override;

	virtual void ResolveCollision(RigidBody* actor2, glm::vec2 contact, glm::vec2* collisionNormal = nullptr) override;
	virtual void SafeToRemove() override;

protected:
	CollectibleType m_type = CollectibleType::NORMAL; // The type of item this is

	int m_score = 0; // The score this item gives when collected
	int m_damage = 0; // The amount of damage this item does when collected
	float m_energy = 0; // The amount of energy this item gives when collected

	Spring* m_spring = nullptr; // The spring that holds this item to another item
	CollectibleCircle* m_springPoint = nullptr;
};


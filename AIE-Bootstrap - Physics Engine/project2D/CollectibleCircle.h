#pragma once
#include "Circle.h"

class Spring;

enum CollectibleType
{
	NORMAL,
	DANGEROUS	
};

class CollectibleCircle : public Circle
{
public:
	CollectibleCircle(glm::vec2 position, glm::vec2 velocity, float mass, float radius, glm::vec4 colour, CollectibleCircle* springHolder = nullptr);

	void CollectibleValues(CollectibleType type, int score, int damage = 0, float energy = 0, Spring* spring = nullptr);

	virtual void MakeGizmo() override;
	virtual void FixedUpdate(glm::vec2 gravity, float dt) override;

	virtual void ResolveCollision(RigidBody* actor2, glm::vec2 contact, glm::vec2* collisionNormal = nullptr) override;
	virtual void SafeToRemove() override;

protected:
	CollectibleType m_type;

	int m_score;
	int m_damage;
	float m_energy;

	Spring* m_spring = nullptr;
	CollectibleCircle* m_springPoint = nullptr;
};


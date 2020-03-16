#pragma once
#include "PhysicsObject.h"

class RigidBody;

class Plane : public PhysicsObject
{
public:
	Plane();
	Plane(glm::vec2 normal, float distance);
	~Plane();

	virtual void FixedUpdate(glm::vec2 gravity, float timeStep);

	virtual void Debug();
	virtual void MakeGizmo();
	virtual void ResetPosition();

	void ResolveCollision(RigidBody* actor2, glm::vec2 contact);

	glm::vec2 GetNormal();
	float GetDistance();

	float m_elasticity = 1;

protected:
	glm::vec2 m_normal = glm::vec2(0, 0);
	float m_distanceToOrigin = 0;
};


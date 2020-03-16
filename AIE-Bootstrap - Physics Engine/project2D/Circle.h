 #pragma once
#include "RigidBody.h"

class Circle : public RigidBody
{
public:
	Circle(glm::vec2 position, glm::vec2 velocity, float mass, float radius, glm::vec4 colour);
	~Circle();

	virtual void MakeGizmo();
	virtual bool CheckCollision(PhysicsObject* pOther);

	virtual void CollisionDetected(RigidBody* collisionBody);

	float GetRadius();
	void SetRadius(float radius);

protected:
	glm::vec4 m_colour = glm::vec4(0, 0, 0, 0);

	float m_radius = 0;
};
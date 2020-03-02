#pragma once
#include "Box.h"

class OBB : public Box
{
public:
	OBB(glm::vec2 position, glm::vec2 velocity, float mass, float rotation, glm::vec2 dimensions, glm::vec4 colour);

	virtual void FixedUpdate(glm::vec2 gravity, float timeStep);
	virtual void MakeGizmo();

	virtual bool CheckCollision(PhysicsObject* pOther);

	virtual void CollisionDetected(RigidBody* collisionBody);

	bool CheckCorners(const class::OBB& obb, glm::vec2& contact, int& numContacts, float& pen, glm::vec2& edgeNormal);

	glm::vec2 GetLocalX();
	glm::vec2 GetLocalY();
};


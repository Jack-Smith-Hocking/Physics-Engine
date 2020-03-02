#pragma once
#include "OBB.h"

class KillerBox : public OBB
{
public:
	KillerBox(glm::vec2 position, glm::vec2 velocity, float mass, float rotation, glm::vec2 dimensions, glm::vec4 colour);

	virtual void ResolveCollision(RigidBody* actor2, glm::vec2 contact, glm::vec2* collisionNormal = nullptr) override;
};


#pragma once
#include "Box.h"
#include <vector>

class AABB : public Box
{
public:
	AABB(glm::vec2 position, glm::vec2 velocity, float mass, float rotation, glm::vec2 dimensions, glm::vec4 colour);
	~AABB();

	virtual bool CheckCollision(PhysicsObject* pOther);
};


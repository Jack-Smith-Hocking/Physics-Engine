#include "AABB.h"

AABB::AABB(glm::vec2 position, glm::vec2 velocity, float mass, float rotation, glm::vec2 dimensions, glm::vec4 colour) :
	Box(ShapeType::AABB, position, velocity, mass, rotation, dimensions, colour)
{

}

AABB::~AABB()
{
}



bool AABB::CheckCollision(PhysicsObject* pOther)
{
	return false;
}

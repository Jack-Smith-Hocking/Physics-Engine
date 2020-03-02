#include "Box.h"

Box::Box(ShapeType shapeID, glm::vec2 position, glm::vec2 velocity, float mass, float rotation, glm::vec2 dimensions, glm::vec4 colour) :
	RigidBody(shapeID, position, velocity, mass),  m_extents(dimensions / 2.0f), m_colour(colour)
{
	m_rotation = rotation;
	m_moment = (1.0f / 12.0f) * mass * GetWidth() * GetHeight();
}

Box::~Box()
{
}

void Box::FixedUpdate(glm::vec2 gravity, float timeStep)
{
	RigidBody::FixedUpdate(gravity, timeStep);
}

void Box::MakeGizmo()
{
}

float Box::GetWidth()
{
	return m_extents.x * 2.0f;
}

float Box::GetHeight()
{
	return m_extents.y * 2.0f;
}

glm::vec2 Box::GetExtents()
{
	return m_extents;
}

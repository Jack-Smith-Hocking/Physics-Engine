#pragma once
#include "RigidBody.h"

class Box : public RigidBody
{
public:
	Box(ShapeType shapeID, glm::vec2 position, glm::vec2 velocity, float mass, float rotation, glm::vec2 dimensions, glm::vec4 colour);
	~Box();

	virtual void FixedUpdate(glm::vec2 gravity, float timeStep);
	virtual void MakeGizmo();

	float GetWidth();
	float GetHeight();

	glm::vec2 GetExtents();

protected:
	glm::vec2 m_extents;

	glm::vec4 m_colour;
};


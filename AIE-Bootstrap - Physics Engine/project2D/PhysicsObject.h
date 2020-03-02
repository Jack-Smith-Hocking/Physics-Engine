#pragma once
#include "glm/ext.hpp"
#include <vector>

enum ShapeType
{
	JOINT = -1,
	PLANE = 0,
	CIRCLE,
	OBB,
	AABB,
	SHAPE_COUNT
};

class PhysicsObject
{
public:
	virtual void FixedUpdate(glm::vec2 gravity, float timeStep) = 0;
	virtual void Debug() = 0;
	virtual void MakeGizmo() = 0;

	virtual void ResetPosition();

	virtual void SafeToRemove();

	ShapeType GetShapeType();

	bool m_destroyObject = false;

protected:
	PhysicsObject(ShapeType shapeID);

	ShapeType m_shapeID;
};


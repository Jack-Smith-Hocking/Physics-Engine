#include "PhysicsObject.h"

PhysicsObject::PhysicsObject(ShapeType shapeID) : 
	m_shapeID(shapeID)
{
}

void PhysicsObject::ResetPosition() 
{
}

void PhysicsObject::SafeToRemove()
{
}


ShapeType PhysicsObject::GetShapeType()
{
	return m_shapeID;
}


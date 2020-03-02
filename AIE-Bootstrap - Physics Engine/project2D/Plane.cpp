#include "Plane.h"
#include "Gizmos.h"
#include "RigidBody.h"

Plane::Plane() :
	PhysicsObject(PLANE)
{
	m_distanceToOrigin = 0;
	m_normal = glm::vec2(0, 1);
	m_elasticity = 1;
}

Plane::Plane(glm::vec2 normal, float distance) :
	PhysicsObject(PLANE), m_normal(normal), m_distanceToOrigin(distance)
{
	m_normal = glm::normalize(m_normal);
	m_elasticity = 1;
}

Plane::~Plane()
{
}

void Plane::FixedUpdate(glm::vec2 gravity, float timeStep)
{
}

void Plane::Debug()
{
}

void Plane::MakeGizmo()
{
	float lineSegmentLength = 300;
	glm::vec2 centrePoint = m_normal * m_distanceToOrigin;
	glm::vec2 parallel = glm::vec2(m_normal.y, -m_normal.x);

	glm::vec2 startPoint = centrePoint + (parallel * lineSegmentLength);
	glm::vec2 endPoint = centrePoint - (parallel * lineSegmentLength);

	glm::vec4 colour = glm::vec4(1, 0, 0, 1);

	//aie::Gizmos::add2DLine(startPoint, endPoint, colour);
}

void Plane::ResetPosition()
{
}

void Plane::ResolveCollision(RigidBody* actor2, glm::vec2 contact)
{
	if (actor2 == nullptr)
	{
		return;
	}

	glm::vec2 normal = m_normal;
	glm::vec2 relativeVelocity = actor2->GetVelocity();

	float elasticity = (actor2->m_elasticity + m_elasticity) / 2.0f;
	float j = glm::dot(-(1 + elasticity) * (relativeVelocity), normal) / (1 / actor2->GetMass());

	glm::vec2 force = normal * j;

	actor2->ApplyForce(force, contact - actor2->GetPosition());
}

glm::vec2 Plane::GetNormal()
{
	return m_normal;
}

float Plane::GetDistance()
{
	return m_distanceToOrigin;
}

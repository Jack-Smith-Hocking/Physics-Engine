#include "Circle.h"
#include "Application2D.h"

Circle::Circle(glm::vec2 position, glm::vec2 velocity, float mass, float radius, glm::vec4 colour) :
	RigidBody(CIRCLE, position, velocity, mass), m_radius(radius), m_colour(colour)
{
	m_moment = 0.5f * m_mass * m_radius * m_radius;
}

Circle::~Circle()
{
}

void Circle::MakeGizmo()
{
	//glm::vec2 end = glm::vec2(std::cos(m_rotation), std::sin(m_rotation)) * m_radius;
	
	aie::Gizmos::add2DCircle(m_position, m_radius, 12, m_colour, nullptr);
	//aie::Gizmos::add2DLine(m_position, m_position + end, glm::vec4(0, 0, 0, 1));
}

bool Circle::CheckCollision(PhysicsObject* pOther)
{
	Circle* circleCollided = dynamic_cast<Circle*>(pOther);

	if (circleCollided == nullptr) { return false; }

	float dist = glm::distance(circleCollided->m_position, m_position);

	return (dist <= (m_radius + circleCollided->m_radius));

	return circleCollided != nullptr;
}

void Circle::CollisionDetected(RigidBody* collisionBody)
{
}

float Circle::GetRadius()
{
	return m_radius;
}

void Circle::SetRadius(float radius)
{
	m_radius = radius;
}

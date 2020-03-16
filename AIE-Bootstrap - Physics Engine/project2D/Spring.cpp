#include "Spring.h"
#include "RigidBody.h"

Spring::Spring(RigidBody* body1, RigidBody* body2, float restLength, float springCoefficient, float maxForce, float dampening, glm::vec2 contact1, glm::vec2 contact2) : PhysicsObject(JOINT),
m_body1(body1), m_body2(body2), m_restLength(restLength), m_springCoefficient(springCoefficient), m_maxForce(maxForce), m_dampening(dampening), m_contact1(contact1), m_contact2(contact2)
{
	CheckIfDestroy();
}

void Spring::FixedUpdate(glm::vec2 gravity, float timeStep)
{
	{
		glm::vec2 p1 = m_body1->ToWorld(m_contact1);
		glm::vec2 p2 = m_body2->ToWorld(m_contact2);

		glm::vec2 relativeVelocity = m_body2->GetVelocity() - m_body1->GetVelocity();
		glm::vec2 dist = p2 - p1;

		float length = glm::length(dist);

		glm::vec2 force = dist * m_springCoefficient * (m_restLength - length) - m_dampening * relativeVelocity;

		//force = glm::clamp(force, 0.0f, m_maxForce);

		m_body1->ApplyForce(-force * timeStep, p1 - m_body1->GetPosition()); // Apply force to keep bodies together 
		m_body2->ApplyForce(force * timeStep, p2 - m_body2->GetPosition()); // Apply force to kepp bodies together 
	}
}

void Spring::MakeGizmo()
{
	if (m_body1 != nullptr && m_body2 != nullptr)
	{
		aie::Gizmos::add2DLine(m_body1->GetPosition(), m_body2->GetPosition(), glm::vec4(1, 1, 1, 1));
	}
}

void Spring::Debug()
{
}

bool Spring::BodyIsNull()
{
	return (m_body1 == nullptr || m_body2 == nullptr);
}

void Spring::CheckIfDestroy()
{
	if (m_body1 == nullptr || m_body2 == nullptr)
	{
		m_destroyObject = true;
	}
}

void Spring::SafeToRemove()
{
	delete this;
}

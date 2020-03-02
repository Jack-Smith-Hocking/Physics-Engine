#include "RigidBody.h"
#include <iostream>

RigidBody::RigidBody(ShapeType shapeID, glm::vec2 position, glm::vec2 velocity, float mass) :
	PhysicsObject(shapeID), m_position(position), m_velocity(velocity), m_mass(mass), m_rotation(0)
{
	m_elasticity = 1;
	m_angularVelocity = 0;
	m_angularDrag = 0;
	m_linearDrag = 0;
}

RigidBody::~RigidBody()
{
}

void RigidBody::FixedUpdate(glm::vec2 gravity, float timeStep)
{
	if (m_isKinematic) 
	{
		m_velocity = glm::vec2(0, 0);
		m_angularVelocity = 0;
		return; 
	}

	m_velocity += gravity * timeStep;
	m_position += m_velocity * timeStep;

	m_velocity -= m_velocity * m_linearDrag * timeStep;
	m_rotation += m_angularVelocity * timeStep;
	m_angularVelocity -= m_angularVelocity * m_angularDrag * timeStep;

	m_angularVelocity = glm::clamp(m_angularVelocity, -m_maxAngularVelocity, m_maxAngularVelocity);

	{
		if (glm::length(m_velocity) < MIN_LINEAR_THRESHOLD)
		{
			m_velocity = glm::vec2(0, 0);
		}
		if (glm::abs(m_angularVelocity) < MIN_ROTATION_THRESHOLD)
		{
			m_angularVelocity = 0;
		}
	}

	Debug();
}

void RigidBody::Debug()
{
}

void RigidBody::ApplyForce(glm::vec2 force, glm::vec2 pos)
{
	m_velocity += force / m_mass;
	m_angularVelocity += (force.y * pos.x - force.x * pos.y) / (GetMoment());
}

void RigidBody::ResolveCollision(RigidBody* actor2, glm::vec2 contact, glm::vec2* collisionNormal)
{
	if (actor2 == nullptr)
	{
		return;
	}

	glm::vec2 normal = glm::normalize(collisionNormal ? *collisionNormal : actor2->m_position - m_position);
	glm::vec2 perp = glm::vec2(normal.y, -normal.x);

	float r1 = glm::dot(contact - m_position, -perp);
	float r2 = glm::dot(contact - actor2->m_position, perp);

	float v1 = glm::dot(m_velocity, normal) - r1 * m_angularVelocity;
	float v2 = glm::dot(actor2->m_velocity, normal) + r2 * actor2->m_angularVelocity;

	if (v1 > v2)
	{
		float mass1 = 1.0f / (1.0f / GetMass() + (r1 * r1) / GetMoment());
		float mass2 = 1.0f / (1.0f / actor2->GetMass() + (r2 * r2) / actor2->GetMoment());

		float elasticity = (m_elasticity + actor2->m_elasticity) / 2.0f;

		glm::vec2 force = (1.0f + elasticity) * mass1 * mass2 / (mass1 + mass2) * (v1 - v2) * normal;

		this->ApplyForce(-force, contact - m_position);
		actor2->ApplyForce(force, contact - actor2->m_position);
	}
}

float RigidBody::GetRotationalKineticEnergy()
{
	return 0.5f * (m_mass * glm::dot(m_velocity, m_velocity) + GetMoment() * (m_angularVelocity * m_angularVelocity));
}

float RigidBody::GetKineticEnergy()
{
	return 0.5f * (m_mass * glm::dot(m_velocity, m_velocity));
}

glm::vec2 RigidBody::GetPosition()
{
	return m_position;
}

glm::vec2 RigidBody::GetVelocity()
{
	return m_velocity;
}

void RigidBody::SetVelocity(glm::vec2 velocity)
{
	m_velocity = velocity;
}

void RigidBody::SetLinearDrag(float linearDrag)
{
	m_linearDrag = linearDrag;
}

void RigidBody::SetAngularDrag(float angularDrag)
{
	m_angularDrag = angularDrag;
}

void RigidBody::SetMoment(float moment)
{
	m_moment = moment;
}

void RigidBody::SetPosition(glm::vec2 position)
{
	m_position = position;
}

void RigidBody::SetKinematic(bool isKinematic)
{
	m_isKinematic = isKinematic;
}

void RigidBody::CollisionDetected(RigidBody* collisionBody)
{

}

float RigidBody::GetRotation()
{
	return m_rotation;
}

float RigidBody::GetMass()
{
	return (m_isKinematic ? INT_MAX : m_mass);
}

float RigidBody::GetLinearDrag()
{
	return m_linearDrag;
}

float RigidBody::GetAngularDrag()
{
	return m_angularDrag;
}

float RigidBody::GetAngularVelocity()
{
	return m_angularVelocity;
}

float RigidBody::GetMoment()
{
	return (m_isKinematic ? INT_MAX : m_moment);
}

glm::vec2 RigidBody::ToWorld(glm::vec2 position)
{
	float cs = cosf(m_rotation);
	float sn = sinf(m_rotation);

	m_localX = glm::normalize(glm::vec2(cs, sn));
	m_localY = glm::normalize(glm::vec2(-sn, cs));

	return m_position + (m_localX * position.x + m_localY * position.y);
}

bool RigidBody::IsKinematic()
{
	return m_isKinematic;
}

float RigidBody::CalcRotKineticEnergy(RigidBody* rb1, RigidBody* rb2)
{
	float RKe = 0;

	if (rb1 != nullptr)
	{
		RKe += rb1->GetRotationalKineticEnergy();
	}
	if (rb2 != nullptr)
	{
		RKe += rb2->GetRotationalKineticEnergy();
	}

	return RKe;
}

float RigidBody::CalcKineticEnergy(RigidBody* rb1, RigidBody* rb2)
{
	float Ke = 0;

	if (rb1 != nullptr)
	{
		Ke += rb1->GetKineticEnergy();
	}
	if (rb2 != nullptr)
	{
		Ke += rb2->GetKineticEnergy();
	}

	return Ke;
}

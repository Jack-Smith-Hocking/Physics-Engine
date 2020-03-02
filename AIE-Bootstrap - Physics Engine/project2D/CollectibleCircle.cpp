#include "CollectibleCircle.h"
#include "Player.h"
#include "Spring.h"

CollectibleCircle::CollectibleCircle(glm::vec2 position, glm::vec2 velocity, float mass, float radius, glm::vec4 colour, CollectibleCircle* springHolder) : Circle(position, velocity, mass, radius, colour)
{
	CollectibleValues(CollectibleType::NORMAL, 100, 0);
	//SetKinematic(true);
	m_destroyObject = false;
}

void CollectibleCircle::CollectibleValues(CollectibleType type, int score, int damage, float energy, Spring* spring)
{
	m_type = type;
	m_score = score;
	m_damage = damage;
	m_energy = energy;

	m_spring = spring;
}

void CollectibleCircle::MakeGizmo()
{
	Circle::MakeGizmo();

	float x1 = 0;
	float x2 = 1.5f;
	float x3 = (x1 + x2) / 2;

	float y1 = 0;
	float y2 = m_radius;
	float y3 = (y1 + y2) / 2;

	switch (m_type)
	{
	case NORMAL:
		Circle::MakeGizmo();
		break;

	case DANGEROUS:
		if (m_damage > 0)
		{
			// Vertical //
			glm::vec2 p1 = m_position + glm::vec2(x1, 0) * m_radius - glm::vec2(m_radius / 1.42f, 0);
			glm::vec2 p2 = m_position + glm::vec2(x2, 0) * m_radius - glm::vec2(m_radius / 1.42f, 0);
			glm::vec2 p3 = m_position + glm::vec2(x3, 1.75f) * m_radius - glm::vec2(m_radius / 1.42f, 0);

			aie::Gizmos::add2DTri(p1, p2, p3, m_colour);

			p1 = m_position + glm::vec2(x1, 0) * m_radius - glm::vec2(m_radius / 1.4f, 0);
			p3 = m_position + glm::vec2(x2, 0) * m_radius - glm::vec2(m_radius / 1.4f, 0);
			p2 = m_position + glm::vec2(x3, -1.75f) * m_radius - glm::vec2(m_radius / 1.4f, 0);

			aie::Gizmos::add2DTri(p1, p2, p3, m_colour);
			// //

			// Horizontal //
			p1 = m_position + glm::vec2(0, 0) * m_radius - glm::vec2(-0.25f, m_radius / 2);
			p2 = m_position + glm::vec2(1.75f, 0.5f) * m_radius - glm::vec2(-0.25f, m_radius / 2);
			p3 = m_position + glm::vec2(0, 1) * m_radius - glm::vec2(-0.25f, m_radius / 2);

			aie::Gizmos::add2DTri(p1, p2, p3, m_colour);

			p1 = m_position + glm::vec2(0, 0) * m_radius - glm::vec2(0.25f, m_radius / 2);
			p3 = m_position + glm::vec2(-1.75f, 0.5f) * m_radius - glm::vec2(0.25f, m_radius / 2);
			p2 = m_position + glm::vec2(0, 1) * m_radius - glm::vec2(0.25f, m_radius / 2);

			aie::Gizmos::add2DTri(p1, p2, p3, m_colour);
			// //
		}
		break;

	default:
		break;
	}
}

void CollectibleCircle::FixedUpdate(glm::vec2 gravity, float dt)
{
	Circle::FixedUpdate(gravity, dt);

	if (m_spring != nullptr && m_spring->m_destroyObject)
	{
		m_spring = nullptr;
	}
}

void CollectibleCircle::ResolveCollision(RigidBody* actor2, glm::vec2 contact, glm::vec2* collisionNormal)
{
	CollectibleCircle* cc = dynamic_cast<CollectibleCircle*>(actor2);
	if (cc != nullptr)
	{
		return;
	}

	RigidBody::ResolveCollision(actor2, contact, collisionNormal);

	Player* p = dynamic_cast<Player*>(actor2);

	if (p != nullptr)
	{
		switch (m_type)
		{
		case NORMAL:
			p->IncreaseScore(m_score);
			m_destroyObject = true;

			if (m_spring != nullptr)
			{
				m_spring->m_destroyObject = true;
			}
			break;

		case DANGEROUS:
			p->TakeDamage(m_damage);

			if (p->GetCurrentHealth() > 0)
			{
				p->IncreaseScore(m_score);
				m_destroyObject = true;

				if (m_spring != nullptr)
				{
					m_spring->m_destroyObject = true;
				}
			}
			break;

		default:
			break;
		}
		p->GainEnergy(m_energy);
	}
}

void CollectibleCircle::SafeToRemove()
{
	if (m_destroyObject)
	{
		delete this;
	}
}

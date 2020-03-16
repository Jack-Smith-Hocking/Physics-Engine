#include "OBB.h"

OBB::OBB(glm::vec2 position, glm::vec2 velocity, float mass, float rotation, glm::vec2 dimensions, glm::vec4 colour) :
	Box(ShapeType::OBB, position, velocity, mass, rotation, dimensions, colour)
{
	float cs = cosf(m_rotation);
	float sn = sinf(m_rotation);

	m_localX = glm::normalize(glm::vec2(cs, sn));
	m_localY = glm::normalize(glm::vec2(-sn, cs));
}

void OBB::FixedUpdate(glm::vec2 gravity, float timeStep)
{
	Box::FixedUpdate(gravity, timeStep);

	if (m_isKinematic) { return; }

	float cs = cosf(m_rotation);
	float sn = sinf(m_rotation);

	m_localX = glm::normalize(glm::vec2(cs, sn));
	m_localY = glm::normalize(glm::vec2(-sn, cs));
}

void OBB::MakeGizmo()
{
	glm::vec2 p1 = m_position - m_localX * m_extents.x - m_localY * m_extents.y;
	glm::vec2 p2 = m_position + m_localX * m_extents.x - m_localY * m_extents.y;
	glm::vec2 p3 = m_position - m_localX * m_extents.x + m_localY * m_extents.y;
	glm::vec2 p4 = m_position + m_localX * m_extents.x + m_localY * m_extents.y;
	aie::Gizmos::add2DTri(p1, p2, p4, m_colour);
	aie::Gizmos::add2DTri(p1, p4, p3, m_colour);
}

bool OBB::CheckCollision(PhysicsObject* pOther)
{
	return false;
}

void OBB::CollisionDetected(RigidBody* collisionBody)
{
	if (collisionBody == nullptr)
	{
		return;
	}
}

bool OBB::CheckCorners(const class::OBB& obb, glm::vec2& contact, int& numContacts, float& pen, glm::vec2& edgeNormal)
{
	float minX = 0, maxX = 0, minY = 0, maxY = 0;
	float boxW = obb.m_extents.x * 2;
	float boxH = obb.m_extents.y * 2;

	int numLocalContacts = 0;
	bool first = true;
	
	glm::vec2 localContact(0, 0);
	
	for (float x = -obb.m_extents.x; x < boxW; x += boxW)
	{
		for (float y = -obb.m_extents.y; y < boxH; y += boxH)
		{
			// position in worldspace
			glm::vec2 p = obb.m_position + x * obb.m_localX + y * obb.m_localY;
			// position in our obb's space
			glm::vec2 p0(glm::dot(p - m_position, m_localX), glm::dot(p - m_position, m_localY));

			if (first || p0.x < minX) minX = p0.x;
			if (first || p0.x > maxX) maxX = p0.x;
			if (first || p0.y < minY) minY = p0.y;
			if (first || p0.y > maxY) maxY = p0.y;

			if (p0.x >= -m_extents.x && p0.x <= m_extents.x && p0.y >= -m_extents.y && p0.y <= m_extents.y)
			{
				numLocalContacts++;
				localContact += p0;
			}

			first = false;
		}
	}
	if (maxX <= -m_extents.x || minX >= m_extents.x || maxY <= -m_extents.y || minY >= m_extents.y) { return false; }
		
	if (numLocalContacts == 0) { return false; }

	bool res = false;
	
	contact += m_position + (localContact.x * m_localX + localContact.y * m_localY) / (float)numLocalContacts;
	numContacts++;

	float pen0 = m_extents.x - minX;
	if (pen0 > 0 && (pen0 < pen || pen == 0)) 
	{
		edgeNormal = m_localX;
		pen = pen0;
		res = true;
	}

	pen0 = maxX + m_extents.x;
	if (pen0 > 0 && (pen0 < pen || pen == 0)) 
	{
		edgeNormal = -m_localX;
		pen = pen0;
		res = true;
	}

	pen0 = m_extents.y - minY;
	if (pen0 > 0 && (pen0 < pen || pen == 0)) 
	{
		edgeNormal = m_localY;
		pen = pen0;
		res = true;
	}

	pen0 = maxY + m_extents.y;
	if (pen0 > 0 && (pen0 < pen || pen == 0)) 
	{
		edgeNormal = -m_localY;
		pen = pen0;
		res = true;
	}

	return res;
}

glm::vec2 OBB::GetLocalX()
{
	return m_localX;
}

glm::vec2 OBB::GetLocalY()
{
	return m_localY;
}

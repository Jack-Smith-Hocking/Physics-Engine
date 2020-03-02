#include "KillerBox.h"
#include "Player.h"

KillerBox::KillerBox(glm::vec2 position, glm::vec2 velocity, float mass, float rotation, glm::vec2 dimensions, glm::vec4 colour) : OBB(position, velocity, mass, rotation, dimensions, colour)
{
}

void KillerBox::ResolveCollision(RigidBody* actor2, glm::vec2 contact, glm::vec2* collisionNormal)
{
	Player* p = dynamic_cast<Player*>(actor2);

	if (p != nullptr)
	{
		p->TakeDamage(p->GetMaxHealth() * 2);
	}

	RigidBody::ResolveCollision(actor2, contact, collisionNormal);
}

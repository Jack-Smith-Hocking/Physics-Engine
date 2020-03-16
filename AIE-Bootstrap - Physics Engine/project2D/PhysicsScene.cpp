#include <list>
#include <iostream>

#include "PhysicsScene.h"
#include "PhysicsObject.h"
#include "RigidBody.h"
#include "Plane.h"
#include "Circle.h"
#include "CollectibleCircle.h"

#include "OBB.h"
#include "AABB.h"

PhysicsScene::PhysicsScene() : m_timeStep(0.01f), m_gravity(glm::vec2(0,0))
{
}

PhysicsScene::PhysicsScene(glm::vec2 gravity, float timeStep) :
	m_gravity(gravity), m_timeStep(timeStep)
{
}

PhysicsScene::~PhysicsScene()
{
	for (PhysicsObject* pActor : m_actors)
	{
		delete pActor;
	}
}

void PhysicsScene::AddActor(PhysicsObject* actor)
{
	if (actor != nullptr)
	{
		m_actors.push_back(actor);
	}
}

void PhysicsScene::RemoveActor(PhysicsObject* actor)
{
	if (actor != nullptr)
	{
		auto it = std::find(m_actors.begin(), m_actors.end(), actor);
		m_actors.erase(it);
	}
}

void PhysicsScene::Update(float dt)
{
	static float accumulatedTime = 0.0f;
	accumulatedTime += dt;

	RemoveNullElements(m_actors);

	while (accumulatedTime >= m_timeStep) // Only update every time step
	{
		for (PhysicsObject* pActor : m_actors)
		{
			if (pActor != nullptr) // Update all PhysicsObjects
			{
				pActor->FixedUpdate(m_gravity, m_timeStep);
			}
			else
			{
				m_actors.erase(std::find(m_actors.begin(), m_actors.end(), pActor));
			}
		}

		accumulatedTime -= m_timeStep;

		this->CheckCollision();
	}
}

void PhysicsScene::UpdateGizmo()
{
	for (PhysicsObject* pActor : m_actors) // Update all PhysicsObject's draw calls
	{
		if (pActor != nullptr)
		{
			pActor->MakeGizmo();
		}
	}
}

void PhysicsScene::DebugScene()
{
	for (PhysicsObject* pActor : m_actors)
	{
		pActor->Debug();
	}
}

void PhysicsScene::RemoveNullElements(std::vector<PhysicsObject*>& objs)
{
	PhysicsObject* obj = nullptr;
	bool keepRemoving = false;

	for (int i = 0; i < objs.size(); i++) // Iterate through the list and remove null elements 
	{
		obj = objs[i];

		if (obj->m_destroyObject)
		{
			auto it = std::find(objs.begin(), objs.end(), obj);
			objs.erase(it);

			keepRemoving = true;

			obj->SafeToRemove(); // Tell the object being removed to 'clean up' essentially

			break;
		}
	}

	if (keepRemoving)
	{
		RemoveNullElements(objs);
	}
}

void PhysicsScene::SetGravity(const glm::vec2 gravity)
{
	m_gravity = gravity;
}

glm::vec2 PhysicsScene::GetGravity() const
{
	return m_gravity;
}

void PhysicsScene::SetTimeStep(const float timeStep)
{
	m_timeStep = timeStep;
}

float PhysicsScene::GetTimeStep() const
{
	return m_timeStep;
}

int PhysicsScene::GetActorsCount()
{
	return (int)m_actors.size();
}

void PhysicsScene::ApplyContactForces(RigidBody* rb1, RigidBody* rb2, glm::vec2 norm, float pen)
{
	float body1Factor = rb1->IsKinematic() ? 0 : (rb2->IsKinematic() ? 1.0f : 0.5f);

	rb1->SetPosition(rb1->GetPosition() - body1Factor * norm * pen);
	rb2->SetPosition(rb2->GetPosition() + (1 - body1Factor) * norm * pen);
}


#pragma region Collisions
typedef bool(*fn)(PhysicsObject*, PhysicsObject*);

// Function pointers for collisions
static fn collisionFunctionArray[16] =
{
	PhysicsScene::plane2Plane, PhysicsScene::plane2Circle, PhysicsScene::plane2OBB, PhysicsScene::plane2AABB,
	PhysicsScene::circle2Plane, PhysicsScene::circle2Circle, PhysicsScene::circle2OBB, PhysicsScene::circle2AABB,
	PhysicsScene::OBB2Plane, PhysicsScene::OBB2Circle, PhysicsScene::OBB2OBB, PhysicsScene::OBB2AABB,
	PhysicsScene::AABB2Plane, PhysicsScene::AABB2Circle, PhysicsScene::AABB2OBB, PhysicsScene::AABB2AABB,
};

void PhysicsScene::CheckCollision()
{
	int actorCount = (int)m_actors.size();

	PhysicsObject* object1 = nullptr;
	PhysicsObject* object2 = nullptr;

	int shapeID1;
	int shapeID2;

	for (int outer = 0; outer < actorCount - 1; outer++)
	{
		for (int inner = outer + 1; inner < m_actors.size(); inner++)
		{
			object1 = m_actors[outer];
			object2 = m_actors[inner];
			shapeID1 = object1->GetShapeType();
			shapeID2 = object2->GetShapeType();

			if (shapeID1 < 0 || shapeID2 < 0)
			{
				continue;
			}

			int functionIndex = (shapeID1 * SHAPE_COUNT) + shapeID2; // Get the index for the function pointer array based off of the colliding objects' type

			fn collisionFunc = collisionFunctionArray[functionIndex];
			
			if (collisionFunc != nullptr)
			{
				collisionFunc(object1, object2);
			}
		}
	}
}

#pragma region PlaneCollisions
bool PhysicsScene::plane2Plane(PhysicsObject* p1, PhysicsObject* p2)
{
	return false;
}

bool PhysicsScene::plane2Circle(PhysicsObject* p, PhysicsObject* c)
{
	return circle2Plane(c, p);
}

bool PhysicsScene::plane2OBB(PhysicsObject* p, PhysicsObject* obb)
{
	return OBB2Plane(obb, p);
}

bool PhysicsScene::plane2AABB(PhysicsObject* p, PhysicsObject* aabb)
{
	return AABB2Plane(aabb, p);
}
#pragma endregion

#pragma region CircleCollisions
bool PhysicsScene::circle2Circle(PhysicsObject* c1, PhysicsObject* c2)
{
	CollectibleCircle* cc1 = dynamic_cast<CollectibleCircle*>(c1);
	CollectibleCircle* cc2 = dynamic_cast<CollectibleCircle*>(c2);

	if (cc1 != nullptr && cc2 != nullptr)
	{
		return false;
	}

	Circle* circle1 = dynamic_cast<Circle*>(c1);
	Circle* circle2 = dynamic_cast<Circle*>(c2);

	if (circle1 != nullptr && circle2 != nullptr)
	{
		glm::vec2 delta = circle2->GetPosition() - circle1->GetPosition(); // Get relative position

		float dist = glm::distance(circle1->GetPosition(), circle2->GetPosition()); // Get distance between their centres
		bool collided = dist <= (circle1->GetRadius() + circle2->GetRadius()); // Check if the sum of their radii are less than the distance between them

		if (collided)
		{
			float body1Factor = circle1->IsKinematic() ? 0 : (circle2->IsKinematic() ? 1.0f : 0.5f);

			glm::vec2 contactForce = (dist - (circle1->GetRadius() + circle2->GetRadius())) * delta / dist;

			circle1->SetPosition(circle1->GetPosition() + contactForce * body1Factor); // Apply contact forces
			circle2->SetPosition(circle2->GetPosition() + contactForce * (1 - body1Factor)); // Apply contact forces

			circle1->ResolveCollision(circle2, 0.5f * (circle1->GetPosition() + circle2->GetPosition()));

			circle1->CollisionDetected(circle2);
			circle2->CollisionDetected(circle1);

			return collided;
		}
	}

	return false;
}

bool PhysicsScene::circle2Plane(PhysicsObject* c, PhysicsObject* p)
{
	Circle* circle = dynamic_cast<Circle*>(c);
	Plane* plane = dynamic_cast<Plane*>(p);

	if (plane != nullptr && circle != nullptr)
	{
		glm::vec2 collisionNormal = plane->GetNormal();
		float circleToPlane = (glm::dot(circle->GetPosition(), plane->GetNormal()) - plane->GetDistance());

		if (circleToPlane < 0)
		{
			collisionNormal *= -1;
			circleToPlane *= -1;
		}

		float intersection = circle->GetRadius() - circleToPlane;
		if (intersection > 0)
		{
			circle->SetPosition(circle->GetPosition() + (collisionNormal * (circle->GetRadius() - circleToPlane)) * (circle->IsKinematic() ? 0.0f : 1.0f)); // Apply contact force

			glm::vec2 contact =  circle->GetPosition() + (plane->GetNormal() * -circle->GetRadius());
			plane->ResolveCollision(circle, contact);

			return true;
		}
	}

	return false;
}
bool PhysicsScene::circle2OBB(PhysicsObject* c, PhysicsObject* obb)
{
	return OBB2Circle(obb, c);
}
bool PhysicsScene::circle2AABB(PhysicsObject* c, PhysicsObject* aabb)
{
	return AABB2Circle(aabb, c);
}
#pragma endregion

#pragma region OBBCollisions
bool PhysicsScene::OBB2Plane(PhysicsObject* obb, PhysicsObject* p)
{
	class::OBB* box = dynamic_cast<class::OBB*>(obb);
	Plane* plane = dynamic_cast<Plane*>(p);

	if (box != nullptr && plane != nullptr)
	{
		glm::vec2 planeOrigin = plane->GetNormal() * plane->GetDistance();
		glm::vec2 contact(0, 0);
		
		int numContacts = 0;
		
		float contactV = 0;
		float radius = 0.5f * (std::fminf(box->GetWidth(), box->GetHeight()));
		float comFromPlane = glm::dot(box->GetPosition() - planeOrigin, plane->GetNormal());
		float penetration = 0;

		for (float x = -box->GetExtents().x; x < box->GetWidth(); x += box->GetWidth())
		{
			for (float y = -box->GetExtents().y; y < box->GetHeight(); y += box->GetHeight())
			{
				// Get the position of the corner in world space
				glm::vec2 p = box->GetPosition() + (x * box->GetLocalX()) + (y * box->GetLocalY ());

				float distFromPlane = glm::dot(p - planeOrigin, plane->GetNormal());
				// This is the total velocity of the point
				float velocityIntoPlane = glm::dot(box->GetVelocity() + box->GetAngularVelocity() * ((-y * box->GetLocalX()) + (x * box->GetLocalY())), plane->GetNormal());

				// If this corner is on the opposite side from the COM,
				// And moving further in, we need to resolve the collision
				if ((distFromPlane > 0 && comFromPlane < 0 && velocityIntoPlane > 0) || (distFromPlane < 0 && comFromPlane > 0 && velocityIntoPlane < 0))
				{
					numContacts++;
					contact += p;
					contactV += velocityIntoPlane;

					if (comFromPlane >= 0)
					{
						if (penetration > distFromPlane)
						{
							penetration = distFromPlane;
						}
						else if (penetration < distFromPlane)
						{
							penetration = distFromPlane;
						}
					}
				}
			}
			
		}

		// We've had a hit - typically only two corners can contact
		if (numContacts > 0)
		{
			// Get the average collision velocity into the plane
			// (covers linear and rotational velocity of all corners involved)
			float collisionV = contactV / (float)numContacts;

			// Get the acceleration required to stop (restitution = 0) or reverse
			// (restitution = 1) the average velocity into the plane
			glm::vec2 acceleration = -plane->GetNormal() * ((1.0f + box->m_elasticity) * collisionV);
			// And the average position at which we'll apply the force
			// (corner or edge centre)
			glm::vec2 localContact = (contact / (float)numContacts) - box->GetPosition();

			// This is the perpendicular distance we apply the force at relative to
			// The COM, so Torque = F*r
			float r = glm::dot(localContact, glm::vec2(plane->GetNormal().y, -plane->GetNormal().x));

			// Work out the "effective mass" - this is a combination of moment of
			// Inertia and mass, and tells us how much the contact point velocity
			// Will change with the force we're applying
			float mass0 = 1.0f / (1.0f / box->GetMass() + (r * r) / box->GetMoment());

			// And apply the force
			box->ApplyForce(acceleration * mass0, localContact);

			box->SetPosition(box->GetPosition() + (plane->GetNormal() * penetration) * (box->IsKinematic() ? 0.0f : 1.0f));
		}
	}

	return false;
}
bool PhysicsScene::OBB2Circle(PhysicsObject* obb, PhysicsObject* c)
{
	class::OBB* box = dynamic_cast<class::OBB*>(obb);
	Circle* circle = dynamic_cast<Circle*>(c);

	if (box != nullptr && circle != nullptr)
	{
		glm::vec2 circlePos = circle->GetPosition() - box->GetPosition();
		glm::vec2 contact(0, 0); // Contact is in our box coordinates

		float widthExtent = box->GetExtents().x, heightExtent = box->GetExtents().y;
		int numContacts = 0;

		// Check the four corners to see if any of them are inside the circle
		for (float x = -widthExtent; x <= widthExtent; x += box->GetWidth())
		{
			for (float y = -heightExtent; y <= heightExtent; y += box->GetHeight())
			{
				glm::vec2 p = x * box->GetLocalX() + y * box->GetLocalY();
				glm::vec2 dp = p - circlePos;

				if (dp.x * dp.x + dp.y * dp.y < circle->GetRadius() * circle->GetRadius())
				{
					numContacts++;
					contact += glm::vec2(x, y);
				}
			}
		}

		glm::vec2* direction = nullptr;
		// Get the local position of the circle centre
		glm::vec2 localPos(glm::dot(box->GetLocalX(), circlePos), glm::dot(box->GetLocalY(), circlePos));

		if (localPos.y < heightExtent && localPos.y > -heightExtent) 
		{
			if (localPos.x > 0 && localPos.x < widthExtent + circle->GetRadius()) 
			{
				numContacts++;
				contact += glm::vec2(widthExtent, localPos.y);
				direction = new glm::vec2(box->GetLocalX());
			}

			if (localPos.x < 0 && localPos.x > -(widthExtent + circle->GetRadius())) 
			{
				numContacts++;
				contact += glm::vec2(-widthExtent, localPos.y);
				direction = new glm::vec2(-box->GetLocalX());
			}
		}
		if (localPos.x < widthExtent && localPos.x > -widthExtent) 
		{
			if (localPos.y > 0 && localPos.y < heightExtent + circle->GetRadius()) 
			{
				numContacts++;
				contact += glm::vec2(localPos.x, heightExtent);
				direction = new glm::vec2(box->GetLocalY());
			}

			if (localPos.y < 0 && localPos.y > -(heightExtent + circle->GetRadius())) 
			{
				numContacts++;
				contact += glm::vec2(localPos.x, -heightExtent);
				direction = new glm::vec2(-box->GetLocalY());
			}
		}

		if (numContacts > 0) 
		{
			// Average, and convert back into world coords
			contact = box->GetPosition() + (1.0f / numContacts) * (box->GetLocalX() * contact.x + box->GetLocalY() * contact.y);

			box->ResolveCollision(circle, contact, direction);

			float pen = circle->GetRadius() - glm::length(contact - circle->GetPosition());
			glm::vec2 norm = glm::normalize(circle->GetPosition() - contact);

			PhysicsScene::ApplyContactForces(box, circle, norm, pen); // Apply contact forces

			box->CollisionDetected(circle);
			circle->CollisionDetected(box);
		}
		delete direction;
	}

	return false;
}

bool PhysicsScene::OBB2OBB(PhysicsObject* obb1, PhysicsObject* obb2)
{
	class::OBB* box1 = dynamic_cast<class::OBB*>(obb1);
	class::OBB* box2 = dynamic_cast<class::OBB*>(obb2);

	if (box1 != nullptr && box2 != nullptr) 
	{
		glm::vec2 boxPos = box2->GetPosition() - box1->GetPosition();
		glm::vec2 norm(0, 0);
		glm::vec2 contact(0, 0);
		glm::vec2 delta = box2->GetPosition() - box1->GetPosition();

		float pen = 0;
		int numContacts = 0;

		box1->CheckCorners(*box2, contact, numContacts, pen, norm);
		if (box2->CheckCorners(*box1, contact, numContacts, pen, norm)) 
		{
			norm *= -1;
		}

		if (pen > 0) 
		{
			box1->ResolveCollision(box2, contact / float(numContacts), &norm);

			box1->CollisionDetected(box2);
			box2->CollisionDetected(box1);

			PhysicsScene::ApplyContactForces(box1, box2, norm, pen); // Apply contact forces
		}

		return true;
	}

	return false;
}
bool PhysicsScene::OBB2AABB(PhysicsObject* obb, PhysicsObject* aabb)
{
	return false;
}
#pragma endregion

#pragma region AABBCollisions
bool PhysicsScene::AABB2Plane(PhysicsObject* aabb, PhysicsObject* p)
{
	return false;
}
bool PhysicsScene::AABB2Circle(PhysicsObject* aabb, PhysicsObject* c)
{
	return false;
}
bool PhysicsScene::AABB2OBB(PhysicsObject* aabb, PhysicsObject* obb)
{
	return false;
}
bool PhysicsScene::AABB2AABB(PhysicsObject* aabb1, PhysicsObject* aabb2)
{
	return false;
}
#pragma endregion
#pragma endregion


#pragma once
#include "glm/ext.hpp"
#include <vector>

class PhysicsObject;
class RigidBody;

class PhysicsScene
{
public :
	PhysicsScene();
	PhysicsScene(glm::vec2 gravity, float timeStep);
	~PhysicsScene();

	void AddActor(PhysicsObject* actor);
	void RemoveActor(PhysicsObject* actor);

	void Update(float dt);
	void UpdateGizmo();

	void DebugScene();
	static void RemoveNullElements(std::vector<PhysicsObject*>& objs);

	void SetGravity(const glm::vec2 gravity);
	glm::vec2 GetGravity() const;

	void SetTimeStep(const float timeStep);
	float GetTimeStep() const;

	int GetActorsCount();

	static void ApplyContactForces(RigidBody* rb1, RigidBody* rb2, glm::vec2 norm, float pen);

#pragma region Collisions
	void CheckCollision();

	static bool plane2Plane(PhysicsObject* p1, PhysicsObject* p2);
	static bool plane2Circle(PhysicsObject* p, PhysicsObject* c);
	static bool plane2OBB(PhysicsObject* p, PhysicsObject* obb);
	static bool plane2AABB(PhysicsObject* p, PhysicsObject* aabb);


	static bool circle2Circle(PhysicsObject* c1, PhysicsObject* c2);
	static bool circle2Plane(PhysicsObject* c, PhysicsObject* p);
	static bool circle2OBB(PhysicsObject* c, PhysicsObject* obb);
	static bool circle2AABB(PhysicsObject* c, PhysicsObject* aabb);

	static bool OBB2Plane(PhysicsObject* obb, PhysicsObject* p);
	static bool OBB2Circle(PhysicsObject* obb, PhysicsObject* c);
	static bool OBB2OBB(PhysicsObject* obb1, PhysicsObject* obb2);
	static bool OBB2AABB(PhysicsObject* obb, PhysicsObject* aabb);

	static bool AABB2Plane(PhysicsObject* aabb, PhysicsObject* p);
	static bool AABB2Circle(PhysicsObject* aabb, PhysicsObject* c);
	static bool AABB2OBB(PhysicsObject* aabb, PhysicsObject* obb);
	static bool AABB2AABB(PhysicsObject* aabb1, PhysicsObject* aabb2);
#pragma endregion

private:
	glm::vec2 m_gravity = glm::vec2(0, 0);
	float m_timeStep = 0.0f;
	std::vector<PhysicsObject*> m_actors;
};


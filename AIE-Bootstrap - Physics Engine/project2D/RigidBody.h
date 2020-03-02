#pragma once
#include "PhysicsObject.h"
#include "Gizmos.h"
#include "glm/ext.hpp"

static float MIN_SLEEP_LINEAR_THRESHOLD = 1.0f;
static float MIN_SLEEP_ROTATION_THRESHOLD = 0.1f;

static float MIN_LINEAR_THRESHOLD = 0.1f;
static float MIN_ROTATION_THRESHOLD = 0.01f;

class RigidBody : public PhysicsObject
{
public:
	RigidBody(ShapeType shapeID, glm::vec2 position, glm::vec2 velocity, float mass);
	~RigidBody();

	virtual void FixedUpdate(glm::vec2 gravity, float timeStep);
	virtual void Debug();

	void ApplyForce(glm::vec2 force, glm::vec2 pos);
	virtual void ResolveCollision(RigidBody* actor2, glm::vec2 contact, glm::vec2* collisionNormal = nullptr);

	virtual bool CheckCollision(PhysicsObject* pOther) = 0;

	float GetRotationalKineticEnergy();
	float GetKineticEnergy();

	glm::vec2 GetPosition();
	glm::vec2 GetVelocity();

	void SetVelocity(glm::vec2 velocity);
	void SetLinearDrag(float linearDrag);
	void SetAngularDrag(float angularDrag);
	void SetMoment(float moment);
	void SetPosition(glm::vec2 position);
	void SetKinematic(bool isKinematic);

	virtual void CollisionDetected(RigidBody* collisionBody);

	float GetRotation();
	float GetMass();
	float GetLinearDrag();
	float GetAngularDrag();
	float GetAngularVelocity();
	float GetMoment();

	glm::vec2 ToWorld(glm::vec2 position);

	bool IsKinematic();

	static float CalcRotKineticEnergy(RigidBody* rb1, RigidBody* rb2);
	static float CalcKineticEnergy(RigidBody* rb1, RigidBody* rb2);

	float m_elasticity = 1;
	float m_maxAngularVelocity = 1000;

	float m_sleepBreakDist = 0.0f;
	
	bool m_isAsleep = false;

protected:
	glm::vec2 m_position = glm::vec2(0, 0);
	glm::vec2 m_velocity = glm::vec2(0, 0);

	float m_mass = 1;
	float m_moment = 1;

	float m_linearDrag = 0.3f;
	float m_angularDrag = 0.3f;
	float m_angularVelocity = 0;
	float m_rotation = 0;

	bool m_isKinematic = false;

	glm::vec2 m_localX;
	glm::vec2 m_localY;
};


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

	void ApplyForce(glm::vec2 force, glm::vec2 pos); // Apply a force to a position on the rigidbody 
	virtual void ResolveCollision(RigidBody* actor2, glm::vec2 contact, glm::vec2* collisionNormal = nullptr);

	virtual bool CheckCollision(PhysicsObject* pOther) = 0;

	float GetRotationalKineticEnergy(); // Calculate rotational energy
	float GetKineticEnergy(); // Calculate linear energy

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

	glm::vec2 ToWorld(glm::vec2 position); // Convert from local co-ordinates to world co-ordinates

	bool IsKinematic();

	static float CalcRotKineticEnergy(RigidBody* rb1, RigidBody* rb2);
	static float CalcKineticEnergy(RigidBody* rb1, RigidBody* rb2);

	float m_elasticity = 1; // How bouncy a Rigidbody is, how much energy will be lost in a collision
	float m_maxAngularVelocity = 1000;

protected:
	glm::vec2 m_position = glm::vec2(0, 0); // Position of the Rigidbody
	glm::vec2 m_velocity = glm::vec2(0, 0); // Velocity of the Rigidbody

	float m_mass = 1; // Mass of the Rigidbody
	float m_moment = 1; // The moment of inertia 

	float m_linearDrag = 0.3f; // How much a Rigidbody will be slowed down linearly 
	float m_angularDrag = 0.3f; // How much the rotation of a Rigidbody will be slowed down
	float m_angularVelocity = 0;
	float m_rotation = 0; // Current rotation of the Rigidbody

	bool m_isKinematic = false; // Whether or not the Rigidbody can be affected by physics 

	glm::vec2 m_localX = glm::vec2(0, 0);
	glm::vec2 m_localY = glm::vec2(0, 0);
};


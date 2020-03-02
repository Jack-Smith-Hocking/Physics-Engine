#pragma once
#include "PhysicsObject.h"

class RigidBody;

class Spring : public PhysicsObject
{
public:
	Spring(RigidBody* body1, RigidBody* body2, float restLength, float springCoefficient, float maxForce, float dampening = 0.1f, glm::vec2 contact1 = glm::vec2(0, 0), glm::vec2 contact2 = glm::vec2(0, 0));

	void FixedUpdate(glm::vec2 gravity, float timeStep) override;
	void MakeGizmo() override;
	void Debug();

	bool BodyIsNull();
	void CheckIfDestroy();

	virtual void SafeToRemove() override;

protected:
	RigidBody* m_body1 = nullptr;
	RigidBody* m_body2 = nullptr;

	glm::vec2 m_contact1 = glm::vec2(0, 0);
	glm::vec2 m_contact2 = glm::vec2(0, 0);

	float m_dampening = 0;
	float m_restLength = 0;
	float m_springCoefficient = 0;
	float m_maxForce;
};


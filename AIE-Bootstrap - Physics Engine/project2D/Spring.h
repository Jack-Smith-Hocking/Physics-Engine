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
	RigidBody* m_body1 = nullptr; // Body one connected to the spring
	RigidBody* m_body2 = nullptr; // Body two connected to the spring 

	glm::vec2 m_contact1 = glm::vec2(0, 0); // Contact point on body one
	glm::vec2 m_contact2 = glm::vec2(0, 0); // Contact point on body two

	float m_dampening = 0;
	float m_restLength = 0; // How far away the spring will allow the bodies to be 
	float m_springCoefficient = 0;
	float m_maxForce = 0; // The max force the spring will apply
};


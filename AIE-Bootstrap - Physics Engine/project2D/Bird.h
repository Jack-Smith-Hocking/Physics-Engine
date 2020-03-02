#pragma once
#include "Circle.h"
class Bird : public Circle
{
public:
	Bird(glm::vec2 pos, glm::vec2 velocity, float mass, float radius, glm::vec4 colour);
};


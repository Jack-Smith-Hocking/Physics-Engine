#include "LevelManage.h"

#include "Application.h"
#include "Gizmos.h"
#include <iostream>
#include <algorithm>

#include "PhysicsScene.h"
#include "PhysicsObject.h"

#include "Circle.h"
#include "Plane.h"
#include "OBB.h"
#include "KillerBox.h"
#include "CollectibleCircle.h"
#include "Spring.h"

extern glm::vec2 g_windowDimensions;

LevelManage::LevelManage(PhysicsScene* scene, float spawnDelay, int maxSpawns, int spawnRate) : m_scene(scene), m_spawnDelay(spawnDelay), m_maxSpawns(maxSpawns), m_spawnRate(spawnRate)
{
	AddWalls();
	AddFloor();
}

void LevelManage::Update(float deltaTime)
{
	aie::Gizmos::clear();

	m_currentTimer += deltaTime;
	if (m_currentTimer >= m_spawnDelay)
	{
		bool hasSpawned = SpawnRandom(m_spawnRate);

		if (hasSpawned)
		{
			m_currentTimer = 0;
		}
	}

	m_scene->Update(deltaTime);
	m_scene->UpdateGizmo();
}

void LevelManage::AddWalls()
{
	Plane* p = nullptr;
	class::OBB* b = nullptr;

	float x = 105;

	b = new	class::OBB(glm::vec2(x, 0), glm::vec2(0, 0), 1, 3.145f / 2, glm::vec2(300, 10), glm::vec4(1, 1, 1, 0));
	b->SetKinematic(true);
	m_scene->AddActor(b);

	b = new	class::OBB(glm::vec2(-x, 0), glm::vec2(0, 0), 1, 3.145f / 2, glm::vec2(300, 10), glm::vec4(1, 1, 1, 0));
	b->SetKinematic(true);
	m_scene->AddActor(b);

	b = new	class::OBB(glm::vec2(0, 62), glm::vec2(0, 0), 1, 0, glm::vec2(300, 10), glm::vec4(1, 1, 1, 0));
	b->SetKinematic(true);
	m_scene->AddActor(b);
}

int LevelManage::GetMaxSpawns()
{
	return m_maxSpawns;
}

bool LevelManage::SpawnRandom(int spawnCount)
{
	if (m_balls.size() == 0)
	{
		std::cout << "No ball datas were detected!" << std::endl;
		return false;
	}

	float w = g_windowDimensions.x;
	float h = g_windowDimensions.y;

	float randXNeg = (float)(rand() % 2);
	float randYNeg = (float)(rand() % 2);

	float randX = (float)(rand() % 20);
	float randY = (float)(rand() % 20);

	CollectibleCircle* c = nullptr;
	CollectibleCircle* c2 = nullptr;

	BallData* ball = nullptr;
	BallData* ball2 = nullptr;

	int i = 0;

	for (i; i < spawnCount; i++)
	{
		if (m_scene->GetActorsCount() >= m_maxSpawns)
		{
			break;
		}

		glm::vec2 vel(0, 0);
		Spring* spring = nullptr;

		{
			// Get random numbers for spawning
			randXNeg = (float)(rand() % 2);
			randYNeg = (float)(rand() % 2);

			randX = (float)(rand() % 95) * (randXNeg == 1 ? -1 : 1);
			randY = (float)(rand() % 40) * (randYNeg == 1 ? -1 : 1);

			ball = RandomBallData(); // Get a random ball type

			if (ball == nullptr)
			{
				continue;
			}

			float randHasSpring = (float)(rand() % 5);

			if (ball->m_type == CollectibleType::DANGEROUS && ball->m_damage > 0 && randHasSpring == 0) // Check if the ball does damage and can have a spring
			{
				ball2 = RandomBallData(); // Get a new random ball

				if (ball2 != nullptr)
				{
					c2 = new CollectibleCircle(glm::vec2(randX, randY + 5), glm::vec2(0, 0), 1, ball2->m_size, ball2->m_colour); // Create a new collectible ball
					c2->SetKinematic(true);

					float randVNeg = (float)(rand() % 2);

					vel = glm::vec2((randVNeg == 0 ? 1 : -1) * (rand() % 25), (randVNeg == 0 ? 1 : -1) * (rand() % 5)); // Rand velocity for the second colelctible
				}
			}
		}

		c = new CollectibleCircle(glm::vec2(randX, randY), vel, 1, ball->m_size, ball->m_colour, c2); // Create a new collectible item
		c->CollectibleValues(ball->m_type, ball->m_score, ball->m_damage, ball->m_energy, nullptr);
		if (c2 == nullptr)
		{
			c->SetKinematic(true);
		}

		m_scene->AddActor(c);
				
		if (c2 != nullptr)
		{
			spring = new Spring(c, c2, (float)(rand() % 10 + 1), 5, 10000, 0.5f); // Create a new spring joiniing the two collectibles

			c->CollectibleValues(ball->m_type, ball->m_score, ball->m_damage, ball->m_energy, spring);
			c2->CollectibleValues(ball2->m_type, ball2->m_score, ball2->m_damage, ball2->m_energy, spring);

			c->SetLinearDrag(0.1f);
			c->m_elasticity = 0;

			m_scene->AddActor(c2);
			
			if (spring != nullptr)
			{
				m_scene->AddActor(spring);
				spring = nullptr;
			}

			c2 = nullptr;
			vel = glm::vec2(0, 0);
		}

		m_currentSpawns++;
	}

	return i > 0;
}

bool SortBall(LevelManage::BallData* a, LevelManage::BallData* b)
{
	return a->m_spawnChance < b->m_spawnChance;
}

void LevelManage::SortBallData()
{
	std::sort(m_balls.begin(), m_balls.end(), SortBall);
}

void LevelManage::AddBallData(BallData* ballData)
{
	if (ballData != nullptr)
	{
		m_balls.push_back(ballData);
	}
}

LevelManage::BallData* LevelManage::RandomBallData()
{
	/// Terrible way of getting random data ///

	int spawnChance = rand() % 100;

	std::vector<BallData*> possibleSpawns;

	for each (BallData* data in m_balls)
	{
		if (spawnChance <= data->m_spawnChance)
		{
			possibleSpawns.push_back(data);
		}
		else
		{
			break;
		}
	}

	if (possibleSpawns.size() > 0)
	{
		int randIndex = rand() % possibleSpawns.size();

		return possibleSpawns[randIndex];
	}

	int randIndex = rand() % m_balls.size();

	return m_balls[randIndex];
}

void LevelManage::AddFloor()
{
	glm::vec2 currentPos(0, 0);

	class::OBB* safeFloor = nullptr;
	KillerBox* killerFloor = nullptr;

	float randFloor = 1;

	for (int i = 0; i < 10; i++)
	{
		randFloor = (float)(rand() % 2);

		if (randFloor == 0)
		{
			safeFloor = new class::OBB(glm::vec2(-100 + currentPos.x + 25, -57), glm::vec2(0, 0), 1, 0, glm::vec2(50, 10), glm::vec4(1, 1, 1, 1));
			safeFloor->SetKinematic(true);

			currentPos.x += 50;

			m_scene->AddActor(safeFloor);
		}
		else if (randFloor == 1)
		{
			killerFloor = new KillerBox(glm::vec2(-100 + currentPos.x + 12.5f, -57), glm::vec2(0, 0), 1, 0, glm::vec2(25, 5), glm::vec4(1, 0.5f, 0, 1));
			killerFloor->SetKinematic(true);
			
			currentPos.x += 25;

			m_scene->AddActor(killerFloor);
		}
	}
}

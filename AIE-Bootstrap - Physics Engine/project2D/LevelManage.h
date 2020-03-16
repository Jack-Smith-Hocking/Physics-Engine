#pragma once
#include <vector>
#include "CollectibleCircle.h"

class PhysicsScene;
class PhysicsObject;

class LevelManage
{
public:
	// Holds data relevant to making new collectible balls
	class BallData
	{
	public:
		CollectibleType m_type = CollectibleType::NORMAL; // The type of item
		glm::vec4 m_colour = glm::vec4(0, 0, 0, 0); // The colour of the ball
		int m_score = 0; // Score gained
		int m_damage = 0; // Damage done
		float m_size = 1; // Radius 
		float m_spawnChance = 100; // Chance that it will spawn
		float m_energy = 0; // Energy gained

		BallData(CollectibleType type = CollectibleType::NORMAL, int score = 0, float size = 1, int damage = 0, float spawnChance = 10, float energy = 0, glm::vec4 colour = glm::vec4(1, 1, 1, 1))
		{
			m_type = type;
			m_score = score;
			m_size = size;
			m_damage = damage;
			m_spawnChance = spawnChance;
			m_colour = colour;
			m_energy = energy;
		}
	};

	LevelManage(PhysicsScene* scene, float spawnDelay, int maxSpawns, int spawnRate);

	void Update(float deltaTime);

	void AddFloor();
	void AddWalls();

	int GetMaxSpawns();

	// Spawns a random ball in the scene 
	bool SpawnRandom(int spawnCount);

	void SortBallData();
	void AddBallData(BallData* ballData);

	// Get a random ball
	BallData* RandomBallData();

protected:
	std::vector<BallData*> m_balls; // All the balls in the scene 

	PhysicsScene* m_scene = nullptr;

	float m_currentTimer = 0; // Time till next spawn
	float m_spawnDelay = 0; // Delay between spawns
	int m_maxSpawns = 0; // Max amount of spawns
	int m_spawnRate = 0; // How many balls will be spawned at once

	int m_currentSpawns = 0; // Current number of spawns in the scene
};


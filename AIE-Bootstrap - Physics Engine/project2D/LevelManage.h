#pragma once
#include <vector>
#include "CollectibleCircle.h"

class PhysicsScene;
class PhysicsObject;

class LevelManage
{
public:
	class BallData
	{
	public:
		CollectibleType m_type = CollectibleType::NORMAL;
		glm::vec4 m_colour = glm::vec4(0, 0, 0, 0);
		int m_score = 0;
		int m_damage = 0;
		float m_size = 1;
		float m_spawnChance = 100;
		float m_energy = 0;

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

	bool SpawnRandom(int spawnCount);

	void SortBallData();
	void AddBallData(BallData* ballData);
	BallData* RandomBallData();

protected:
	std::vector<BallData*> m_balls;

	PhysicsScene* m_scene = nullptr;

	float m_currentTimer = 0;
	float m_spawnDelay = 0;
	int m_maxSpawns = 0;
	int m_spawnRate = 0;

	int m_currentSpawns = 0;
};


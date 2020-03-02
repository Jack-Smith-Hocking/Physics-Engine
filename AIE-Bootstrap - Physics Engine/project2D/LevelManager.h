#pragma once
#include <vector>

class PhysicsScene;

class LevelManager
{
public:
	LevelManager(PhysicsScene* firstLevel);

	void UpdateScene(float dt);
	
	void IncrementLevel();
	void SetLevel(int levelIndex);

	void AddLevel(PhysicsScene* level);

	int GetLevelCount();

private:
	int m_currentIndex;

	std::vector<PhysicsScene*> m_levels;
};


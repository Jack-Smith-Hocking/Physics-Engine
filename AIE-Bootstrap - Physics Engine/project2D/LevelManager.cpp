#include "LevelManager.h"
#include "PhysicsScene.h"
#include "glm/ext.hpp"

LevelManager::LevelManager(PhysicsScene* firstLevel)
{
	AddLevel(firstLevel);
}

void LevelManager::UpdateScene(float dt)
{
	if (m_currentIndex > m_levels.size() - 1 || m_levels[m_currentIndex] == nullptr)
	{
		return;
	}

	m_levels[m_currentIndex]->Update(dt);
}

void LevelManager::IncrementLevel()
{
	m_currentIndex++;
	SetLevel(m_currentIndex);
}

void LevelManager::SetLevel(int levelIndex)
{
	int index = glm::clamp(levelIndex, 0, GetLevelCount() - 1);

	m_currentIndex = index;
}

void LevelManager::AddLevel(PhysicsScene* level)
{
	if (level != nullptr)
	{
		m_levels.push_back(level);
	}
}

int LevelManager::GetLevelCount()
{
	return m_levels.size();
}

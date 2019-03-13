#include "PCH.h"
#include "ContentManager.h"



CContentManager::CContentManager()
{
}


CContentManager::~CContentManager()
{
}

void CContentManager::Update(float _deltaTime)
{
	// Oldscool c++ foreach
	//for (std::list<CEntity*>::iterator itr = m_entities.begin(); itr != m_entities.end(); itr++)
	//{
	//	(*itr)->Update(_deltaTime);
	//}

	// C++ 11 foreach
	for (auto itr : m_entities)
	{
		itr->Update(_deltaTime);
	}

	for (auto itr : m_entities2D)
	{
		itr->Update(_deltaTime);
	}
	m_cursor->Update(_deltaTime);

	CleanObjects();
}

void CContentManager::Render()
{
	for (auto itr : m_entities)
	{
		itr->Render();
	}

	m_spriteBatch->Begin();
	for (auto itr : m_entities2D)
	{
		itr->Render();
	}
	m_cursor->Render();
	m_spriteBatch->End();
}

bool CContentManager::AddEntity(CEntity * _entity)
{
	if (!_entity)
	{
		return false;
	}

	bool value = _entity->Initialize();
	if (value)
	{
		m_entities.push_back(_entity);
	}

	return value;
}

bool CContentManager::RemoveEntity(CEntity * _entity)
{
	if (!ContainsEntity(_entity))
	{
		return false;
	}

	m_entitiesToDelete.push_back(_entity);

	return true;
}

bool CContentManager::ContainsEntity(CEntity * _entity)
{
	if (!_entity)
	{
		return false;
	}

	for (auto itr : m_entities)
	{
		if (itr == _entity)
		{
			return true;
		}
	}

	return false;
}

bool CContentManager::AddEntity(CEntity2D * _entity)
{
	if (!_entity)
	{
		return false;
	}

	bool value = _entity->Initialize();
	if (value)
	{
		m_entities2D.push_back(_entity);
	}

	return value;
}

bool CContentManager::RemoveEntity(CEntity2D* _entity)
{
	if (!ContainsEntity(_entity))
	{
		return false;
	}

	m_entities2DToDelete.push_back(_entity);

	return true;
}

bool CContentManager::ContainsEntity(CEntity2D* _entity)
{
	if (!_entity)
	{
		return false;
	}

	for (auto itr : m_entities2D)
	{
		if (itr == _entity)
		{
			return true;
		}
	}

	return false;
}

void CContentManager::CleanObjects()
{
	for (auto itr : m_entitiesToDelete)
	{
		m_entities.remove(itr);
		delete itr;					// Sonst speicherleck
	}

	m_entitiesToDelete.clear();

	for (auto itr : m_entities2DToDelete)
	{
		m_entities2D.remove(itr);
		delete itr;					// Sonst speicherleck
	}

	m_entities2DToDelete.clear();
}

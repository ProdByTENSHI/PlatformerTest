#include "entity/EntityManager.h"

#include <string>

#include "core/Globals.h"

namespace tenshi
{
	EntityManager::EntityManager()
	{
	}

	void EntityManager::Save()
	{
		std::fstream _stream(DATA_PATH, std::ios::out | std::ios::app);

		for (auto& entity : m_Entities)
		{
			json _data = entity.second->Serialize();
			_stream << _data;
		}

		_stream.close();
	}

	void EntityManager::Load()
	{
	}

#pragma region Create Entity Method Overloads

#pragma endregion
	Entity* EntityManager::GetEntity(u32 id)
	{
		return m_Entities[id];
	}

	u32 EntityManager::GetEntityCount()
	{
		return m_Entities.size();
	}

	u32 EntityManager::GetFreeId()
	{
		u32 _id = 0;
		if (m_NextEntityIds.size() > 1)
			_id = m_NextEntityIds.top();
		else
			_id = m_TotalEntityCount;

		return _id;
	}
}
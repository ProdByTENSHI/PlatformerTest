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
		std::fstream _stream(DATA_PATH, std::ios::out);
		_stream << "[";

		for (auto& entity : m_Entities)
		{
			json _data = entity.second->Serialize();
			_stream << _data;
			_stream << ",";
		}

		_stream << "]";

		_stream.close();

		// Clear Entities
		i32 _entityCount = m_Entities.size();
		for (i32 i = 0; i < _entityCount; i++)
		{
			DestroyEntity(m_Entities[i]->m_EntityId);
		}
	}

	void EntityManager::Load()
	{
		// Parse Data and spawn in Entities with the Data
		std::fstream _stream(DATA_PATH, std::ios::in);
		if (!_stream.is_open())
		{
			std::cerr << "[ENTITYMANAGER] Could not read Data File " << DATA_PATH << std::endl;
			_stream.close();
			return;
		}

		json _data;
		_stream >> _data;
		std::cout << "Loaded Data: " << _data << std::endl;

		_stream.close();
	}

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

	void EntityManager::DestroyEntity(u32 entityId)
	{
		if (m_Entities[entityId] == nullptr)
			return;

		delete m_Entities[entityId];
		m_Entities.erase(entityId);

		OnEntityDestroyed.Dispatch(entityId);
	}
}
#include "entity/EntityManager.h"

#include <string>
#include <sstream>

#include "core/Globals.h"

namespace tenshi
{
	EntityManager::EntityManager()
	{
	}

	void EntityManager::Save()
	{
		std::fstream _stream(DATA_PATH, std::ios::out);

		json _finalData;
		for (auto& entity : m_Entities)
		{
			json _data = entity.second->Serialize();
			_data["type"] = entity.second->GetType();
			_finalData.push_back(_data);
		}

		_stream << _finalData;
		_stream.close();

		// Clear Entities
		i32 _entityCount = m_Entities.size();
		for (i32 i = 0; i < _entityCount; i++)
		{
			if (!m_Entities[i])
				continue;

			DestroyEntity(m_Entities[i]->m_EntityId);
		}

		for (i32 i = 0; i < m_NextEntityIds.size(); i++)
		{
			m_NextEntityIds.pop();
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

		json _data = json::parse(_stream);
		_stream.close();

		for (auto& entity : _data.items())
		{
			EntityType::EntityType _type = entity.value()["type"];
			switch (_type)
			{
			case EntityType::SpriteEntity:
				json _path = entity.value()["texturePath"];
				std::string _texturePath = _path.dump();
				SpriteEntity& _entity = g_EntityManager->CreateEntity<SpriteEntity>
					(g_ResourceManager->GetTexture(_texturePath.substr(1, _texturePath.length() - 2)));
				_entity.Deserialize(entity.value());
				g_MasterRenderer->AddStaticEntity(_entity.m_EntityId, _entity.m_Sprite->m_Texture);
				break;
			}
		}
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
		if (m_NextEntityIds.size() >= 1)
		{
			_id = m_NextEntityIds.top();
			m_NextEntityIds.pop();
		}
		else
			_id = m_Entities.size();

		return _id;
	}

	void EntityManager::DestroyEntity(u32 entityId)
	{
		if (m_Entities[entityId] == nullptr)
			return;

		delete m_Entities[entityId];
		m_Entities.erase(entityId);

		m_NextEntityIds.push(entityId);

		OnEntityDestroyed.Dispatch(entityId);
	}
}
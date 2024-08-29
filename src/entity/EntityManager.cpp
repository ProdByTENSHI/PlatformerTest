#include "entity/EntityManager.h"

namespace tenshi
{
#pragma region Create Entity Method Overloads
	SpriteEntity& EntityManager::CreateEntity(std::shared_ptr<Texture> texture)
	{
		u32 _id = GetFreeId();

		SpriteEntity* _entity = new SpriteEntity(_id, texture);
		m_Entities.insert(std::make_pair(_id, _entity));

		return *_entity;
	}

#pragma endregion

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
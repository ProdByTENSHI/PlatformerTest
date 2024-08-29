#include "entity/EntityManager.h"

#include <string>

#include "core/Globals.h"

namespace tenshi
{
#pragma region Create Entity Method Overloads

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
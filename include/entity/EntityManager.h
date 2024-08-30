#pragma once

#include <map>
#include <stack>
#include <concepts>
#include <iostream>
#include <memory>

#include "entity/Entity.h"
#include "tenshiUtil/Types.h"
#include "entity/SpriteEntity.h"
#include "entity/SpriteSheetEntity.h"

namespace tenshi
{
	template <typename Derived>
	concept IsBaseOfEntity = std::is_base_of<Entity, Derived>::value;

	class EntityManager
	{
	public:
		u32 GetEntityCount();
		Entity* GetEntity(u32 id);

	public:
		// [typename] T: Derived Class of Entity
		// [typename] Args: Parameter Pack with all Arguments equals to the Constructor Args of T
		template <IsBaseOfEntity T, typename... Args> T& CreateEntity(Args... args)
		{
			u32 _id = GetFreeId();

			T* entity = new T(_id, args...);
			m_Entities.insert(std::make_pair(_id, entity));

			++m_TotalEntityCount;

			return *entity;
		}

		template <IsBaseOfEntity T> T* GetEntity(u32 id)
		{
			return dynamic_cast<T*>(m_Entities[id]);
		}

#pragma endregion

	private:
		std::map<u32, Entity*> m_Entities;

		// Stores the next Entity ID
		// When deleting an Entity the ID becomes avaiable for a new Entity
		// Those freed Entity IDs get stored on the next entity ids Stack
		// When creating an Entity the ID is taken of the Top of the Stack
		std::stack<u32> m_NextEntityIds;

		// Doesnt shrink when deleting an Entity
		u32 m_TotalEntityCount = 0;

	private:
		u32 GetFreeId();
	};
}
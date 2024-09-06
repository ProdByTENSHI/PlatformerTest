#pragma once

#include <map>
#include <stack>
#include <concepts>
#include <iostream>
#include <fstream>
#include <memory>

#include "entity/Entity.h"
#include "tenshiUtil/Types.h"
#include "entity/SpriteEntity.h"
#include "entity/SpriteSheetEntity.h"
#include "entity/AnimatedEntity.h"

namespace tenshi
{
	template <typename Derived>
	concept IsBaseOfEntity = std::is_base_of<Entity, Derived>::value;

	class EntityManager
	{
	public:
		EntityManager();

		u32 GetEntityCount();
		Entity* GetEntity(u32 id);

		// Serializes all Entities and writes them into the DATA_PATH File
		void Save();

		// Deserializes all Entities in the DATA_PATH File and spawns in the Entity if it doesnt exist yet
		void Load();

		// Destroy the given Entity and invokes the OnEntityDestroy Event
		void DestroyEntity(u32 entityId);

	public:
		// [typename] T: Derived Class of Entity
		// [typename] Args: Parameter Pack with all Arguments equals to the Constructor Args of T
		template <IsBaseOfEntity T, typename... Args> T& CreateEntity(Args... args)
		{
			u32 _id = GetFreeId();

			T* entity = new T(_id, args...);
			m_Entities.insert_or_assign(_id, entity);

			std::cout << "[ENTITY MANAGER] Spawned in Entity with ID " << _id << std::endl;

			return *entity;
		}

		template <IsBaseOfEntity T> T* GetEntity(u32 id)
		{
			return dynamic_cast<T*>(m_Entities[id]);
		}

	public:
		Event<u32> OnEntityDestroyed;

	private:
		std::map<u32, Entity*> m_Entities;

		// Stores the next Entity ID
		// When deleting an Entity the ID becomes avaiable for a new Entity
		// Those freed Entity IDs get stored on the next entity ids Stack
		// When creating an Entity the ID is taken of the Top of the Stack
		std::stack<u32> m_NextEntityIds;

		// Path to the Entity Data File
		const std::string DATA_PATH = "entities.json";

	private:
		u32 GetFreeId();
	};
}
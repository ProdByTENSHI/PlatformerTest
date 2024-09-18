#pragma once

#include <map>
#include <iostream>

#include "tenshiUtil/Types.h"
#include "ecs/Component.h"
#include "ecs/ECS_Definitions.h"

namespace tenshi
{
	// Needed to store all Component Arrays by their Point in one Map instead of manually creating an Array for each Type
	struct BaseComponentArray
	{
		virtual ~BaseComponentArray() {}

		void OnEntityDestroyed(Entity entity)
		{
			m_EntityToComponent.erase(entity);
		}

		// [Key]: Entity
		// [Value]: Pointer to Component
		std::map<Entity, Component*> m_EntityToComponent;
	};

	template <class T> class ComponentArray : public BaseComponentArray
	{
	public:
		void AddComponent(Entity entity, T& component)
		{
			if (m_EntityToComponent[entity] != nullptr)
			{
				std::cout << "[COMPONENTARRAY] Entity already contains Component" << std::endl;
				return;
			}

			m_EntityToComponent.insert(std::make_pair(entity, &component));
		}

		void RemoveComponent(Entity entity)
		{
			for (auto& pair : m_EntityToComponent)
			{
				if (pair.first != entity)
					continue;

				m_EntityToComponent.erase(entity);
			}

			std::cout << "[COMPONENTARRAY] Entity " << entity << " had no Component of this Type" << std::endl;
		}

		T* GetComponent(Entity entity)
		{
			return dynamic_cast<T*>(m_EntityToComponent[entity]);
		}
	};
}

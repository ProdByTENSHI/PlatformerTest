#pragma once

#include <type_traits>
#include <memory>
#include <map>
#include <stack>
#include <iostream>

#include "tenshiUtil/Types.h"
#include "ecs/ECS_Definitions.h"
#include "ecs/Component.h"
#include "ecs/System.h"
#include "ecs/ComponentArray.h"
#include "ecs/systems/SpriteRenderer.h"

namespace tenshi
{
	template <typename Derived> concept IsBaseOfComponent = std::is_base_of<Component, Derived>::value;
	template <typename Derived> concept IsBaseOfSystem = std::is_base_of<System, Derived>::value;

	class Ecs
	{
	public:
		Ecs();
		~Ecs();

		Entity CreateEntity();
		void DestroyEntity(Entity entity);
		void Init();

	public:
		template <IsBaseOfComponent T> void AddComponent(Entity entity, T& component)
		{
			ComponentType _type = component.m_Type;
			if (_type == ComponentType::InvalidType)
			{
				std::cout << "[ECS] Cannot add Base Component to Entity " << entity << std::endl;
				return;
			}

			if (!m_ComponentArrays[typeid(T).name])
				m_ComponentArrays[typeid(T).name] = new ComponentArray<T>();

			m_ComponentArrays[typeid(T).name()]->m_EntityToComponent
				.insert(std::make_pair(entity, &component));
			m_EntitySignatures[entity].set(_type);
			m_OnEntitySignatureChange.Dispatch(entity, m_EntitySignatures[entity]);
		}

		template <IsBaseOfComponent T> void RemoveComponent(Entity entity, ComponentType type)
		{
			m_ComponentArrays[typeid(T).name()]->m_EntityToComponent.erase(entity);
			m_EntitySignatures[entity].set(type, false);
			m_OnEntitySignatureChange.Dispatch(entity, m_EntitySignatures[entity]);
		}

		// Pretty Expensive Operation so use this once, Cache the Pointer instead of every Frame
		template <IsBaseOfComponent T> T* GetComponent(Entity entity)
		{
			return dynamic_cast<T*>(m_ComponentArrays[typeid(T).name()]
				->m_EntityToComponent[entity]);
		}

		template <IsBaseOfSystem T> std::unique_ptr<System> CreateSystem(Signature signature)
		{
			std::unique_ptr<T> system = std::make_unique<T>(signature, m_Systems.size());
			m_Systems.insert(std::make_pair(system->m_Id, system));
		}

		void Serialize();
		void Deserialize();

	public:
		// -- SYSTEMS
		Event<Entity, Signature> m_OnEntitySignatureChange;

		std::unique_ptr<SpriteRenderer> m_SpriteRenderer;

	private:
		// Holds Component Arrays by their typename
		std::map<const char*, BaseComponentArray*> m_ComponentArrays;
		std::map<u8, std::unique_ptr<System>> m_Systems;
		std::map<Entity, Signature> m_EntitySignatures;
		std::stack<Entity> m_EntityIdStack;

		u32 m_EntityCount = 0;

		const std::string ENTITIES_DATA_PATH = "entities.json";

	private:
		void InitSystems();

		Entity GetNextEntityId();
	};
}
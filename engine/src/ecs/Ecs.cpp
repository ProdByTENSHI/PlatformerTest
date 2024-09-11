#include "ecs/Ecs.h"

#include <fstream>

namespace tenshi
{
	Ecs::Ecs()
	{
	}

	Ecs::~Ecs()
	{
	}

	Entity Ecs::CreateEntity()
	{
		Entity _entity = GetNextEntityId();
		++m_EntityCount;

		return _entity;
	}

	void Ecs::DestroyEntity(Entity entity)
	{
		for (auto& arr : m_ComponentArrays)
		{
			arr.second->OnEntityDestroyed(entity);
		}

		m_EntitySignatures[entity].reset();
		m_OnEntitySignatureChange.Dispatch(entity, m_EntitySignatures[entity]);

		m_EntityIdStack.push(entity);
		--m_EntityCount;
	}

	void Ecs::Serialize()
	{
		json _data;

		for (auto& arr : m_ComponentArrays)
		{
			for (auto& component : arr.second->m_EntityToComponent)
			{
				_data.push_back(component.second->Serialize());
			}
		}

		std::fstream _stream(ENTITIES_DATA_PATH, std::ios::out);
		_stream << _data;
		_stream.close();
	}

	void Ecs::Deserialize()
	{
	}

	void Ecs::InitSystems()
	{
		Signature _sig;
		_sig.set(ComponentType::TransformType);
		_sig.set(ComponentType::SpriteType);
		m_SpriteRenderer = std::make_unique<SpriteRenderer>(_sig, 0);
		_sig.reset();
	}

	u32 Ecs::GetNextEntityId()
	{
		u32 _id = 0;
		if (m_EntityIdStack.size() > 0)
		{
			_id = m_EntityIdStack.top();
			m_EntityIdStack.pop();
		}
		else
		{
			_id = m_EntityCount;
		}

		return _id;
	}

	void Ecs::Init()
	{
		InitSystems();
	}
}
#include "entity/Entity.h"

#include <iostream>

namespace tenshi
{
	Entity::Entity(u32 id)
		: m_EntityId(id)
	{
	}

	Entity::~Entity()
	{
		std::cout << "Entity: " << m_EntityId << " was destroyed" << std::endl;
	}

	json Entity::Serialize()
	{
		json _data;
		_data["id"] = m_EntityId;
		_data["name"] = m_EntityName;
		_data["position"] = { m_Transform.m_Position.x, m_Transform.m_Position.y };
		_data["rotation"] = { m_Transform.m_Rotation.x, m_Transform.m_Rotation.y };
		_data["scale"] = { m_Transform.m_Scale.x, m_Transform.m_Scale.y };

		return _data;
	};

	void Entity::Deserialize(const json& data)
	{
		m_EntityName = data["name"];
		m_Transform.Scale(glm::vec2(data["scale"][0], data["scale"][1]));
		m_Transform.Rotate(glm::vec2(1.0f, 0.0f), data["rotation"][0]);
		m_Transform.Rotate(glm::vec2(0.0f, 1.0f), data["rotation"][1]);
		m_Transform.Translate(glm::vec2(data["position"][0], data["position"][1]));
	}

	EntityType::EntityType Entity::GetType()
	{
		return EntityType::Invalid;
	}
}
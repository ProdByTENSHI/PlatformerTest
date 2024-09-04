#include "entity/Entity.h"

namespace tenshi
{
	Entity::~Entity()
	{
		std::cout << "Entity: " << m_EntityId << " was destroyed" << std::endl;
	}

	json Entity::Serialize()
	{
		json _data;
		_data["id"] = m_EntityId;
		_data["name"] = m_EntityName;
		_data["position"] = { {m_Transform.m_Position.x, m_Transform.m_Position.y} };
		_data["rotation"] = { {m_Transform.m_Rotation.x, m_Transform.m_Rotation.y} };
		_data["scale"] = { {m_Transform.m_Scale.x, m_Transform.m_Scale.y} };

		std::cout << _data << std::endl;

		return _data;
	};

	void Entity::Deserialize(json& data)
	{
	}
}
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
		_data[m_EntityId]["id"] = m_EntityId;
		_data[m_EntityId]["transform"]["position"] = { {"x", m_Transform.m_Position.x}, {"y",m_Transform.m_Position.y} };
		_data[m_EntityId]["transform"]["scale"] = { {"x", m_Transform.m_Scale.x}, {"y", m_Transform.m_Scale.y} };

		std::cout << _data << std::endl;

		return _data;
	};

	void Entity::Deserialize(json& data)
	{
	}
}
#include "entity/SpriteEntity.h"

#include <iostream>

namespace tenshi
{
	SpriteEntity::SpriteEntity(u32 id, std::shared_ptr<Texture> texture)
		: Entity(id)
	{
		m_Sprite = new Sprite(texture);
	}

	SpriteEntity::~SpriteEntity()
	{
		delete m_Sprite;
	}

	EntityType::EntityType SpriteEntity::GetType()
	{
		return EntityType::SpriteEntity;
	}

	json SpriteEntity::Serialize()
	{
		json _data = Entity::Serialize();
		_data["texturePath"] = m_Sprite->m_Texture->GetName();

		return _data;
	}

	void SpriteEntity::Deserialize(const json& data)
	{
		Entity::Deserialize(data);
	}
}
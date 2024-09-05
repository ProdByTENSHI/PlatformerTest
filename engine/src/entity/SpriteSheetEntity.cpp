#include "entity/SpriteSheetEntity.h"

#include <iostream>

namespace tenshi
{
	SpriteSheetEntity::SpriteSheetEntity(u32 id, std::shared_ptr<SpriteSheet> spriteSheet)
		: Entity(id)
	{
		m_SpriteSheet = spriteSheet;
	}

	void SpriteSheetEntity::SetFrame(u32 index)
	{
		u32 _prev = m_CurrentFrame;
		m_CurrentFrame = index;
		OnFrameChange.Dispatch(_prev, index);
	}

	EntityType::EntityType SpriteSheetEntity::GetType()
	{
		return EntityType::SpriteSheetEntity;
	}

	json SpriteSheetEntity::Serialize()
	{
		json _data = Entity::Serialize();
		_data["texturePath"] = m_SpriteSheet->m_Texture->GetName();
		_data["frameWidth"] = m_SpriteSheet->FRAME_WIDTH;
		_data["frameHeight"] = m_SpriteSheet->FRAME_HEIGHT;

		return _data;
	}

	void SpriteSheetEntity::Deserialize(const json& data)
	{
		Entity::Deserialize(data);
	}
}
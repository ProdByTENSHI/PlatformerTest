#include "entity/SpriteSheetEntity.h"

#include <iostream>

namespace tenshi
{
	SpriteSheetEntity::SpriteSheetEntity(u32 id, SpriteSheet& spriteSheet)
		: Entity(id)
	{
		m_SpriteSheet = &spriteSheet;
	}

	void SpriteSheetEntity::SetFrame(u32 index)
	{
		m_CurrentFrame = index;
		OnFrameChange.Dispatch();
	}
}
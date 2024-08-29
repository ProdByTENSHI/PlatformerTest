#include "entity/SpriteSheetEntity.h"

namespace tenshi
{
	SpriteSheetEntity::SpriteSheetEntity(u32 id, SpriteSheet* spriteSheet)
		: Entity(id)
	{
		m_SpriteSheet = spriteSheet;
	}

	void SpriteSheetEntity::SetFrame(u8 hIndex, u8 vIndex)
	{
		m_CurrentHFrame = hIndex;
		m_CurrentVFrame = vIndex;
	}
}
#pragma once

#include "entity/Entity.h"
#include "resources/SpriteSheet.h"

#include "tenshiUtil/eventsystem/EventSystem.h"

namespace tenshi
{
	struct SpriteSheetEntity : virtual public Entity
	{
		SpriteSheetEntity(u32 id, SpriteSheet& spriteSheet);

		void SetFrame(u32 index);

		SpriteSheet* m_SpriteSheet = nullptr;

		// Between 0 and TOTAL_FRAMES - 1
		u32 m_CurrentFrame = 0;
		const u32 TOTAL_FRAMES = 0;

		Event<> OnFrameChange;
	};
}
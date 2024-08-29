#pragma once

#include "entity/Entity.h"
#include "resources/SpriteSheet.h"

namespace tenshi
{
	struct SpriteSheetEntity : virtual public Entity
	{
		SpriteSheetEntity(u32 id, SpriteSheet* spriteSheet);

		void SetFrame(u8 hIndex, u8 vIndex);

		SpriteSheet* m_SpriteSheet = nullptr;
		std::vector<Vertex> m_Vertices;

		u8 m_CurrentHFrame = 0;
		u8 m_CurrentVFrame = 0;
	};
}
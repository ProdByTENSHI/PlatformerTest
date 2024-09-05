#pragma once

#include "entity/Entity.h"
#include "resources/SpriteSheet.h"

#include "tenshiUtil/eventsystem/EventSystem.h"

namespace tenshi
{
	struct SpriteSheetEntity : virtual public Entity
	{
		SpriteSheetEntity(u32 id, std::shared_ptr<SpriteSheet> spriteSheet);

		EntityType::EntityType GetType() override;
		json Serialize() override;
		void Deserialize(const json& data) override;

		void SetFrame(u32 index);

		std::shared_ptr<SpriteSheet> m_SpriteSheet = nullptr;

		// Between 0 and TOTAL_FRAMES - 1
		u32 m_CurrentFrame = 0;
		const u32 TOTAL_FRAMES = 0;

		// Previous Frame, New Frame
		Event<u32, u32> OnFrameChange;
	};
}
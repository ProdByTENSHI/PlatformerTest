#pragma once

#include "ecs/Component.h"
#include "resources/SpriteSheet.h"
#include "tenshiUtil/eventsystem/EventSystem.h"

namespace tenshi
{
	class SpriteSheetComponent : public virtual Component
	{
	public:
		SpriteSheetComponent();
		explicit SpriteSheetComponent(std::shared_ptr<SpriteSheet> spriteSheet);

		json Serialize() override;
		void Deserialize(const json& data) override;

		void SetFrame(u32 frame);

	public:
		const ComponentType m_Type = ComponentType::SpriteSheetType;
		std::shared_ptr<SpriteSheet> m_SpriteSheet = nullptr;
		u32 m_CurrentFrame = 0;

		Event<u32, u32> m_OnFrameChange;
	};
}
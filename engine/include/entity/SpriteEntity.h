#pragma once

#include <memory>

#include "entity/Entity.h"
#include "resources/Sprite.h"

namespace tenshi
{
	struct SpriteEntity : virtual public Entity
	{
		SpriteEntity(u32 id, std::shared_ptr<Texture> texture);
		~SpriteEntity();

		EntityType::EntityType GetType() override;
		json Serialize() override;
		void Deserialize(const json& data) override;

		Sprite* m_Sprite = nullptr;
	};
}
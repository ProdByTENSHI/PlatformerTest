#pragma once

#include <memory>

#include "entity/Entity.h"
#include "resources/Sprite.h"

namespace tenshi
{
	struct SpriteEntity : virtual public Entity
	{
		SpriteEntity(u32 id, std::shared_ptr<Texture> texture)
			: Entity(id)
		{
			m_Sprite = new Sprite(texture);
		}

		~SpriteEntity()
		{
			delete m_Sprite;
		}

		const Sprite* m_Sprite = nullptr;
	};
}
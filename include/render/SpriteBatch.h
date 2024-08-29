#pragma once

#include <iostream>
#include <vector>

#include "entity/SpriteEntity.h"

namespace tenshi
{
	inline const u32 MAX_SPRITES_PER_BATCH = 1000;

	// Batch of a Sprite
	// All Sprites that have the same Texture will be rendered in the same Batch)
	struct SpriteBatch
	{
		std::vector<const SpriteEntity*> m_Entities;

		GLuint m_Vao = 0;
		GLuint m_Vbo = 0;

		bool IsTextureInBatch(const SpriteEntity& entity)
		{
			if (m_Entities.size() <= 0)
				return false;

			return entity.m_Sprite->m_Texture->m_Id == m_Entities[0]->m_Sprite->m_Texture->m_Id;
		}
	};
}
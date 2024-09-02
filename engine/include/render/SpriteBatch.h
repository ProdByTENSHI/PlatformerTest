#pragma once

#include <iostream>
#include <vector>

#include "entity/SpriteEntity.h"

namespace tenshi
{
	inline const u32 MAX_SPRITES_PER_BATCH = 1000;

	// All Entities that have the same Texture(Batched or not) are in one Batch
	struct SpriteBatch
	{
		SpriteBatch(std::shared_ptr<Texture> texture) : m_Texture(texture) {}

		std::shared_ptr<Texture> m_Texture = nullptr;

		// Holds all Entities that are in this Batch by their IDs
		// Used to update their Transforms, etc before Drawing
		std::vector<u32> m_EntityIds;

		GLuint m_Vao = 0;
		GLuint m_Vbo = 0;

		bool IsTextureInBatch(std::shared_ptr<Texture> texture)
		{
			if (!m_Texture)
				return false;

			return texture->m_Id == m_Texture->m_Id;
		}
	};
}
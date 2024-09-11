#pragma once

#include <iostream>
#include <vector>

#include "resources/Texture.h"
#include "tenshiUtil/container/STLUtil.h"

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

		void RemoveEntity(u32 entityId)
		{
			std::vector<u32>::iterator _it = STLUtil::GetItOfElementInVec<u32>(m_EntityIds, entityId);
			m_EntityIds.erase(_it);
		}

		bool IsTextureInBatch(std::shared_ptr<Texture> texture)
		{
			if (!m_Texture)
				return false;

			return texture->m_Id == m_Texture->m_Id;
		}

		bool IsEntityInBatch(u32 entityId)
		{
			for (auto& entity : m_EntityIds)
			{
				if (entity == entityId)
					return true;
			}

			return false;
		}
	};
}
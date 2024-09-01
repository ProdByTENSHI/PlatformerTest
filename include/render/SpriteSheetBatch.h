#pragma once

#include <vector>
#include <map>

#include "entity/SpriteSheetEntity.h"

namespace tenshi
{
	// All Entities that have the same Texture are in one Batch
	// Entities that have the same Frame in the Spritesheet get Instanced Rendered
	struct SpriteSheetBatch
	{
		SpriteSheetBatch(std::shared_ptr<Texture> texture) : m_Texture(texture) {}

		std::shared_ptr<Texture> m_Texture = nullptr;

		// Holds all Entities by the Frame they are currently in
		std::map<u32, std::vector<u32>> m_FrameToEntityId;

		GLuint m_Vao = 0;
		GLuint m_Vbo = 0;

		bool IsTextureInBatch(std::shared_ptr<Texture> texture)
		{
			if (!m_Texture)
				return false;

			return texture->m_Id == m_Texture->m_Id;
		}
	};
};
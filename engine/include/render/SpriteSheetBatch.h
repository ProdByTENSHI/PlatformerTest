#pragma once

#include <vector>
#include <map>
#include <algorithm>

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

		// Moves the Entity from its current Frame in the Map to the new one
		void MoveEntity(SpriteSheetEntity& entity, u32 previousFrame, u32 newFrame)
		{
			auto& _vec = m_FrameToEntityId[previousFrame];

			bool _wasDeleted = false;
			// Todo: Rewrite this to use STLUtil Functionality
			for (i32 i = 0; i < _vec.size(); i++)
			{
				if (_vec[i] != entity.m_EntityId)
					continue;

				_vec.erase(std::next(_vec.begin(), i));
				_wasDeleted = true;
			}

			if (_wasDeleted)
				m_FrameToEntityId[newFrame].push_back(entity.m_EntityId);
		}

		void RemoveEntity(u32 entityId)
		{
			for (i32 i = 0; i < m_FrameToEntityId.size(); i++)
			{
				std::vector<u32>& entities = m_FrameToEntityId[i];
				for (auto& entity : entities)
				{
					if (entity != entityId)
						continue;

					std::vector<u32>::iterator _it = STLUtil::GetItOfElementInVec<u32>(entities, entity);
					entities.erase(_it);
				}
			}
		}

		bool IsTextureInBatch(std::shared_ptr<Texture> texture)
		{
			if (!m_Texture)
				return false;

			return texture->m_Id == m_Texture->m_Id;
		}

		bool IsEntityInBatch(u32 entityId)
		{
			for (auto& frame : m_FrameToEntityId)
			{
				for (auto& entity : frame.second)
				{
					if (entity == entityId)
						return true;
				}
			}

			return false;
		}
	};
};
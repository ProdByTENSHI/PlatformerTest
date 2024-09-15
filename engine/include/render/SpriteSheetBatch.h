#pragma once

#include <vector>
#include <map>
#include <algorithm>
#include <memory>

#include "resources/Texture.h"
#include "tenshiUtil/container/STLUtil.h"

namespace tenshi
{
	// All Entities that have the same Texture are in one Batch
	// Entities that have the same Frame in the Spritesheet get Instanced Rendered
	struct SpriteSheetBatch
	{
		SpriteSheetBatch(std::shared_ptr<Texture> texture) : m_Texture(texture) {}

		std::shared_ptr<Texture> m_Texture = nullptr;

		// Holds all Entities by the Frame they are currently in
		std::map<u32, std::vector<Entity>> m_FrameToEntityId;

		GLuint m_Vao = 0;
		GLuint m_Vbo = 0;

		// Moves the Entity from its current Frame in the Map to the new one
		void MoveEntity(Entity entity, u32 previousFrame, u32 newFrame)
		{
			auto& _vec = m_FrameToEntityId[previousFrame];

			bool _wasDeleted = false;
			// Todo: Rewrite this to use STLUtil Functionality
			for (i32 i = 0; i < _vec.size(); i++)
			{
				if (_vec[i] != entity)
					continue;

				_vec.erase(std::next(_vec.begin(), i));
				_wasDeleted = true;
			}

			if (_wasDeleted)
				m_FrameToEntityId[newFrame].push_back(entity);
		}

		void RemoveEntity(Entity entity)
		{
			for (i32 i = 0; i < m_FrameToEntityId.size(); i++)
			{
				std::vector<Entity>& entities = m_FrameToEntityId[i];
				for (auto& entity : entities)
				{
					if (entity != entity)
						continue;

					std::vector<u32>::iterator _it = STLUtil::GetItOfElementInVec<Entity>(entities, entity);
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

		bool IsEntityInBatch(Entity entity)
		{
			for (auto& frame : m_FrameToEntityId)
			{
				for (auto& val : frame.second)
				{
					if (entity == val)
						return true;
				}
			}

			return false;
		}
	};
};
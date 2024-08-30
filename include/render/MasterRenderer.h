#pragma once

#include <vector>
#include <type_traits>

#include "resources/Shader.h"
#include "render/SpriteBatch.h"
#include "entity/SpriteEntity.h"
#include "entity/SpriteSheetEntity.h"
#include "tenshiUtil/memory/Ssbo.h"
#include "tenshiUtil/memory/UniformBuffer.h"

namespace tenshi
{
	// -- SSBO BINDING POINTS
#define SPRITE_TRANSFORMS_SSBO_BIMDING_POINT 0

	// -- UBO BINDING POINTS
#define ENTITY_IDS_UBO_BIINDING_POINT 0

	class MasterRenderer
	{
	public:
		MasterRenderer();

		void AddEntity(u32 entityId, std::shared_ptr<Texture> texture);

		void Render();

	public:
		Ssbo m_TransformsSsbo;

		// [baseOffset + draw call id] = Entity ID
		UniformBuffer m_EntityIdsUbo;

	private:
		std::shared_ptr<Shader> m_SpriteShader = nullptr;

		// Vector that holds all Sprite Batches
		std::vector<SpriteBatch*> m_SpriteBatches;
	};
}
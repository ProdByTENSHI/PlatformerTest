#pragma once

#include <vector>
#include <type_traits>

#include "resources/Shader.h"
#include "render/SpriteBatch.h"
#include "render/SpriteSheetBatch.h"
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

		// Adds an Entity that doesnt change its Vertex Data
		// Returns the Index to the Sprite Batch it is stored in
		// [in] entityId: ID of the Entity
		// [in] texture: Shared_ptr to Texture
		// [in, optional] vboSize: Size of the Vertex Buffer in Bytes
		i32 AddStaticEntity(u32 entityId, std::shared_ptr<Texture> texture);

		// Adds an Entity that changes its Vertex Data(Used for Spritesheets)
		// Returns the Index to the Sprite Batch it is stored in
		i32 AddDynamicEntity(u32 entityId, SpriteSheet& spriteSheet);

		// Returns a Pointer to the Static Sprite Batch
		SpriteBatch* GetSpriteBatch(u32 index);

		// Returns a Pointer to the Spritesheet Batch
		SpriteSheetBatch* GetSpriteSheetBatch(u32 index);

		void Render();

	public:
		Ssbo m_TransformsSsbo;

		// [baseOffset + draw call id] = Entity ID
		UniformBuffer m_EntityIdsUbo;

	private:
		std::shared_ptr<Shader> m_SpriteShader = nullptr;

		// Vector that holds all Static Sprite Batches
		//	-> Dynamic: Data in the Vertex Buffer doesnt change and can be instanced
		std::vector<SpriteBatch*> m_StaticSpriteBatches;

		// Vector that holds all Dynamic Sprite Batches
		//	-> Dynamic: Data in the Vertex Buffer changes frequently
		std::vector<SpriteSheetBatch*> m_DynamicSpriteBatches;
	};
}
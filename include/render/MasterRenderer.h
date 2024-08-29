#pragma once

#include <vector>

#include "resources/Shader.h"
#include "render/SpriteBatch.h"
#include "entity/SpriteEntity.h"
#include "memory/Ssbo.h"

namespace tenshi
{
	// -- SSBO BINDING POINTS
#define SPRITE_TRANSFORMS_SSBO_BIMDING_POINT 0

	class MasterRenderer
	{
	public:
		MasterRenderer();

		void AddSpriteEntity(SpriteEntity& entity);

		void Render();

	public:
		Ssbo* m_TransformsSsbo = nullptr;

	private:
		std::shared_ptr<Shader> m_SpriteShader = nullptr;

		// Vector that holds all Sprite Batches
		// Sprite Batches are only used for Sprites that are not Batched
		std::vector<SpriteBatch*> m_SpriteBatches;

		// std::vector<SpriteSheetBatch*> m_SpriteSheetBatches;
	};
}
#pragma once

#include <memory>
#include <vector>
#include <type_traits>

#include "resources/Shader.h"
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

		void Render();

	public:
		Ssbo m_TransformsSsbo;

		// [baseOffset + draw call id] = Entity ID
		UniformBuffer m_EntityIdsUbo;

	private:
		std::shared_ptr<Shader> m_SpriteShader = nullptr;
	};
}
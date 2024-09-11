#pragma once

#include <vector>

#include "ecs/System.h"
#include "resources/Shader.h"
#include "render/SpriteBatch.h"

namespace tenshi
{
	class SpriteRenderer : System
	{
	public:
		SpriteRenderer(Signature signature, u8 id);
		~SpriteRenderer();

		void Render(Shader& shader);

	private:
		std::vector<SpriteBatch*> m_Batches;
	};
}
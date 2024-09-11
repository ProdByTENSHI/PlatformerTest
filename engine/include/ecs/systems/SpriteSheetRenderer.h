#pragma once

#include <vector>

#include "ecs/System.h"
#include "resources/Shader.h"
#include "render/SpriteSheetBatch.h"

namespace tenshi
{
	class SpriteSheetRenderer : public System
	{
	public:
		SpriteSheetRenderer(Signature signature, u8 id);

		void Render(Shader& shader);

	private:
		std::vector<SpriteSheetBatch*> m_Batches;

	};
}
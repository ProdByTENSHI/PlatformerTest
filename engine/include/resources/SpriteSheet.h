#pragma once

#include <memory>
#include <vector>
#include <string>

#include "resources/Texture.h"
#include "resources/Vertex.h"

namespace tenshi
{
	class SpriteSheet
	{
	public:
		// [in] texture: Shared Pointer to the Spritesheet Texture
		// [in] frameWidth: Width in Pixels per Frame
		// [in] frameHeight: Height in Pixels per Frame
		SpriteSheet(std::shared_ptr<Texture> texture, const std::string& name, u16 frameWidth, u16 frameHeight);

		// Sets the Texture Coords of the given Vertice Vector to apply to the Frame
		// [in] index: Index into the Spritesheet (0 to FRAME_COUNT - 1)
		// [out] vertices: Vector that holds the Vertices
		void SetTextureCoordsOfFrame(u32 index, std::vector<Vertex>& vertices);

		// Texture of the Spritesheet
		std::shared_ptr<Texture> m_Texture = nullptr;

		const u16 FRAME_WIDTH = 0;
		const u16 FRAME_HEIGHT = 0;
		const i16 H_FRAMES = 0;
		const i16 V_FRAMES = 0;
		const u32 FRAME_COUNT = 0;
		const std::string NAME;
	};
}
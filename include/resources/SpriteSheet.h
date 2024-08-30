#pragma once

#include <memory>
#include <vector>

#include "resources/Texture.h"
#include "tenshiUtil/math/Rect2D.h"
#include "resources/Vertex.h"

namespace tenshi
{
	class SpriteSheet
	{
	public:
		// [in] texture: Shared Pointer to the Spritesheet Texture
		// [in] frameWidth: Width in Pixels per Frame
		// [in] frameHeight: Height in Pixels per Frame
		// [in] hFrameCount: Number of Frames on the Horizontal(0 to m_HFrames - 1)
		// [in] vFrameCount: Number of Frames on the Vertical(0 to m_VFrames - 1)
		SpriteSheet(std::shared_ptr<Texture> texture, u16 frameWidth, u16 frameHeight,
			u16 hFrameCount, u16 vFrameCount);

		// Sets the Texture Coords of the given Vertice Vector to apply to the Frame
		// [in] hIndex: Index on the Horizontal into the Spritesheet
		// [in] hIndex: Index on the Vertical into the Spritesheet
		// [out] u: U Texture Coor
		// [out] V: V Texture Coor
		void SetTextureCoordsOfFrame(i32 hIndex, i32 vIndex, f32& u, f32& v);

		// Texture of the Spritesheet
		std::shared_ptr<Texture> m_Texture = nullptr;

		const i16 m_HFrames = 0;
		const i16 m_VFrames = 0;
	};
}
#include "resources/SpriteSheet.h"

#include <iostream>

namespace tenshi
{
	SpriteSheet::SpriteSheet(std::shared_ptr<Texture> texture, u16 frameWidth, u16 frameHeight,
		u16 hFrameCount, u16 vFrameCount)
		: m_Texture(texture), m_HFrames(hFrameCount), m_VFrames(vFrameCount)
	{

	}

	void SpriteSheet::SetTextureCoordsOfFrame(i32 hIndex, i32 vIndex, f32& u, f32& v)
	{
		u /= m_HFrames;
		v /= m_VFrames;

		u += hIndex;
		v += vIndex;
	}
}
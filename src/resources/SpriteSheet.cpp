#include "resources/SpriteSheet.h"

#include <iostream>

namespace tenshi
{
	SpriteSheet::SpriteSheet(std::shared_ptr<Texture> texture, u16 frameWidth, u16 frameHeight,
		u16 hFrameCount, u16 vFrameCount)
		: m_Texture(texture), m_HFrames(hFrameCount), m_VFrames(vFrameCount)
	{

	}

	void SpriteSheet::GetTextureCoordsOfFrame(i32 hIndex, i32 vIndex, std::vector<Vertex>& vertices)
	{
		if (vertices.size() <= 0)
		{
			std::cout << "[SPRITE SHEET]: Cannot set Texture Coords for empty Vertice Vector" << std::endl;
			return;
		}

		for (auto& vertex : vertices)
		{
			std::cout << "Texture Coords: " << vertex.m_TexCoords[0] << " " << vertex.m_TexCoords[1] << std::endl;
		}
	}
}
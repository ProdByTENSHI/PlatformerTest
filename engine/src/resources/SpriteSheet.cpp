#include "resources/SpriteSheet.h"

#include <iostream>

namespace tenshi
{
	SpriteSheet::SpriteSheet(std::shared_ptr<Texture> texture, const std::string& name, u16 frameWidth, u16 frameHeight)
		: m_Texture(texture), H_FRAMES(m_Texture->GetWidth() / frameWidth), V_FRAMES(m_Texture->GetHeight() / frameHeight),
		FRAME_WIDTH(frameWidth), FRAME_HEIGHT(frameHeight), FRAME_COUNT(H_FRAMES* V_FRAMES), NAME(name)
	{
	}

	void SpriteSheet::SetTextureCoordsOfFrame(u32 index, std::vector<Vertex>& vertices)
	{
		if (index >= FRAME_COUNT)
		{
			std::cout << "[SPRITESHEET] Index out of Bounds. Must be between 0 and " << FRAME_COUNT - 1 << std::endl;
			return;
		}

		if (vertices.size() != 6)
		{
			std::cout << "[SPRITESHEET] Cannot set Texture Coords because vertices Vector needs to be 6 Elements" << std::endl;
			return;
		}

		u32 _x = 0, _y = 0;
		_x = (index % H_FRAMES) * (f32)FRAME_WIDTH;
		_y = (index / H_FRAMES) * (f32)FRAME_HEIGHT;

		// Top Left
		vertices[2].m_TexCoords[0] = _x / (f32)m_Texture->GetWidth();
		vertices[2].m_TexCoords[1] = _y / (f32)m_Texture->GetHeight();
		vertices[5].m_TexCoords[0] = vertices[2].m_TexCoords[0];
		vertices[5].m_TexCoords[1] = vertices[2].m_TexCoords[1];

		// Top Right
		vertices[0].m_TexCoords[0] = (_x + (f32)FRAME_WIDTH) / (f32)m_Texture->GetWidth();
		vertices[0].m_TexCoords[1] = _y / (f32)m_Texture->GetHeight();

		// Bottom Right
		vertices[1].m_TexCoords[0] = (_x + (f32)FRAME_WIDTH) / (f32)m_Texture->GetWidth();
		vertices[1].m_TexCoords[1] = (_y + (f32)FRAME_HEIGHT) / (f32)m_Texture->GetHeight();
		vertices[3].m_TexCoords[0] = vertices[1].m_TexCoords[0];
		vertices[3].m_TexCoords[1] = vertices[1].m_TexCoords[1];

		// Bottom Left
		vertices[4].m_TexCoords[0] = _x / (f32)m_Texture->GetWidth();
		vertices[4].m_TexCoords[1] = (_y + (f32)FRAME_HEIGHT) / (f32)m_Texture->GetHeight();
	}
}
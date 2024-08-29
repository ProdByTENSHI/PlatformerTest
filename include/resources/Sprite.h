#pragma once

#include <vector>
#include <memory>

#include "resources/Vertex.h"
#include "resources/Texture.h"
#include "tenshiUtil/math/Transform2D.h"
#include "tenshiUtil/math/Rect2D.h"

namespace tenshi
{
	struct Sprite
	{
		Sprite(std::shared_ptr<Texture> texture) : m_Texture(texture) {}

		// For Sprite Sheets -> Can be Pos 0 and size of Texture for entire Texture
		Rect2D m_SrcRect;

		Transform2D m_Transform;
		const std::shared_ptr<Texture> m_Texture;
	};
}
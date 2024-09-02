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

		Transform2D m_Transform;
		std::shared_ptr<Texture> m_Texture;
	};
}
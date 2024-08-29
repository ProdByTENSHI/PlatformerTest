#pragma once

#include "tenshiUtil/Types.h"
#include "tenshiUtil/math/Transform2D.h"

namespace tenshi
{
	inline constexpr u32 INVALID_ENTITY = 0xFFFFFFFF;

	struct Entity
	{
		Entity(u32 id) : m_EntityId(id) {}
		virtual ~Entity() {}

		const u32 m_EntityId = INVALID_ENTITY;

		Transform2D m_Transform;
	};
}
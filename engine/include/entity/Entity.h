#pragma once

#include <nlohmann/json.hpp>

#include <iostream>

#include "tenshiUtil/Types.h"
#include "tenshiUtil/math/Transform2D.h"

using json = nlohmann::json;

namespace tenshi
{
	inline constexpr u32 INVALID_ENTITY = 0xFFFFFFFF;

	struct Entity
	{
		Entity(u32 id) : m_EntityId(id) {}

		virtual ~Entity();

		// Serialize Data into JSON Format and returns the serialized Json Data
		// Entity Base Class serializes the Name, id and the Transform
		virtual json Serialize();

		// Deserializes the Input and overwrites the current Entity Data
		// Entity Base Class deserializes the Name, id, and the Transform
		virtual void Deserialize(json& data);

		const char* m_EntityName = "INVALID_ENTITY";
		const u32 m_EntityId = INVALID_ENTITY;

		Transform2D m_Transform;
	};
}
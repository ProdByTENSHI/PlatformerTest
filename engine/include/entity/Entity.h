#pragma once

#include <nlohmann/json.hpp>
#include <string>

#include "tenshiUtil/Types.h"
#include "tenshiUtil/math/Transform2D.h"

using json = nlohmann::json;

namespace tenshi
{
	inline constexpr u32 INVALID_ENTITY = 0xFFFFFFFF;

	namespace EntityType
	{
		enum EntityType
		{
			Invalid = BIT(0),
			SpriteEntity = BIT(1),
			SpriteSheetEntity = BIT(2),
			AnimatedEntity = BIT(3)
		};
	}

	struct Entity
	{
		Entity(u32 id);

		virtual ~Entity();

		// Returns the Type of the Entity
		// Returns Invalid if its the base Class
		virtual EntityType::EntityType GetType();

		// Serialize Data into JSON Format and returns the serialized Json Data
		// Entity Base Class serializes the Name, id and the Transform
		virtual json Serialize();

		// Deserializes the Input and overwrites the current Entity Data
		// Entity Base Class deserializes the Name, and the Transform
		virtual void Deserialize(const json& data);

		std::string m_EntityName = "INVALID_ENTITY";
		const u32 m_EntityId = INVALID_ENTITY;

		Transform2D m_Transform;
	};
}
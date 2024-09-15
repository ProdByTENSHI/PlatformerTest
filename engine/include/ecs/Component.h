#pragma once

#include <nlohmann/json.hpp>

#include "ecs/ComponentTypes.h"
#include "tenshiUtil/Types.h"

namespace tenshi
{
	using json = nlohmann::json;

	// Base Class for all ECS Components
	struct Component
	{
		virtual ~Component() {}

		virtual json Serialize() = 0;
		virtual void Deserialize(const json& data) = 0;
		virtual ComponentType GetType() = 0;
	};
}
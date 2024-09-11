#pragma once

#include <memory>

#include "ecs/Component.h"
#include "resources/Texture.h"

namespace tenshi
{
	class SpriteComponent : public virtual Component
	{
	public:
		SpriteComponent(std::shared_ptr<Texture> texture);

		json Serialize() override;
		void Deserialize(const json& data) override;

	public:
		const ComponentType m_Type = ComponentType::SpriteType;
		const std::shared_ptr<Texture> m_Texture = nullptr;
	};
}
#include "ecs/components/SpriteComponent.h"

namespace tenshi
{
	SpriteComponent::SpriteComponent(std::shared_ptr<Texture> texture)
		: m_Texture(texture)
	{
	}

	json SpriteComponent::Serialize()
	{
		json _data;
		_data["texture"] = m_Texture->m_Name;

		return _data;
	}

	void SpriteComponent::Deserialize(const json& data)
	{

	}

	ComponentType SpriteComponent::GetType()
	{
		return ComponentType::SpriteType;
	}
}
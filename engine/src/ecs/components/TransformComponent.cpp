#include "ecs/components/TransformComponent.h"

namespace tenshi
{
	TransformComponent::TransformComponent()
	{
		Scale(1.0f);
	}

	TransformComponent::TransformComponent(const glm::vec2& pos, const glm::vec2& rot,
		const glm::vec2& scale)
	{
		Scale(scale);
		Rotate(glm::vec2(1.0f, 0.0f), rot.x);
		Rotate(glm::vec2(0.0f, 1.0f), rot.y);
		Translate(pos);
	}

	json TransformComponent::Serialize()
	{
		json _data;
		_data["position"] = { m_Position.x, m_Position.y };
		_data["rotation"] = { m_Rotation.x, m_Rotation.y };
		_data["scale"] = { m_Scale.x, m_Scale.y };
		return _data;
	}

	// TODO: Implement
	void TransformComponent::Deserialize(const json& data)
	{
	}

	ComponentType TransformComponent::GetType()
	{
		return ComponentType::TransformType;
	}
}
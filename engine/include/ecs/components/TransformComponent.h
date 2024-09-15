#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "ecs/Component.h"
#include "tenshiUtil/eventsystem/EventSystem.h"

namespace tenshi
{
	struct TransformComponent : public virtual Component
	{
		TransformComponent();
		TransformComponent(const glm::vec2& pos, const glm::vec2& rot, const glm::vec2& scale);

		json Serialize() override;
		void Deserialize(const json& data) override;
		ComponentType GetType() override;

		void Translate(const glm::vec2& translation)
		{
			m_Position += translation;
			m_ModelMatrix = glm::translate(m_ModelMatrix, glm::vec3(translation.x, translation.y, 0.0f));

			onTransformChange.Dispatch(m_ModelMatrix, m_Position, m_Rotation, m_Scale);
		}

		void Rotate(const glm::vec2& axis, f32 degrees)
		{
			m_Rotation += axis * degrees;
			m_ModelMatrix = glm::rotate(m_ModelMatrix, degrees, glm::vec3(axis.x, axis.y, 0.0f));

			onTransformChange.Dispatch(m_ModelMatrix, m_Position, m_Rotation, m_Scale);
		}

		void Scale(const glm::vec2& scale)
		{
			m_Scale = scale;
			m_ModelMatrix = glm::scale(m_ModelMatrix, glm::vec3(scale.x, scale.y, 0.0f));

			onTransformChange.Dispatch(m_ModelMatrix, m_Position, m_Rotation, m_Scale);
		}

		void Scale(f32 scalar)
		{
			Scale(glm::vec2(scalar, scalar));
		}

		// -- Events
		// [Args]: Model Matrix, Position, Rotation, Scale
		Event<glm::mat4, glm::vec2, glm::vec2, glm::vec2> onTransformChange;

		glm::mat4 m_ModelMatrix = glm::mat4(1.0f);
		glm::vec2 m_Position = glm::vec2(0.0f);
		glm::vec2 m_Rotation = glm::vec2(0.0f);
		glm::vec2 m_Scale = glm::vec2(1.0f);

		const ComponentType m_Type = ComponentType::TransformType;
	};
}
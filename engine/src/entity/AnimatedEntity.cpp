#include "entity/AnimatedEntity.h"

#include <iostream>

#include "core/Globals.h"
#include "tenshiUtil/math/Random.h"

namespace tenshi
{
	AnimatedEntity::AnimatedEntity(u32 id, std::shared_ptr<SpriteSheet> spriteSheet, const std::string& animDataFile)
		: Entity(id), SpriteSheetEntity(id, spriteSheet), m_AnimDataFile(animDataFile),
		m_OnUpdate([this]()
			{
				if (m_FrameCounter >= m_CurrentAnimation->FRAMES_PER_SECOND)
				{
					if (m_CurrentFrame >= m_CurrentAnimation->END_FRAME)
					{
						m_CurrentFrame = m_CurrentAnimation->START_FRAME;
						OnFrameChange.Dispatch(m_CurrentAnimation->END_FRAME, m_CurrentAnimation->START_FRAME);
					}
					else
					{
						++m_CurrentFrame;
						OnFrameChange.Dispatch(m_CurrentFrame - 1, m_CurrentFrame);
					}

					m_FrameCounter = 0.0f;
					return;
				}

				m_FrameCounter += g_DeltaTime;
			})
	{
		m_Animations.insert(std::make_pair("Default", g_ResourceManager->GetAnimation(m_AnimDataFile, "Default")));
		m_CurrentAnimation = m_Animations["Default"];

		EventHandler<i32> _onKeyDown([this](i32 key)
			{
				if (key == GLFW_KEY_B)
				{
					SetActiveAnimation("Default");
				}
				else if (key == GLFW_KEY_N)
				{
					SetActiveAnimation("Walk");
				}
			});
		g_InputManager->OnKeyDown.Subscribe(_onKeyDown);

		g_OnUpdate.Subscribe(m_OnUpdate);
	}

	AnimatedEntity::~AnimatedEntity()
	{
		g_OnUpdate.Unsubscribe(m_OnUpdate);
	}

	EntityType::EntityType AnimatedEntity::GetType()
	{
		return EntityType::AnimatedEntity;
	}

	json AnimatedEntity::Serialize()
	{
		json _data = SpriteSheetEntity::Serialize();

		// Serialize the Sprite Sheet Animations
		for (auto& anim : m_Animations)
		{
			_data["animationData"] = m_AnimDataFile;
			_data["animations"].push_back(anim.first);
		}

		return _data;
	}

	void AnimatedEntity::Deserialize(const json& data)
	{
		SpriteSheetEntity::Deserialize(data);

		std::string _animData = data["animationData"];
		for (auto& anim : data["animations"].items())
		{
			AddAnimation(g_ResourceManager->GetAnimation(_animData, anim.value()));
		}
	}

	void AnimatedEntity::AddAnimation(std::shared_ptr<SpriteSheetAnimation> anim)
	{
		m_Animations.insert(std::make_pair(anim->m_NAME, anim));
	}

	std::shared_ptr<SpriteSheetAnimation> AnimatedEntity::GetAnimation(const std::string& name)
	{
		return m_Animations[name];
	}

	bool AnimatedEntity::DoesAnimationExist(const std::string& name)
	{
		return m_Animations.find(name) != m_Animations.end();
	}

	void AnimatedEntity::SetActiveAnimation(const std::string& name)
	{
		if (!DoesAnimationExist(name))
		{
			std::cout << "[ANIMATED ENTITY] Couldnt set Animation " << name << " because it doesnt exist" << std::endl;
			return;
		}

		if (m_CurrentAnimation->m_NAME == name)
			return;

		i32 _lastFrame = m_CurrentFrame;
		m_CurrentAnimation = m_Animations[name];
		m_CurrentFrame = m_CurrentAnimation->START_FRAME;
		m_FrameCounter = 0.0f;
		OnFrameChange.Dispatch(_lastFrame, m_CurrentFrame);

		std::cout << "[ANIMATED ENTITY] Set Animation to " << m_CurrentAnimation->m_NAME << std::endl;
	}
}
#pragma once

#include <string>
#include <map>
#include <memory>

#include "anim/SpriteSheetAnimation.h"
#include "entity/SpriteSheetEntity.h"

namespace tenshi
{
	class AnimatedEntity : virtual public SpriteSheetEntity
	{
	public:
		AnimatedEntity(u32 id, std::shared_ptr<SpriteSheet> spriteSheet,
			const std::string& animDataFile);
		~AnimatedEntity();

		void AddAnimation(std::shared_ptr<SpriteSheetAnimation> anim);
		std::shared_ptr<SpriteSheetAnimation> GetAnimation(const std::string& name);

		bool DoesAnimationExist(const std::string& name);
		void SetActiveAnimation(const std::string& name);

		EntityType::EntityType GetType() override;
		json Serialize() override;
		void Deserialize(const json& data) override;

	private:
		// [Key] Animation Name
		// [Value] Pointer to the Animation
		std::map<std::string, std::shared_ptr<SpriteSheetAnimation>> m_Animations;
		std::shared_ptr<SpriteSheetAnimation> m_CurrentAnimation = nullptr;

		std::string m_AnimDataFile;

		f32 m_FrameCounter = 0.0f;
		u32 m_CurrentFrame = 0;

		EventHandler<> m_OnUpdate;
	};
}
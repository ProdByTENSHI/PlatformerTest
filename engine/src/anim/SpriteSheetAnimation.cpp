#include "anim/SpriteSheetAnimation.h"

#include <iostream>

#include "core/Globals.h"

namespace tenshi
{
	SpriteSheetAnimation::SpriteSheetAnimation(std::shared_ptr<SpriteSheet> spriteSheet, const std::string& name,
		u32 startFrame, u32 endFrame, f32 fps)
		: m_SpriteSheet(spriteSheet), FRAMES_PER_SECOND(fps), START_FRAME(startFrame), END_FRAME(endFrame), m_NAME(name)
	{
		if (endFrame >= m_SpriteSheet->FRAME_COUNT)
		{
			std::cout << "[SPRITESHEET ANIMATION] End Frame must be less than the Spritesheet Frame Count";
			return;
		}

		m_CreationStatus = true;
	}

	bool SpriteSheetAnimation::GetCreationStatus()
	{
		return m_CreationStatus;
	}
}
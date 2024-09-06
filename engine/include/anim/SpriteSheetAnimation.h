#pragma once

#include <nlohmann/json.hpp>

#include "resources/SpriteSheet.h"
#include "tenshiUtil/eventsystem/EventSystem.h"

using json = nlohmann::json;

namespace tenshi
{
	class SpriteSheetAnimation
	{
	public:
		SpriteSheetAnimation(std::shared_ptr<SpriteSheet> spriteSheet, const std::string& name,
			u32 startFrame, u32 endFrame, f32 fps);

		bool GetCreationStatus();

	public:
		const u32 START_FRAME = 0;
		const u32 END_FRAME = 0;
		const f32 FRAMES_PER_SECOND = 0;
		const std::string m_NAME;

	private:
		std::shared_ptr<SpriteSheet> m_SpriteSheet = nullptr;
		bool m_CreationStatus = false;
	};
}
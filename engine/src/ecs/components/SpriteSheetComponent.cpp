#include "ecs/components/SpriteSheetComponent.h"

#include "core/Globals.h"

namespace tenshi
{
	SpriteSheetComponent::SpriteSheetComponent() {}

	SpriteSheetComponent::SpriteSheetComponent(std::shared_ptr <SpriteSheet> spriteSheet)
		: m_SpriteSheet(spriteSheet)
	{
	}

	json SpriteSheetComponent::Serialize()
	{
		json _data;
		_data["spriteSheet"] = m_SpriteSheet->NAME;

		return _data;
	}

	void SpriteSheetComponent::Deserialize(const json& data)
	{
		m_SpriteSheet = g_ResourceManager->GetSpriteSheet(data["spriteSheet"]);
	}

	void SpriteSheetComponent::SetFrame(u32 frame)
	{
		u32 _oldFrame = m_CurrentFrame;
		m_CurrentFrame = frame;
		m_OnFrameChange.Dispatch(_oldFrame, m_CurrentFrame);
	}

	ComponentType SpriteSheetComponent::GetType()
	{
		return ComponentType::SpriteSheetType;
	}
}
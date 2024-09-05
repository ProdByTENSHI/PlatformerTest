#pragma once

#include <unordered_map>
#include <memory>
#include <string>

#include "tenshiUtil/Types.h"
#include "resources/Shader.h"
#include "resources/Texture.h"
#include "resources/SpriteSheet.h"

namespace tenshi
{
	inline const std::string SHADER_LOCATION = "res/shader";
	inline const std::string TEXTURE_LOCATION = "res/sprites";
	inline const std::string SPRITE_SHEET_LOCATION = "res/sprites";

	class ResourceManager
	{
	public:
		std::shared_ptr<tenshi::Shader> GetShader(const std::string& name);
		std::shared_ptr<Texture> GetTexture(const std::string& name);
		std::shared_ptr<Texture> GetTexture(u16 id);
		std::shared_ptr<SpriteSheet> GetSpriteSheet(const std::string& name,
			u16 frameWidth, u16 frameHeight);

	private:
		std::unordered_map<std::string, std::shared_ptr<tenshi::Shader>> m_ShaderCache;
		std::unordered_map<std::string, std::shared_ptr<Texture>> m_TextureCache;
		std::unordered_map<std::string, std::shared_ptr<SpriteSheet>> m_SpriteSheetCache;

	};
}
#include "resources/ResourceManager.h"

#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace tenshi
{
	std::shared_ptr<tenshi::Shader> ResourceManager::GetShader(const std::string& name)
	{
		if (m_ShaderCache.find(name) != m_ShaderCache.end())
			return m_ShaderCache[name];

		std::string _loc = SHADER_LOCATION;
		_loc += OS_SEP;
		_loc.append(name);
		std::shared_ptr<tenshi::Shader> _shader = std::make_shared<tenshi::Shader>
			(_loc + ".vert", _loc + ".frag");
		if (_shader == nullptr)
		{
			std::cerr << "Could not load Shader " << name << std::endl;
			return nullptr;
		}

		m_ShaderCache.insert(std::make_pair(name, _shader));
		return _shader;
	}

	std::shared_ptr<Texture> ResourceManager::GetTexture(const std::string& name)
	{
		if (m_TextureCache.find(name) != m_TextureCache.end())
			return m_TextureCache[name];

		std::string _loc = TEXTURE_LOCATION;
		_loc += OS_SEP;
		_loc.append(name);

		std::cout << _loc << std::endl;
		std::shared_ptr<Texture> _texture = std::make_shared<Texture>(_loc);
		if (_texture == nullptr || !_texture->GetCreationStatus())
		{
			std::cerr << "Could not load Texture " << name << std::endl;
			return nullptr;
		}

		_texture->m_Name = name;

		m_TextureCache.insert(std::make_pair(name, _texture));

		std::cout << "[RESOURCEMANAGER] Loaded Texture " << name << std::endl;

		return _texture;
	}

	std::shared_ptr<Texture> ResourceManager::GetTexture(u16 id)
	{
		for (auto& texture : m_TextureCache)
		{
			if (texture.second->m_Id == id)
				return texture.second;
		}

		std::cerr << "No Texture with the ID " << id << " found!" << std::endl;
		return nullptr;
	}

	std::shared_ptr<SpriteSheet> ResourceManager::GetSpriteSheet(const std::string& name)
	{
		if (m_SpriteSheetCache.find(name) != m_SpriteSheetCache.end())
			return m_SpriteSheetCache[name];

		std::string _texture;
		u32 _frameWidth = 0;
		u32 _frameHeight = 0;

		// -- Load Sprite Sheet Data from json File
		std::fstream _stream(SPRITE_SHEET_LOCATION, std::ios::in);
		json _data;
		_stream >> _data;

		bool _wasFound = false;
		for (auto& spriteSheet : _data.items())
		{
			if (spriteSheet.value()["name"] != name)
				continue;

			_texture = spriteSheet.value()["texture"];
			_frameWidth = spriteSheet.value()["frameWidth"];
			_frameHeight = spriteSheet.value()["frameHeight"];

			_wasFound = true;
		}

		if (!_wasFound)
		{
			std::cerr << "[RESOURCEMANAGER] Spritesheet Data for " << name << " could not be found" << std::endl;
			return nullptr;
		}

		std::shared_ptr<SpriteSheet> spriteSheet = std::make_shared<SpriteSheet>(GetTexture(_texture), name, _frameWidth, _frameHeight);
		if (!spriteSheet)
		{
			std::cout << "[RESOURCEMANAGER] Could not load Spritesheet " << name << std::endl;
			return nullptr;
		}

		m_SpriteSheetCache.insert(std::make_pair(name, spriteSheet));
		return spriteSheet;
	}

	std::shared_ptr<SpriteSheetAnimation> ResourceManager::GetAnimation(const std::string& fileName,
		const std::string& animName)
	{
		if (m_AnimCache.find(fileName + OS_SEP + animName) != m_AnimCache.end())
			return m_AnimCache[fileName + OS_SEP + animName];

		std::string _spriteSheet;
		u32 _startFrame = 0;
		u32 _endFrame = 0;
		f32 _fps = 0.0f;

		// -- Load Data from Json File
		std::fstream _stream(ANIM_LOCATION + OS_SEP + fileName);
		if (!_stream.is_open())
		{
			std::cerr << "[RESOURCEMANAGER] Could not load Animation Data for " << animName
				<< " from " << ANIM_LOCATION + OS_SEP + fileName << std::endl;
			return nullptr;
		}

		json _data;
		_stream >> _data;

		bool _wasFound = false;
		for (auto& anim : _data.items())
		{
			if (anim.value()["name"] != animName)
				continue;

			_spriteSheet = anim.value()["spriteSheet"];
			_startFrame = anim.value()["startFrame"];
			_endFrame = anim.value()["endFrame"];
			_fps = anim.value()["framesPerSecond"];

			_wasFound = true;
		}

		if (!_wasFound)
		{
			std::cerr << "No Animation with name " << animName << " in Data File" << std::endl;
			return nullptr;
		}

		std::shared_ptr<SpriteSheetAnimation> anim = std::make_shared<SpriteSheetAnimation>
			(GetSpriteSheet(_spriteSheet), animName, _startFrame, _endFrame, _fps);

		std::cout << "[RESOURCEMANAGER] Loaded Animation " << animName << std::endl;

		m_AnimCache.insert(std::make_pair(fileName + OS_SEP + animName, anim));
		return anim;
	}
}
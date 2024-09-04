#include "resources/ResourceManager.h"

#include <iostream>

namespace tenshi
{
	std::shared_ptr<tenshi::Shader> ResourceManager::GetShader(const std::string& name)
	{
		if (m_ShaderCache.find(name) != m_ShaderCache.end())
			return m_ShaderCache[name];

		std::shared_ptr<tenshi::Shader> _shader = std::make_shared<tenshi::Shader>
			(std::string(SHADER_LOCATION).append("/").append(name).append(".vert"),
				std::string(SHADER_LOCATION).append("/").append(name).append(".frag"));
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

		std::shared_ptr<Texture> _texture = std::make_shared<Texture>
			(std::string(TEXTURE_LOCATION).append("/").append(name));
		if (_texture == nullptr)
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
}
#include "resources/Texture.h"

#include <stb_image.h>
#include <iostream>

namespace tenshi
{
	Texture::Texture(const std::string& name) : m_Id(s_TextureCount), m_Name(name)
	{
		stbi_set_flip_vertically_on_load(0);
		stbi_uc* _buffer = stbi_load(name.c_str(), &m_Width, &m_Height, &m_Channels, 4);
		if (!_buffer)
		{
			std::cerr << "Could not load Texture " << name << std::endl;
			return;
		}

		glCreateTextures(GL_TEXTURE_2D, 1, &m_Texture);

		glTextureParameteri(m_Texture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTextureParameteri(m_Texture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTextureParameteri(m_Texture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(m_Texture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTextureStorage2D(m_Texture, 1, GL_RGBA8, m_Width, m_Height);
		glTextureSubImage2D(m_Texture, 0, 0, 0, m_Width, m_Height, GL_RGBA, GL_UNSIGNED_BYTE, _buffer);

		++s_TextureCount;
	}

	Texture::~Texture()
	{
		glDeleteTextures(1, &m_Texture);
	}

	void Texture::Bind(u8 slot /*= 0*/)
	{
		glBindTextureUnit(slot, m_Texture);
	}

	void Texture::Unbind()
	{
		glBindTextureUnit(0, m_Texture);
	}
}
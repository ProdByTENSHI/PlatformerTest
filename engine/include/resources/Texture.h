#pragma once

#include <GL/glew.h>
#include <string>

#include "tenshiUtil/Types.h"

namespace tenshi
{
	class Texture
	{
	public:
		Texture(const std::string& name);
		~Texture();

		void Bind(u8 slot = 0);
		void Unbind();

		const u16 m_Id = 0;

		inline i32 GetWidth()
		{
			return m_Width;
		}

		inline i32 GetHeight()
		{
			return m_Height;
		}

	private:
		inline static u16 s_TextureCount = 0;

		GLuint m_Texture = 0;
		i32 m_Width = 0;
		i32 m_Height = 0;
		i32 m_Channels = 0;
	};
}
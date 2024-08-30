#pragma once

#include <string>
#include <GL/glew.h>

#include "resources/Shader.h"
#include "tenshiUtil/Types.h"

namespace tenshi
{
	class UniformBuffer
	{
	public:
		~UniformBuffer();

		void Create(GLsizei size, const std::string& name, const void* data = nullptr);

		void BindToShader(Shader& shader, u32 bindingPoint);
		void SubBufferData(GLintptr offset, GLsizei size, const void* data);

	private:
		GLuint m_Buffer = 0;
		std::string m_Name;

	};
}
#pragma once

#include <GL/glew.h>

#include "tenshiUtil/Types.h"
#include "resources/Shader.h"

namespace tenshi
{
	class Ssbo
	{
	public:
		Ssbo(GLsizei size, const std::string& name);
		~Ssbo();

		GLuint GetBufferBindingPoint(tenshi::Shader& shader);
		void BindToShader(tenshi::Shader& shader, u8 index);
		void SubBufferData(GLintptr offset, GLsizei size, const void* data);

	private:
		GLuint m_Buffer = 0;
		const std::string m_Name;

	};
}
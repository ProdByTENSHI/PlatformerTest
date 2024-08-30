#include "Ssbo.h"

namespace tenshi
{
	Ssbo::~Ssbo()
	{
		glDeleteBuffers(1, &m_Buffer);
	}

	void Ssbo::Create(GLsizei size, const std::string& name)
	{
		m_Name = name;

		glCreateBuffers(1, &m_Buffer);
		glNamedBufferStorage(m_Buffer, size, nullptr, GL_DYNAMIC_STORAGE_BIT);
	}

	GLuint Ssbo::GetBufferBindingPoint(tenshi::Shader& shader)
	{
		return glGetProgramResourceIndex(shader.GetProgram(), GL_SHADER_STORAGE_BLOCK,
			m_Name.c_str());
	}

	void Ssbo::BindToShader(tenshi::Shader& shader, u8 index)
	{
		GLuint _bindingPoint = GetBufferBindingPoint(shader);
		glShaderStorageBlockBinding(shader.GetProgram(), _bindingPoint, index);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index, m_Buffer);
	}

	void Ssbo::SubBufferData(GLintptr offset, GLsizei size, const void* data)
	{
		glNamedBufferSubData(m_Buffer, offset, size, data);
	}
}
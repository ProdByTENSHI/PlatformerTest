#include "memory/Ssbo.h"

namespace tenshi
{
	Ssbo::Ssbo(GLsizei size, const std::string& name)
		: m_Name(name)
	{
		glCreateBuffers(1, &m_Buffer);
		glNamedBufferStorage(m_Buffer, size, nullptr, GL_DYNAMIC_STORAGE_BIT);
	}

	Ssbo::~Ssbo()
	{
		glDeleteBuffers(1, &m_Buffer);
	}

	u8 Ssbo::GetBufferBindingPoint(tenshi::Shader& shader)
	{
		return glGetProgramResourceIndex(shader.GetProgram(), GL_SHADER_STORAGE_BLOCK,
			m_Name.c_str());
	}

	void Ssbo::BindToShader(tenshi::Shader& shader, u8 index)
	{
		glShaderStorageBlockBinding(shader.GetProgram(), GetBufferBindingPoint(shader), index);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index, m_Buffer);
	}

	void Ssbo::SubBufferData(GLintptr offset, GLsizei size, const void* data)
	{
		glNamedBufferSubData(m_Buffer, offset, size, data);
	}
}
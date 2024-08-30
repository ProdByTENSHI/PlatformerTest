#include "UniformBuffer.h"

#include <iostream>

namespace tenshi
{
	UniformBuffer::~UniformBuffer()
	{
		glDeleteBuffers(1, &m_Buffer);
	}

	void UniformBuffer::Create(GLsizei size, const std::string& name, const void* data /*= nullptr*/)
	{
		glCreateBuffers(1, &m_Buffer);
		glNamedBufferStorage(m_Buffer, size, data, GL_DYNAMIC_STORAGE_BIT);

		m_Name = name;
	}

	void UniformBuffer::BindToShader(Shader& shader, u32 bindingPoint)
	{
		u32 _blockIndex = glGetUniformBlockIndex(shader.GetProgram(), m_Name.c_str());
		if (_blockIndex == GL_INVALID_INDEX)
		{
			std::cerr << "[UNIFORM BUFFER] No Uniform Buffer with the Name " << m_Name << " was found" << std::endl;
			return;
		}

		glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, m_Buffer);
		glUniformBlockBinding(shader.GetProgram(), _blockIndex, bindingPoint);
	}

	void UniformBuffer::SubBufferData(GLintptr offset, GLsizei size, const void* data)
	{
		glNamedBufferSubData(m_Buffer, offset, size, data);
	}
}
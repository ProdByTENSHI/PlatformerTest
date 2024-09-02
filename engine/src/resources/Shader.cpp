#include "resources/Shader.h"

#include <iostream>
#include <fstream>

#include <glm/gtc/type_ptr.hpp>

namespace tenshi
{
	Shader::Shader() {}

	Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
		: m_Program(0), m_VertexShader(0), m_FragmentShader(0)
	{
		std::string vertexSource = LoadShader(vertexPath);
		std::string fragmentSource = LoadShader(fragmentPath);

		Create(vertexSource, fragmentSource);

		//Logger::getInstance()->write("Loaded Shader " + vertexPath + " : " + fragmentPath);
		std::cout << "Loaded Shader " << vertexPath << " : " << fragmentPath << std::endl;
	}

	Shader::~Shader()
	{
		glDeleteShader(m_VertexShader);
		glDeleteShader(m_FragmentShader);
		glDeleteProgram(m_Program);
	}

	GLuint Shader::CompileShader(GLuint type, const std::string& source)
	{
		GLuint id = glCreateShader(type);
		const GLchar* src = source.c_str();

		glShaderSource(id, 1, &src, NULL);
		glCompileShader(id);

		// ERROR HANDLING
		GLint hasCompiled = 0;
		glGetShaderiv(id, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE)
		{
			// Type to String to get better Debug Info
			std::string typeStr = "";
			if (type == GL_VERTEX_SHADER)
				typeStr = "Vertex Shader";
			else if (type == GL_FRAGMENT_SHADER)
				typeStr = "Fragment Shader";

			// Get Log Length in Bytes
			GLint logSize = 0;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logSize);

			// Get Log Message
			GLchar errorMessage[1024];
			glGetShaderInfoLog(id, logSize, &logSize, errorMessage);

			//Logger::getInstance()->write(typeStr + " : " + errorMessage);
			std::cout << "Shader Error: " << errorMessage << std::endl;

			glDeleteShader(id);
			return INVALID_SHADER;
		}

		return id;
	}

	void Shader::Create(const std::string& vertexSource, const std::string& fragmentSource)
	{
		m_Program = glCreateProgram();
		m_VertexShader = CompileShader(GL_VERTEX_SHADER, vertexSource);
		m_FragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentSource);

		glAttachShader(m_Program, m_VertexShader);
		glAttachShader(m_Program, m_FragmentShader);
		glLinkProgram(m_Program);
		glValidateProgram(m_Program);
	}

	void Shader::Bind()
	{
		glUseProgram(m_Program);
	}

	void Shader::Unbind()
	{
		glUseProgram(0);
	}

	const std::string Shader::LoadShader(const std::string& path)
	{
		std::string content;
		std::ifstream stream(path, std::ios::in);

		if (!stream.is_open())
		{
			std::cerr << "Could not load File " << path << std::endl;
			return "";
		}

		std::string line = "";
		while (!stream.eof())
		{
			std::getline(stream, line);
			content.append(line + "\n");
		}

		stream.close();
		return content;
	}

	void Shader::SetUniform1i(const std::string& name, int value)
	{
		glUniform1i(GetUniformLocation(name), value);
	}

	void Shader::SetUniform1f(const std::string& name, float value)
	{
		glUniform1f(GetUniformLocation(name), value);
	}

	void Shader::SetUniform3f(const std::string& name, const glm::vec3& vec)
	{
		glUniform3f(GetUniformLocation(name), vec.x, vec.y, vec.z);
	}

	void Shader::SetUniform4f(const std::string& name, const glm::vec4& vec)
	{
		glUniform4f(GetUniformLocation(name), vec.x, vec.y, vec.z, vec.w);
	}

	void Shader::SetUniformMat4(const std::string& name, const glm::mat4& mat)
	{
		glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat));
	}

	GLint Shader::GetUniformLocation(const std::string& name)
	{
		if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
			return m_UniformLocationCache[name];

		GLint _location = glGetUniformLocation(m_Program, name.c_str());
		if (_location == -1)
		{
			std::cerr << "WARNING: Uniform '" << name << "' does not exist" << std::endl;
			GLint count;
			glGetProgramiv(m_Program, GL_ACTIVE_UNIFORMS, &count);
			for (int i = 0; i < count; i++)
			{
				char name[100];
				glGetActiveUniformName(m_Program, i, sizeof(name), (GLsizei*)NULL, name);
				std::cout << "Active Uniforms: " << name << std::endl;
			}
		}
		else
			m_UniformLocationCache.insert(std::make_pair(name, _location));

		return _location;
	}

	GLint Shader::GetSsboIndex(const std::string& name)
	{
		if (m_SsboLocationCache.find(name) != m_SsboLocationCache.end())
			return m_SsboLocationCache[name];

		GLint _location = glGetProgramResourceIndex(m_Program, GL_SHADER_STORAGE_BLOCK,
			name.c_str());
		if (_location == -1)
		{
			std::cerr << "WARNING: Ssbo '" << name << "' does not exist" << std::endl;
			GLint count;
			glGetProgramiv(m_Program, GL_ACTIVE_RESOURCES, &count);
			for (int i = 0; i < count; i++)
			{
				char name[100];
				glGetActiveUniformName(m_Program, i, sizeof(name), (GLsizei*)NULL, name);
				std::cout << "Active Ssbos: " << name << std::endl;
			}
		}

		m_SsboLocationCache.insert(std::make_pair(name, _location));
		return _location;
	}
}
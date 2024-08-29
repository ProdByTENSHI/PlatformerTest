#pragma once

#include <unordered_map>
#include <string>

#include <gl/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace tenshi
{
	constexpr unsigned int INVALID_SHADER = 0xFFFFF;

	class Shader
	{
	public:
		Shader();
		// Create a Shader from external Shader Source Files
		Shader(const std::string& vertexPath, const std::string& fragmentPath);
		~Shader();

		void Bind();
		void Unbind();

		// Set Uniforms
		GLint GetUniformLocation(const std::string& name);
		GLint GetSsboIndex(const std::string& name);

		void SetUniform1i(const std::string& name, int value);
		void SetUniform1f(const std::string& name, float value);
		void SetUniform3f(const std::string& name, const glm::vec3& vec);
		void SetUniform4f(const std::string& name, const glm::vec4& vec);
		void SetUniformMat4(const std::string& name, const glm::mat4& mat);

		inline GLuint GetProgram() { return m_Program; }

	private:
		// Create Shader from Vertex Shader Source and Fragment Shader Source
		void Create(const std::string& vertexSource, const std::string& fragmentSource);

		// Load a Shader Source File and return it's source code
		const std::string LoadShader(const std::string& path);

		GLuint CompileShader(GLuint type, const std::string& source);

		GLuint m_Program = 0;
		GLuint m_VertexShader = 0;
		GLuint m_FragmentShader = 0;

		std::unordered_map<std::string, GLint> m_UniformLocationCache;
		std::unordered_map<std::string, GLint> m_SsboLocationCache;
	};
}
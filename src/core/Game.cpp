#include "core/Game.h"

#include <GL/glew.h>
#include <iostream>

#include "core/Globals.h"
#include "resources/SpriteSheet.h"

namespace tenshi
{
	void glewMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
		GLsizei length, GLchar const* message, void const* user_param)
	{
		auto const src_str = [source]()
			{
				switch (source)
				{
				case GL_DEBUG_SOURCE_API: return "API";
				case GL_DEBUG_SOURCE_WINDOW_SYSTEM: return "WINDOW SYSTEM";
				case GL_DEBUG_SOURCE_SHADER_COMPILER: return "SHADER COMPILER";
				case GL_DEBUG_SOURCE_THIRD_PARTY: return "THIRD PARTY";
				case GL_DEBUG_SOURCE_APPLICATION: return "APPLICATION";
				case GL_DEBUG_SOURCE_OTHER: return "OTHER";
				}
			}();

			auto const type_str = [type]()
				{
					switch (type)
					{
					case GL_DEBUG_TYPE_ERROR: return "ERROR";
					case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "DEPRECATED_BEHAVIOR";
					case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: return "UNDEFINED_BEHAVIOR";
					case GL_DEBUG_TYPE_PORTABILITY: return "PORTABILITY";
					case GL_DEBUG_TYPE_PERFORMANCE: return "PERFORMANCE";
					case GL_DEBUG_TYPE_MARKER: return "MARKER";
					case GL_DEBUG_TYPE_OTHER: return "OTHER";
					}
				}();

				auto const severity_str = [severity]()
					{
						switch (severity)
						{
						case GL_DEBUG_SEVERITY_NOTIFICATION: return "NOTIFICATION";
						case GL_DEBUG_SEVERITY_LOW: return "LOW";
						case GL_DEBUG_SEVERITY_MEDIUM: return "MEDIUM";
						case GL_DEBUG_SEVERITY_HIGH: return "HIGH";
						}
					}();
					std::cout << src_str << ", " << type_str << ", " << severity_str << ", " << id << ": " << message << '\n';
	}

	Game::Game()
	{
		if (glfwInit() < 0)
		{
			const char* _desc;
			glfwGetError(&_desc);
			std::cout << "Could not initialize GLFW. Error: " << _desc << std::endl;
			return;
		}

		glfwSetErrorCallback([](int code, const char* description)
			{
				std::cerr << "GLFW Error " << code << ": " << description << std::endl;
			});


		glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		g_Window = glfwCreateWindow(g_WindowWidth, g_WindowHeight, "Super Duper Platformer", nullptr, nullptr);
		glfwMakeContextCurrent(g_Window);
		glfwSwapInterval(1);

		GLenum _glInit = glewInit();
		if (_glInit != GLEW_OK)
		{
			std::cerr << "Could not initialize GLEW. Error: " << glewGetErrorString(_glInit);
			return;
		}

		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(glewMessageCallback, nullptr);

		glfwSetWindowSizeCallback(g_Window, [](GLFWwindow* window, i32 width, i32 height)
			{
				g_WindowWidth = width;
				g_WindowHeight = height;
				glViewport(0, 0, width, height);
				g_OnWindowResize.Dispatch(width, height);
			});

		glViewport(0, 0, g_WindowWidth, g_WindowHeight);

		// -- Init Systemss
		g_ResourceManager = std::make_unique<ResourceManager>();
		g_MasterRenderer = std::make_unique<MasterRenderer>();
		g_EntityManager = std::make_unique<EntityManager>();
		g_Camera = std::make_unique<Camera>();

		SpriteEntity* _entity = &g_EntityManager->CreateEntity
		(g_ResourceManager->GetTexture("Wood.png"));
		g_MasterRenderer->AddSpriteEntity(*_entity);

		m_InitStatus = true;
	}

	Game::~Game()
	{
		glfwDestroyWindow(g_Window);
		glfwTerminate();
	}

	void Game::Update()
	{
		f32 _lastFrameTime = 0.0f;
		f32 _currentFrameTime = 0.0f;

		while (!glfwWindowShouldClose(g_Window) && m_InitStatus)
		{
			_currentFrameTime = glfwGetTime();
			g_DeltaTime = _currentFrameTime - _lastFrameTime;
			_lastFrameTime = _currentFrameTime;

			glfwPollEvents();
			glClearColor(0.2f, 0.2f, 0.5f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			g_MasterRenderer->Render();

			glfwSwapBuffers(g_Window);
		}
	}
}
#include "core/Game.h"

#include <GL/glew.h>
#include <iostream>
#include <sstream>

#include "core/Globals.h"
#include "resources/SpriteSheet.h"
#include "ecs/components/TransformComponent.h"
#include "ecs/components/SpriteComponent.h"
#include "ecs/components/SpriteSheetComponent.h"

namespace tenshi
{
	Entity _spriteSheetEntity;

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

		g_Window = glfwCreateWindow(g_WindowWidth, g_WindowHeight, g_WindowTitle, nullptr, nullptr);
		glfwMakeContextCurrent(g_Window);
		glfwSwapInterval(1);

		GLenum _glInit = glewInit();
		if (_glInit != GLEW_OK)
		{
			std::cerr << "Could not initialize GLEW. Error: " << glewGetErrorString(_glInit);
			return;
		}

#ifdef _DEBUG
		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(glewMessageCallback, nullptr);
#endif

		// Enable Transparency
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glfwSetWindowSizeCallback(g_Window, [](GLFWwindow* window, i32 width, i32 height)
			{
				g_WindowWidth = width;
				g_WindowHeight = height;
				glViewport(0, 0, width, height);
				g_OnWindowResize.Dispatch(width, height);
			});

		glViewport(0, 0, g_WindowWidth, g_WindowHeight);

		// -- Init Global Systemss
		g_ResourceManager = std::make_unique<ResourceManager>();
		g_MasterRenderer = std::make_unique<MasterRenderer>();
		g_InputManager = std::make_unique<InputManager>();
		g_Ecs = std::make_unique<Ecs>();
		g_Ecs->Init();
		g_Camera = std::make_unique<Camera>();

		m_InitStatus = true;

		// -- Testing
		{
			Entity _entity = g_Ecs->CreateEntity();
			TransformComponent* transform = new TransformComponent();
			SpriteComponent* sprite = new SpriteComponent(g_ResourceManager->GetTexture("Wood.png"));
			g_Ecs->AddComponent<TransformComponent>(_entity, *transform);
			g_Ecs->AddComponent<SpriteComponent>(_entity, *sprite);
		}
		{
			_spriteSheetEntity = g_Ecs->CreateEntity();
			TransformComponent* transform = new TransformComponent(glm::vec2(2.0f, 1.0f), glm::vec2(0.0f), glm::vec2(1.0f));
			SpriteSheetComponent* spriteSheet = new SpriteSheetComponent(g_ResourceManager->GetSpriteSheet("Player"));
			g_Ecs->AddComponent<TransformComponent>(_spriteSheetEntity, *transform);
			g_Ecs->AddComponent<SpriteSheetComponent>(_spriteSheetEntity, *spriteSheet);
		}
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
		f32 _fps = 0.075f;
		f32 _counter = 0.0f;

		SpriteSheetComponent& _spriteSheet = *g_Ecs->GetComponent<SpriteSheetComponent>(_spriteSheetEntity);

		while (!glfwWindowShouldClose(g_Window) && m_InitStatus)
		{
			_currentFrameTime = glfwGetTime();
			g_DeltaTime = _currentFrameTime - _lastFrameTime;
			_lastFrameTime = _currentFrameTime;

#ifdef _DEBUG
			std::stringstream _ss;
			_ss << g_WindowTitle << " Delta Time: " << g_DeltaTime << " | Camera Position "
				<< g_Camera->m_Position.x << " " << g_Camera->m_Position.y;
			glfwSetWindowTitle(g_Window, _ss.str().c_str());
#endif

			glfwPollEvents();
			glClearColor(0.2f, 0.2f, 0.5f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			g_InputManager->Update();
			g_OnUpdate.Dispatch();

			g_MasterRenderer->Render();

			glfwSwapBuffers(g_Window);
		}
	}
}
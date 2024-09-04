#include "core/Game.h"

#include <GL/glew.h>
#include <iostream>
#include <sstream>

#include "core/Globals.h"
#include "resources/SpriteSheet.h"

namespace tenshi
{
	u32 _spriteSheetEntity = 0;

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
		g_EntityManager = std::make_unique<EntityManager>();
		g_InputManager = std::make_unique<InputManager>();
		g_Camera = std::make_unique<Camera>();

		// -- Testing
		const i32 _ENTITIES = 10;
		for (i32 i = 0; i < _ENTITIES; i++)
		{
			SpriteEntity& entity = g_EntityManager->CreateEntity<SpriteEntity, std::shared_ptr<Texture>>
				(g_ResourceManager->GetTexture("Wood.png"));
			entity.m_Transform.Translate(glm::vec2(i, 2.0f));
			g_MasterRenderer->AddStaticEntity(entity.m_EntityId, entity.m_Sprite->m_Texture);
		}

		SpriteSheet* spriteSheet = new SpriteSheet(g_ResourceManager->GetTexture("Gem_Merchant.png"),
			101, 37);
		SpriteSheetEntity& _entity = g_EntityManager->CreateEntity<SpriteSheetEntity>(*spriteSheet);
		g_MasterRenderer->AddDynamicEntity(_entity.m_EntityId, *spriteSheet);

		_spriteSheetEntity = _entity.m_EntityId;
		EventHandler<i32> _onSave([](i32 key)
			{
				if (key != GLFW_KEY_F3)
					return;

				g_EntityManager->Save();
			});
		g_InputManager->OnKeyDown.Subscribe(_onSave);

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
		f32 _fps = 0.05f;
		f32 _counter = 0.0f;

		SpriteSheetEntity& _entity = *g_EntityManager->GetEntity<SpriteSheetEntity>(_spriteSheetEntity);

		while (!glfwWindowShouldClose(g_Window) && m_InitStatus)
		{
			_currentFrameTime = glfwGetTime();
			g_DeltaTime = _currentFrameTime - _lastFrameTime;
			_lastFrameTime = _currentFrameTime;

#ifdef _DEBUG
			std::stringstream _ss;
			_ss << g_WindowTitle << " Delta Time: " << g_DeltaTime;
			glfwSetWindowTitle(g_Window, _ss.str().c_str());
#endif

			glfwPollEvents();
			glClearColor(0.2f, 0.2f, 0.5f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			g_InputManager->Update();

			if (_counter >= _fps)
			{
				if (_entity.m_CurrentFrame >= _entity.m_SpriteSheet->FRAME_COUNT - 1)
					_entity.SetFrame(0);
				else
					_entity.SetFrame(_entity.m_CurrentFrame + 1);

				_counter = 0;
			}

			_counter += g_DeltaTime;

			g_MasterRenderer->Render();

			glfwSwapBuffers(g_Window);
		}
	}
}
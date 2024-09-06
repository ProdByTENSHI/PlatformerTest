#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <memory>
#include <keccak.h>
#include <md5.h>

#include "tenshiUtil/Types.h"
#include "tenshiUtil/eventsystem/EventSystem.h"
#include "resources/ResourceManager.h"
#include "render/MasterRenderer.h"
#include "entity/EntityManager.h"
#include "render/Camera.h"
#include "input/InputManager.h"

// -- Window
inline tenshi::i32 g_WindowWidth = 1280;
inline tenshi::i32 g_WindowHeight = 720;
inline const char* g_WindowTitle = "Platformer";
inline GLFWwindow* g_Window = nullptr;

// -- Systems
inline std::unique_ptr<tenshi::ResourceManager> g_ResourceManager = nullptr;
inline std::unique_ptr<tenshi::MasterRenderer> g_MasterRenderer = nullptr;
inline std::unique_ptr<tenshi::EntityManager> g_EntityManager = nullptr;
inline std::unique_ptr<tenshi::InputManager> g_InputManager = nullptr;
inline std::unique_ptr<tenshi::Camera> g_Camera = nullptr;

// -- Events
inline tenshi::Event<tenshi::i32, tenshi::i32> g_OnWindowResize;
inline tenshi::Event<> g_OnUpdate;

// -- Globals
inline tenshi::f32 g_DeltaTime = 0.0f;
inline tenshi::u32 g_MAX_SPRITES = 500.000;

// -- Hashing
inline Keccak g_Keccak = Keccak();
inline MD5 g_MD5 = MD5();
#pragma once

#include <GLFW/glfw3.h>
#include <memory>

#include "tenshiUtil/Types.h"
#include "tenshiUtil/eventsystem/EventSystem.h"
#include "resources/ResourceManager.h"
#include "render/MasterRenderer.h"
#include "entity/EntityManager.h"

// -- Window
inline tenshi::i32 g_WindowWidth = 1280;
inline tenshi::i32 g_WindowHeight = 720;
inline GLFWwindow* g_Window = nullptr;

// -- Systems
inline std::unique_ptr<tenshi::ResourceManager> g_ResourceManager = nullptr;
inline std::unique_ptr<tenshi::MasterRenderer> g_MasterRenderer = nullptr;
inline std::unique_ptr<tenshi::EntityManager> g_EntityManager = nullptr;

// -- Events
inline chroma::Event<tenshi::i32, tenshi::i32> g_OnWindowResize;

// -- Globals
inline tenshi::u32 g_MAX_SPRITES = 500.000;
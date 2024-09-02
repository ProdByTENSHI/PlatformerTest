#pragma once

#include <string>
#include <map>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "tenshiUtil/Types.h"
#include "tenshiUtil/eventsystem/EventSystem.h"
#include "input/InputProvider.h"

namespace tenshi
{
	class InputManager
	{
	public:
		InputManager();

		void AddInputProvider(const std::string& name, InputProvider* provider);
		void Update();

		// Do this only once and store the Pointer to the Input Provider as it is a costly operation
		InputProvider* GetInputProvider(const std::string& name);

	public:
		// [param] u32: GLFW KeyCode
		Event<i32> OnKeyDown;

		// [param] u32: GLFW KeyCode
		Event<i32> OnKeyUp;

		// [param] u32: GLFW KeyCode
		Event<i32> OnKeyHold;

		// Holds the Timer for the Key Hold for each Key
		std::map<i32, f32> m_KeyHoldData;

		std::vector<i32> m_CurrentlyPressedKeys;

	private:
		// Time since Press till the Key is marked as "Holding"
		const f32 KEY_HOLD_DELAY = 0.5f;

		std::map<std::string, InputProvider*> m_InputProvider;

	};
}
// Credits: https://www.youtube.com/watch?v=pOEyYwKtHJo
#pragma once

#include <vector>

#include "input/InputData.h"
#include "input/InputHandler.h"

namespace tenshi
{
	// Provides the Input and processes it through all InputHandlers
	class InputProvider
	{
	public:
		// Processes the Input through all Handlers and returns the final Input
		InputData& GetInput();

		// Adds an InputHandler into the Process Chain
		// [in] handler: Reference to the InputHandler
		void AddHandler(InputHandler& handler);

	private:
		std::vector<InputHandler*> m_Handler;
	};
}
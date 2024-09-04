// Credits: https://www.youtube.com/watch?v=pOEyYwKtHJo
#pragma once

#include <vector>

#include "input/InputData.h"
#include "input/InputMiddleware.h"
#include "tenshiUtil/eventsystem/EventSystem.h"

namespace tenshi
{
	// Provides the Input and processes it through all InputHandlers
	class InputProvider
	{
	public:
		InputProvider();

		// Processes the Input through all Handlers and returns the final Input
		InputData GetInput();

		// Adds an InputHandler into the Process Chain
		// [in] handler: Reference to the InputHandler
		void AddMiddleware(InputMiddleware& handler);

	private:
		std::vector<InputMiddleware*> m_Handler;
	};
}
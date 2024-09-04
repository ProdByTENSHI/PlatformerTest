#pragma once

#include "input/InputData.h"
#include "tenshiUtil/eventsystem/EventSystem.h"

namespace tenshi
{
	// Abstract Base Class for all Classes that want to process the InputData
	// Input Handler act as a Middle Ware as we process Player Input by the Chain of Responsibility Pattern
	struct InputMiddleware
	{
		InputMiddleware();
		virtual ~InputMiddleware() {}

		virtual void Process(InputData& data) = 0;
	};
}
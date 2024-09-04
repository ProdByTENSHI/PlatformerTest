#pragma once

#include "input/InputMiddleware.h"

namespace tenshi
{
	class PlayerInputMiddleware : public InputMiddleware
	{
		void Process(InputData& data) override;
	};
}
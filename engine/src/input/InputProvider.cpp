#include "input/InputProvider.h"

namespace tenshi
{
	InputProvider::InputProvider()
	{
	}

	InputData InputProvider::GetInput()
	{
		InputData _data;

		for (auto& handler : m_Handler)
		{
			handler->Process(_data);
		}

		return _data;
	}

	void InputProvider::AddMiddleware(InputMiddleware& handler)
	{
		m_Handler.push_back(&handler);
	}
}
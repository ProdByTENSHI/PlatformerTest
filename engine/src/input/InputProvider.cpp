#include "input/InputProvider.h"

namespace tenshi
{
	InputData& InputProvider::GetInput()
	{
		InputData _data;

		for (auto& handler : m_Handler)
		{
			handler->Process(_data);
		}

		return _data;
	}

	void InputProvider::AddHandler(InputHandler& handler)
	{
		m_Handler.push_back(&handler);
	}
}
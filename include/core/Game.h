#pragma once

namespace tenshi
{
	class Game
	{
	public:
		Game();
		~Game();

		void Update();

	private:
		bool m_InitStatus = false;
	};
}
#include "ecs/System.h"

#include "ecs/ComponentTypes.h"

namespace tenshi
{
	System::System(Signature signature, u8 id)
		: m_Signature(signature), m_Id(id)
	{
	}
}
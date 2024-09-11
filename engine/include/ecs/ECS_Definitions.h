#pragma once

#include "tenshiUtil/Types.h"

namespace tenshi
{
	// An Entity is just a Unique 32-Bit Integer ID
	typedef u32 Entity;

	// Max Entities per Scene
	const u32 MAX_ENTITIES = 10000;

	// Max Components per Entity
	const u32 MAX_COMPONENTS = 32;

	// The Signature for an ECS System
	typedef std::bitset<MAX_COMPONENTS> Signature;
}
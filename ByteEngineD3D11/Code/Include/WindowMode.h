#pragma once

#include "TypesAliases/PrimitiveTypesAliases.h"

namespace ByteEngine
{
	enum class WindowMode : uint8
	{
		MINIMIZED,
		WINDOWED,
		MAXIMIZED,
		BORDERLESS_FULLSCREEN,
		EXCLUSIVE_FULLSCREEN
	};
}
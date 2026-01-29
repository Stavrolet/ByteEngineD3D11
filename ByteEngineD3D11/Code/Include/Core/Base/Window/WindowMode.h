#pragma once

#include "Primitives.h"

namespace ByteEngine
{
	enum class WindowMode : uint8
	{
		Minimized,
		Windowed,
		Mazimized,
		BorderlessFullscreen,
		ExclusiveFullscreen
	};
}
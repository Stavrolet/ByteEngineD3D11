#pragma once

#include "Primitives.h"

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
#pragma once

#ifdef BYTEENGINE_EXPORTS
#define BYTEENGINE_API __declspec(dllexport)
#else
#define BYTEENGINE_API __declspec(dllimport)
#endif
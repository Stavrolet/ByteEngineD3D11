#pragma once

#include "Base/Singleton.h"
#include "TypesAliases/PrimitiveTypesAliases.h"

struct HINSTANCE__;
using HINSTANCE = HINSTANCE__*;

namespace ByteEngine
{
    class Application : public Singleton<Application>
    {
        friend extern int __stdcall ::wWinMain(HINSTANCE, HINSTANCE, wchar_t*, int);

    private:
        int32 exitCode = 0;
        bool isRunning = true;

    public:
        void Quit(int32 exitCode);

    private:
        int32 Run();
    };
}
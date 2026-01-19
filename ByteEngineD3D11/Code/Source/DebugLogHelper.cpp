#include "WinApiExcludingDefs/NoAll.h"
#undef NOMB
#undef NOUSER

#include "TypesAliases/PrimitiveTypesAliases.h"
#include "TypesAliases/StdStringTypesAliases.h"
#include "DebugLogHelper.h"

#include <filesystem>
#include <chrono>
#include <Windows.h>

namespace ByteEngine::DebugHelper
{
#ifndef _DEBUG
    [[noreturn]]
#endif
    void LogCriticalError(StringView errorMessageForUser, uint32 errorCode, const ::std::source_location& loc)
    {
#ifdef _DEBUG
        LogDebugError(errorCode, loc);
#else
        uint32 errorCode = GetLastError();

        if (errorCode == ERROR_NOT_ENOUGH_MEMORY || errorCode == ERROR_OUTOFMEMORY || static_cast<HRESULT>(errorCode) == E_OUTOFMEMORY)
        {
            MessageBox(nullptr, L"Not enough memory to continue execution. The applcation will be closed.", L"Critical Error", MB_OK | MB_ICONERROR);
            exit(errorCode);
        }

        String formattedMessage = std::format("{} The applcation will be closed.\nError Code: {}", errorMessageForUser, errorCode);
        MessageBoxA(nullptr, formattedMessage.c_str(), "Critical Error", MB_OK | MB_ICONERROR);
        exit(errorCode);
#endif
    }

    void LogDebugError(uint32 errorCode, const ::std::source_location& loc)
    {
#ifdef _DEBUG
        void* formattedMessageBuff = nullptr;

        uint32 formatResult = FormatMessageA(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            nullptr, errorCode,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            reinterpret_cast<LPSTR>(&formattedMessageBuff),
            0, nullptr
        );

        if (formatResult == 0)
        {
            MessageBox(nullptr, L"FormatMessage failed", L"Error", MB_OK | MB_ICONERROR);
            return;
        }

        size_t messageLength = std::strlen(static_cast<const char*>(formattedMessageBuff));
        static_cast<char*>(formattedMessageBuff)[messageLength - 2] = '\0';
        static_cast<char*>(formattedMessageBuff)[messageLength - 1] = ' ';

        String formattedOutput = std::format(
            "[DEBUG ERROR] {} Code: 0x{:x}. Function: {}, file: {}:{}.\n",
            static_cast<const char*>(formattedMessageBuff), errorCode,
            loc.function_name(), loc.file_name(), loc.line()
        );

        MessageBeep(MB_ICONERROR);
        OutputDebugStringA(formattedOutput.c_str());

        LocalFree(formattedMessageBuff);
        __debugbreak();
#endif
    }

    void LogDebugMessageInternal(FmtWithLocation fmt, std::format_args args)
    {
        String fmtStr(fmt.fmt);

        if (fmtStr[fmtStr.length() - 1] != '.')
            fmtStr += '.';

        fmtStr += " [FILE: " + std::filesystem::path(fmt.loc.file_name()).filename().string() + ":" + std::to_string(fmt.loc.line()) + "]\n";

        String time = std::format("[TIME {:%T}] ", std::chrono::system_clock::now());

        String formattedMessage = time + std::vformat(fmtStr, args);
        OutputDebugStringA(formattedMessage.c_str());
    }
}
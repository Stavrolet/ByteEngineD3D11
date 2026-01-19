#pragma once

#include <wil/com.h>

namespace ByteEngine
{
    template<typename T>
    using ComPtr = wil::com_ptr_nothrow<T>;
}
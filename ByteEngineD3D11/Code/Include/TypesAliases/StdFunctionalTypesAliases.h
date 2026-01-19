#pragma once

#include <functional>

namespace ByteEngine
{
    template<typename... TArgs>
    using Proc = std::function<void(TArgs...)>;

    using Action = std::function<void()>;

    template<typename TResult, typename... TArgs>
    using Func = std::function<TResult(TArgs...)>;
}
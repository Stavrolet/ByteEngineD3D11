#pragma once

#include "ByteEngine/Math/Vector2.h"
#include "ByteEngine/Math/Vector3.h"

namespace ByteEngine::Math
{
    template<AnyNumber T>
    Vector2t<T>::operator Vector3t<T>() const
    {
        return Vector3t(x, y, 0);
    }

    template<AnyNumber T>
    template<AnyNumber U>
        requires (!std::is_same_v<T, U>&& std::is_convertible_v<T, U>)
    Vector2t<T>::operator Vector3t<U>() const
    {
        return Vector3t(static_cast<U>(x), static_cast<U>(y), 0);
    }

    template<AnyNumber T>
    Vector3t<T>::operator Vector2t<T>() const
    {
        return Vector2t(x, y);
    }

    template<AnyNumber T>
    template<AnyNumber U>
        requires (!std::is_same_v<T, U>&& std::is_convertible_v<T, U>)
    Vector3t<T>::operator Vector2t<U>() const
    {
        return Vector3t(static_cast<U>(x), static_cast<U>(y), 0);
    }
}
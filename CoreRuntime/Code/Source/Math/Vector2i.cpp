#include "ByteEngine/Math/Vector2.h"
#include "ByteEngine/Math/Vector2i.h"
#include "ByteEngine/Math/Vector3.h"

namespace ByteEngine::Math
{
    Vector2i::operator Vector2() const { return Vector2(static_cast<float>(x), static_cast<float>(y)); }
    Vector2i::operator Vector3() const { return Vector3(static_cast<float>(x), static_cast<float>(y), 0.0f); }
}
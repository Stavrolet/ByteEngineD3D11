#include "Math/Vector2.h"

namespace ByteEngine::Math
{
    void Vector2::Normalize()
    {
        float length = LengthSquared();

        if (length != 0.0f)
        {
            length = Math::Sqrt(length);
            x /= length;
            y /= length;
        }
    }

    Vector2 Vector2::Normalized() const
    {
        Vector2 copy = *this;
        copy.Normalize();
        return copy;
    }

    void Vector2::LimitLength(float maxLength)
    {
        float currentLength = LengthSquared();

        if (currentLength > maxLength * maxLength)
        {
            currentLength = Math::Sqrt(currentLength);
            *this *= maxLength / currentLength;
        }
    }

    RadianF Vector2::AngleBetween(Vector2 from, Vector2 to)
    {
        float cross = Cross(from, to);
        float dot = Dot(from, to);
        return Math::Atan2(cross, dot);
    }

    RadianF Vector2::UnsigedAngleBetween(Vector2 from, Vector2 to) { return Math::Abs(AngleBetween(from, to)); }

    Vector2 Vector2::Direction(Vector2 from, Vector2 to)
    {
        Vector2 dir = to - from;
        dir.Normalize();
        return dir;
    }

    RadianF Vector2::DirectionAngle(Vector2 from, Vector2 to)
    {
        Vector2 dir = to - from;
        dir.Normalize();
        return Math::Atan2(dir.x, dir.y);
    }

    Vector2 Vector2::Slerp(Vector2 from, Vector2 to, float t)
    {
        assert(t >= 0.0f && t <= 1.0f);  

        float startLength = from.LengthSquared();
        float endLength = to.LengthSquared();

        if (startLength == 0.0f || endLength == 0.0f)
            return Lerp(from, to, t);

        startLength = Math::Sqrt(startLength);
        float resultLength = Math::Lerp(startLength, Math::Sqrt(endLength), t);
        float angle = AngleBetween(from, to);        

        from.RotateBy(angle * t);
        return from * (resultLength / startLength);
    }

    Vector2 Vector2::MoveTowards(Vector2 current, Vector2 target, float maxDelta)
    {
        Vector2 vd = target - current;
        float length = vd.Length();

        if (length <= maxDelta || length < Math::Epsilon)
            return target;
        else
            return current + vd / length * maxDelta;
    }
}
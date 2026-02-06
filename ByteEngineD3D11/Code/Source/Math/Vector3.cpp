#include "./pch.h"

#include "Math/Vector3.h"

namespace ByteEngine::Math
{
    float Vector3::Length() const { return Math::Sqrt(LengthSquared()); }

    void Vector3::Normalize()
    {
        float length = LengthSquared();
        
        if (length != 0.0f)
            *this /= Math::Sqrt(length);
    }

    Vector3 Vector3::Normalized() const
    {
        Vector3 copy = *this;
        copy.Normalize();
        return copy;
    }

    void Vector3::LimitLength(float maxLength)
    {
        float currentLength = LengthSquared();

        if (currentLength > maxLength * maxLength)
        {
            currentLength = Math::Sqrt(currentLength);
            *this *= maxLength / currentLength;
        }
    }

    RadianF Vector3::AngleBetween(Vector3 from, Vector3 to, Vector3 axis)
    {
        Vector3 cross = Cross(from, to);
        float unsignedAngle = Math::Atan2(cross.Length(), Dot(from, to));
        float sign = Math::Sign(Dot(cross, axis));
        return unsignedAngle * sign;
    }

    RadianF Vector3::UnsigedAngleBetween(Vector3 from, Vector3 to) { return Math::Atan2(Cross(from, to).Length(), Dot(from, to)); }
    float Vector3::Distcance(Vector3 a, Vector3 b) { return Math::Sqrt(DistcanceSquared(a, b)); }

    Vector3 Vector3::Direction(Vector3 from, Vector3 to)
    {
        Vector3 dir = to - from;
        dir.Normalize();
        return dir;
    }

    Vector3 Vector3::MoveTowards(Vector3 current, Vector3 target, float maxDelta)
    {
        Vector3 vd = target - current;
        float length = vd.Length();

        if (length <= maxDelta || length < Math::Epsilon)
            return target;
        else
            return current + vd / length * maxDelta;
    }
}
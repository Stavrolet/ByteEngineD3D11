#include <DirectXMath.h>

#include "ByteEngine/Math/Vector2.h"
#include "ByteEngine/Math/Quaternion.h"

using namespace DirectX;

namespace ByteEngine::Math
{
    BYTEENGINE_API float Quaternion::Length() const { return Math::Sqrt(LengthSquared()); }

    BYTEENGINE_API void Quaternion::Normalize()
    {
        float length = LengthSquared();

        if (length != 0.0f)
        {
            length = Math::Sqrt(length);
            x /= length;
            y /= length;
            z /= length;
            w /= length;
        }
    }

    BYTEENGINE_API Quaternion Quaternion::Normalized() const
    {
        Quaternion copy = *this;
        copy.Normalize();
        return copy;
    }

    BYTEENGINE_API Vector3f Quaternion::GetEuler()
    {
        if (!IsNormalized())
            Normalize();

        float num = w * w - x * x;
        float ySq = y * y;
        float zSq = z * z;

        return Vector3f(Math::Asin(2 * (w * x - y * z)).value, Math::Atan2(2 * (w * y + x * z), num + ySq - zSq).value, Math::Atan2(2 * (w * z + x * y), num - ySq + zSq).value);
    }

    BYTEENGINE_API Vector3f Quaternion::GetEuler() const
    {
        assert(IsNormalized());

        float num = w * w - x * x;
        float ySq = y * y;
        float zSq = z * z;

        return Vector3f(Math::Asin(2 * (w * x - y * z)).value, Math::Atan2(2 * (w * y + x * z), num + ySq - zSq).value, Math::Atan2(2 * (w * z + x * y), num - ySq + zSq).value);
    }

    BYTEENGINE_API Vector3f Quaternion::GetEulerInDegrees() const { return GetEuler() * (180.0f / Math::PI); }

    // GetAxis implementation adapted from Godot Engine (MIT License). See THIRDPARTY.md
    // Source: Quaternion::get_axis
    BYTEENGINE_API Vector3f Quaternion::GetAxis() const
    {
        if (Math::Abs(w) > 1 - Math::Epsilon)
            return Vector3f(x, y, z);

        float invRoot = 1.0f / Math::Sqrt(1 - w * w);
        return Vector3f(x * invRoot, y * invRoot, z * invRoot);
    }

    // GetAngle implementation adapted from Godot Engine (MIT License). See THIRDPARTY.md
    // Source: Quaternion::get_angle
    BYTEENGINE_API RadianF Quaternion::GetAngle() const { return 2 * Math::Acos(w); }

    BYTEENGINE_API RadianF Quaternion::AngleBetween(Quaternion a, Quaternion b) { return 2 * Math::Acos(Math::Abs(Dot(a, b))); }

    // FromAngleAxis implementation adapted from Godot Engine (MIT License). See THIRDPARTY.md
    // Source: Quaternion::Quaternion(const Vector3f &p_axis, real_t p_angle)
    BYTEENGINE_API Quaternion Quaternion::FromAngleAxis(RadianF angle, Vector3f axis)
    {
        if (!axis.IsNormalized())
            axis.Normalize();

        float length = axis.Length();

        if (length == 0)
        {
            return Quaternion(0.0f);
        }
        else
        {
            float sin = Math::Sin(static_cast<RadianF>(angle * 0.5f));
            float cos = Math::Cos(static_cast<RadianF>(angle * 0.5f));
            float s = sin / length;

            return Quaternion(axis.x * s, axis.y * s, axis.z * s, cos);
        }
    }

    BYTEENGINE_API Quaternion Quaternion::FromAngleAxis(DegreeF angle, Vector3f axis) { return FromAngleAxis(Math::DegToRad(angle), axis); }

    BYTEENGINE_API Quaternion Quaternion::FromLookDirection(Vector3f direction, Vector3f worldUp)
    {
        if (Math::IsEqualApproximetly(direction.LengthSquared(), 0.0f))
            return Quaternion(0.0f);

        XMVECTOR forward = XMLoadFloat3(reinterpret_cast<XMFLOAT3*>(&direction));
        XMVECTOR worldUp2 = XMLoadFloat3(reinterpret_cast<XMFLOAT3*>(&worldUp));

        if (!direction.IsNormalized())
            forward = XMVector3Normalize(forward);

        XMVECTOR right = XMVector3Cross(worldUp2, forward);
        XMVECTOR up = XMVector3Cross(forward, right);

        XMMATRIX m = XMMatrixIdentity();
        m.r[0] = right;
        m.r[1] = up;
        m.r[2] = forward;

        XMVECTOR q = XMQuaternionRotationMatrix(m);
        Quaternion result;
        XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&result), q);
        return result;
    }

    BYTEENGINE_API Quaternion Quaternion::FromToRotation(Vector3f from, Vector3f to)
    {
        from.Normalize();
        to.Normalize();

        float dot = Math::Clamp(Vector3f::Dot(from, to), -1.0f, 1.0f);
        
        if (dot >= 1.0f || Math::IsEqualApproximetly(dot, 0.0f))
            return Quaternion();
        else if (dot <= -1.0f)
            return FromAngleAxis(static_cast<RadianF>(Math::PI), Vector3f(0.0f, 1.0f, 0.0f));

        Vector3f axis = Vector3f::Cross(from, to);
        float root = Math::Sqrt((1.0f + dot) * 2.0f);
        float invRoot = 1.0f / root;

        return Quaternion(axis.x * invRoot, axis.y * invRoot, axis.z * invRoot, root * 0.5f);
    }

    // SlerpUnclamped implementation adapted from Godot Engine (MIT License). See THIRDPARTY.md
    // Source: Quaternion::slerp
    BYTEENGINE_API Quaternion Quaternion::SlerpUnclamped(Quaternion from, Quaternion to, float t)
    {
        assert(from.IsNormalized());
        assert(to.IsNormalized());
        assert(t >= 0 && t <= 1);

        Quaternion to1;
        RadianF omega;
        float cosom, sinom, scale0, scale1;

        cosom = Dot(from, to);

        if (cosom < 0.0f)
        {
            cosom = -cosom;
            to1 = -to;
        }
        else
        {
            to1 = to;
        }

        if ((1.0f - cosom) > Math::Epsilon)
        {
            omega = Math::Acos(cosom);
            sinom = Math::Sin(omega);
            scale0 = Math::Sin(static_cast<RadianF>((1.0f - t) * omega)) / sinom;
            scale1 = Math::Sin(static_cast<RadianF>(t * omega)) / sinom;
        }
        else
        {
            scale0 = 1.0f - t;
            scale1 = t;
        }
        
        return Quaternion(
            scale0 * from.x + scale1 * to1.x,
            scale0 * from.y + scale1 * to1.y,
            scale0 * from.z + scale1 * to1.z,
            scale0 * from.w + scale1 * to1.w
        );
    }

    BYTEENGINE_API Quaternion Quaternion::Slerp(Quaternion from, Quaternion to, float t) { return SlerpUnclamped(from, to, Math::Clamp(t)); }
}
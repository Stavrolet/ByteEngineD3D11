#include <DirectXMath.h>

#include "Math/Vector2.h"
#include "Math/Quaternion.h"

using namespace DirectX;

namespace ByteEngine::Math
{
    float Quaternion::Length() const { return Math::Sqrt(LengthSquared()); }

    void Quaternion::Normalize()
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

    Quaternion Quaternion::Normalized() const
    {
        Quaternion copy = *this;
        copy.Normalize();
        return copy;
    }

    Vector3 Quaternion::GetEuler()
    {
        if (!IsNormalized())
            Normalize();

        float num = w * w - x * x;
        float ySq = y * y;
        float zSq = z * z;

        return Vector3(Math::Asin(2 * (w * x - y * z)), Math::Atan2(2 * (w * y + x * z), num + ySq - zSq), Math::Atan2(2 * (w * z + x * y), num - ySq + zSq));
    }

    Vector3 Quaternion::GetEuler() const
    {
        assert(IsNormalized());

        float num = w * w - x * x;
        float ySq = y * y;
        float zSq = z * z;

        return Vector3(Math::Asin(2 * (w * x - y * z)), Math::Atan2(2 * (w * y + x * z), num + ySq - zSq), Math::Atan2(2 * (w * z + x * y), num - ySq + zSq));
    }

    Vector3 Quaternion::GetEulerInDegrees() const { return GetEuler() * (180.0f / Math::PI); }

    Vector3 Quaternion::GetAxis() const
    {
        if (Math::Abs(w) > 1 - Math::Epsilon)
            return Vector3(x, y, z);

        float invRoot = 1.0f / Math::Sqrt(1 - w * w);
        return Vector3(x * invRoot, y * invRoot, z * invRoot);
    }

    RadianF Quaternion::GetAngle() const { return 2 * Math::Acos(w); }

    void Quaternion::GetAxisAngle(Vector3& axis, RadianF& angle) const
    {
        angle = GetAngle();
        axis = GetAxis();
    }

    RadianF Quaternion::AngleBetween(Quaternion a, Quaternion b) { return 2 * Math::Acos(Math::Abs(Dot(a, b))); }

    Quaternion Quaternion::FromAngleAxis(RadianF angle, Vector3 axis)
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

    Quaternion Quaternion::FromAngleAxis(DegreeF angle, Vector3 axis) { return FromAngleAxis(Math::DegToRad(angle), axis); }

    Quaternion Quaternion::FromLookDirection(Vector3 direction, Vector3 worldUp)
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

    Quaternion Quaternion::FromToRotation(Vector3 from, Vector3 to)
    {
        assert(!Math::IsEqualApproximetly(from.LengthSquared(), 0.0f));
        assert(!Math::IsEqualApproximetly(to.LengthSquared(), 0.0f));

        from.Normalize();
        to.Normalize();

        float dot = Math::Clamp(Vector3::Dot(from, to), -1.0f, 1.0f);
        
        if (dot >= 1.0f)
            return Quaternion();
        else if (dot <= -1.0f)
            return FromAngleAxis(static_cast<RadianF>(Math::PI), Vector3(0.0f, 1.0f, 0.0f));

        Vector3 axis = Vector3::Cross(from, to);
        float root = Math::Sqrt((1.0f + dot) * 2.0f);
        float invRoot = 1.0f / root;

        return Quaternion(axis.x * invRoot, axis.y * invRoot, axis.z * invRoot, root * 0.5f);
    }

    Quaternion Quaternion::SlerpUnclamped(Quaternion from, Quaternion to, float t)
    {
        assert(from.IsNormalized());
        assert(to.IsNormalized());

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

    Quaternion Quaternion::Slerp(Quaternion from, Quaternion to, float t) { return SlerpUnclamped(from, to, Math::Clamp(t)); }

    Quaternion::operator Vector2() const { return Vector2(x, y); }
}
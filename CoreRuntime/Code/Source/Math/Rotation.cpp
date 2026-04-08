#include "ByteEngine/Math/Rotation.h"

namespace ByteEngine::Math
{
    void Rotation::Normalize()
    {
        pitch = DegreeF { Math::Fmod(pitch.value, 360.0f) };
        if (pitch < 0_df)
            pitch += 360.0_df;

        if (pitch > 180_df)
            pitch -= 360.0_df;

        yaw = DegreeF { Math::Fmod(yaw.value, 360.0f) };
        if (yaw < 0_df)
            yaw += 360.0_df;

        if (yaw > 180_df)
            yaw -= 360.0_df;

        roll = DegreeF { Math::Fmod(roll.value, 360.0f) };
        if (roll < 0_df)
            roll += 360.0_df;

        if (roll > 180_df)
            roll -= 360.0_df;
    }

    Rotation Rotation::Normalized() const
    {
        Rotation copy = *this;
        copy.Normalize();
        return copy;
    }

    Quaternion Rotation::ToQuaternion() const { return Quaternion::FromEuler(pitch, yaw, roll); }

    Vector3F Rotation::RotateVector(Vector3F vector) const
    {
        Quaternion q = ToQuaternion();
        return q * vector;
    }

    Rotation Rotation::FromQuaternion(Quaternion q)
    {
        EulerDeg euler = q.GetEulerInDegrees();
        return Rotation(euler);
    }

    bool Rotation::IsEqualApproximately(Rotation a, Rotation b, DegreeF epsilon)
    {
        return Math::IsEqualApproximetly(a.pitch.value, b.pitch.value, epsilon.value) &&
            Math::IsEqualApproximetly(a.yaw.value, b.yaw.value, epsilon.value) &&
            Math::IsEqualApproximetly(a.roll.value, b.roll.value, epsilon.value);
    }

    Rotation Rotation::Slerp(Rotation from, Rotation to, float t)
    {
        Quaternion qFrom = from.ToQuaternion();
        Quaternion qTo = to.ToQuaternion();
        Quaternion qResult = Quaternion::Slerp(qFrom, qTo, t);
        return FromQuaternion(qResult);
    }
}
#include <gtest/gtest.h>
#include <cmath>
#include "ByteEngine/Math/Vector2.h"
#include "ByteEngine/Math/Vector3.h"
#include "ByteEngine/Math/Vector4.h"
#include "ByteEngine/Math/VectorConversions.h"

using namespace ByteEngine::Math;

template <typename T>
class Vector2tTest : public ::testing::Test
{
protected:
    using Vec2 = Vector2t<T>;
};

template <typename T>
class Vector2tFloatTypesTest : public ::testing::Test
{
protected:
    using Vec2 = Vector2t<T>;
};

using Types = ::testing::Types<float, double, ByteEngine::int32, ByteEngine::int64>;
using FloatTypes = ::testing::Types<float, double>;

TYPED_TEST_SUITE(Vector2tTest, Types);
TYPED_TEST_SUITE(Vector2tFloatTypesTest, FloatTypes);

TYPED_TEST(Vector2tTest, LengthAndNormalize)
{
    using Vec2 = typename TestFixture::Vec2;
    Vec2 v(3.0, 4.0);
    EXPECT_NEAR(v.Length(), 5.0, 1e-5);
    EXPECT_NEAR(v.LengthSquared(), 25.0, 1e-5);

    if constexpr (FloatingPointNumber<decltype(v.x)>)
    {
        Vec2 normalized = v.Normalized();
        EXPECT_NEAR(normalized.Length(), 1.0, 1e-5);
        EXPECT_TRUE(normalized.IsNormalized());

        v.Normalize();
        EXPECT_TRUE(v.IsNormalized());
        EXPECT_NEAR(v.Length(), 1.0, 1e-5);

        Vec2 v2(1.0, 1.0);
        v2.LimitLength(0.5);
        EXPECT_NEAR(v2.Length(), 0.5, 1e-5);
    }
}

TYPED_TEST(Vector2tFloatTypesTest, Rotation)
{
    using Vec2 = typename TestFixture::Vec2;
    using RadianT = typename Vec2::RadianT;

    Vec2 v(1.0, 0.0);
    v.RotateBy(static_cast<RadianT>(Math::PI_D / 2.0));
    EXPECT_NEAR(v.x, 0.0, 1e-5);
    EXPECT_NEAR(v.y, 1.0, 1e-5);

    Vec2 v2(1.0, 0.0);
    Vec2 rotated = v2.RotatedBy(static_cast<RadianT>(Math::PI / 2.0));
    EXPECT_NEAR(rotated.x, 0.0, 1e-5);
    EXPECT_NEAR(rotated.y, 1.0, 1e-5);
}

TYPED_TEST(Vector2tFloatTypesTest, AngleMethods)
{
    using Vec2 = typename TestFixture::Vec2;
    Vec2 a(1.0, 0.0);
    Vec2 b(0.0, 1.0);

    EXPECT_NEAR(Vec2::AngleBetween(a, b), 1.57079 /* 90 degrees */, 1e-4);
    EXPECT_NEAR(Vec2::UnsigedAngleBetween(a, b), 1.57079, 1e-4);
}

TYPED_TEST(Vector2tTest, AlgebraMethods)
{
    using Vec2 = typename TestFixture::Vec2;
    Vec2 a(2.0, 3.0);
    Vec2 b(1.0, 5.0);

    if constexpr (FloatingPointNumber<decltype(a.x)>)
        EXPECT_NEAR(Vec2::Distcance(a, b), std::sqrt(5.0), 1e-5);
    else
        EXPECT_EQ(Vec2::Distcance(a, b), 2);

    EXPECT_NEAR(Vec2::DistcanceSquared(a, b), 5.0, 1e-5);

    if constexpr (FloatingPointNumber<decltype(a.x)>)
    {
        EXPECT_EQ(Vec2::Cross(a, b), 7.0);
        EXPECT_EQ(Vec2::Dot(a, b), 17.0);

        Vec2 p = Vec2::Project(Vec2(2, 2), Vec2(1, 0));
        EXPECT_EQ(p.x, 2.0);
        EXPECT_EQ(p.y, 0.0);
    }

    Vec2 dir = Vec2::Direction(Vec2(0, 0), Vec2(1, 1));
    if constexpr (FloatingPointNumber<decltype(dir.x)>)
        EXPECT_NEAR(dir.x, 0.707106, 1e-4);
    else
        EXPECT_EQ(dir.x, 1);
}

TYPED_TEST(Vector2tFloatTypesTest, InterpolationAndReflection)
{
    using Vec2 = typename TestFixture::Vec2;
    Vec2 start(0, 0);
    Vec2 end(10, 10);

    EXPECT_EQ(Vec2::Lerp(start, end, 0.5), Vec2(5, 5));
    EXPECT_EQ(Vec2::LerpClamped(start, end, 2.0), Vec2(10, 10));

    Vec2 vec(1, -1);
    Vec2 normal(0, 1);
    EXPECT_EQ(Vec2::Reflect(vec, normal), Vec2(1, 1));
}

TYPED_TEST(Vector2tTest, Operators)
{
    using Vec2 = typename TestFixture::Vec2;
    Vec2 v1(2.0, 3.0);
    Vec2 v2(1.0, 2.0);

    EXPECT_EQ(+v1, Vec2(2, 3));
    EXPECT_EQ(-v1, Vec2(-2, -3));
    EXPECT_EQ(v1 + v2, Vec2(3, 5));
    EXPECT_EQ(v1 + 2.0, Vec2(4, 5));
    EXPECT_EQ(v1 - v2, Vec2(1, 1));
    EXPECT_EQ(v1 - 1.0, Vec2(1, 2));
    EXPECT_EQ(v1 * v2, Vec2(2, 6));
    EXPECT_EQ(v1 * 2.0, Vec2(4, 6));
    EXPECT_EQ(2.0 * v1, Vec2(4, 6));
    EXPECT_EQ(v1 / v2, Vec2(2, 1.5));
    EXPECT_EQ(v1 / 2.0, Vec2(1, 1.5));

    Vec2 v3 = v1;

    v3 += v2;
    EXPECT_EQ(v3, Vec2(3, 5));

    v3 -= v2;
    EXPECT_EQ(v3, Vec2(2, 3));

    v3 *= 2.0;
    EXPECT_EQ(v3, Vec2(4, 6));

    v3 /= 2.0;
    EXPECT_EQ(v3, Vec2(2, 3));

    EXPECT_TRUE(v1 == Vec2(2, 3));
    EXPECT_TRUE(v1 != v2);
}

TYPED_TEST(Vector2tTest, Conversion)
{
    using Vec2 = typename TestFixture::Vec2;
    Vec2 v(1.0, 2.0);

    Vector3t<decltype(Vec2(1).x)> v3 = v;
    EXPECT_EQ(v3.x, 1.0);
    EXPECT_EQ(v3.y, 2.0);
    EXPECT_EQ(v3.z, 0.0);

    Vector4t<decltype(Vec2(1).x)> v4 = v;
    EXPECT_EQ(v4.x, 1.0);
    EXPECT_EQ(v4.y, 2.0);
    EXPECT_EQ(v4.z, 0.0);
    EXPECT_EQ(v4.w, 0.0);
}

TYPED_TEST(Vector2tTest, Access)
{
    using Vec2 = typename TestFixture::Vec2;
    Vec2 v(1.0, 2.0);

    EXPECT_EQ(v[0], 1.0);
    EXPECT_EQ(v[1], 2.0);
    v[0] = 5.0;
    v[1] = 2.0;
    EXPECT_EQ(v.x, 5.0);
    EXPECT_EQ(v.v, 2.0);
    
    v.width = 100.0;
    EXPECT_EQ(v[0], 100.0);
}

TYPED_TEST(Vector2tFloatTypesTest, IsEqualApproximetly)
{
    using Vec2 = typename TestFixture::Vec2;
    Vec2 v1(1.0000000001, 1.0);
    Vec2 v2(1.0, 1.0);
    EXPECT_TRUE(Vec2::IsEqualApproximetly(v1, v2));
}

TYPED_TEST(Vector2tTest, TypeConversionOperator)
{
    Vector2f fVec(1.1f, 2.2f);
    Vector2d dVec = fVec;

    EXPECT_NEAR(dVec.x, 1.1, 1e-6);
    EXPECT_NEAR(dVec.y, 2.2, 1e-6);

    Vector2i iVec = fVec;
    EXPECT_EQ(iVec.x, 1);
    EXPECT_EQ(iVec.y, 2);
}

template <typename T>
class Vector2tFloatTypesRobustnessTest : public ::testing::Test
{
protected:
    using Vec2 = Vector2t<T>;
};

template <typename T>
class Vector2tIntTypesRobustnessTest : public ::testing::Test
{
protected:
    using Vec2 = Vector2t<T>;
};

using FloatTypes = ::testing::Types<float, double>;
using IntTypes = ::testing::Types<ByteEngine::int32, ByteEngine::int64>;

TYPED_TEST_SUITE(Vector2tFloatTypesRobustnessTest, FloatTypes);
TYPED_TEST_SUITE(Vector2tIntTypesRobustnessTest, IntTypes);

TYPED_TEST(Vector2tFloatTypesRobustnessTest, VectorNormalization)
{
    using Vec2 = typename TestFixture::Vec2;
    Vec2 zero(0);

    zero.Normalize();
    EXPECT_TRUE(zero.x == 0 && zero.y == 0);

    Vec2 inf(Math::Infinity, Math::Infinity);
    inf.Normalize();
    EXPECT_TRUE(std::isnan(inf.x) && std::isnan(inf.y));

    inf = Vec2(Math::Infinity, 1213);
    inf.Normalize();
    EXPECT_TRUE(std::isnan(inf.x) && inf.y == 0);

    Vec2 nan(std::numeric_limits<decltype(zero.x)>::quiet_NaN());
    nan.Normalize();
    EXPECT_TRUE(nan.x == 0 && nan.y == 0);

    nan = Vec2(std::numeric_limits<decltype(zero.x)>::quiet_NaN(), 123);
    nan.Normalize();
    EXPECT_TRUE(nan.x == 0 && nan.y == 0);
}

TYPED_TEST(Vector2tFloatTypesRobustnessTest, DivisionByZero)
{
    using Vec2 = typename TestFixture::Vec2;
    Vec2 v(1.0, 1.0);
    Vec2 zero(0, 0);

    Vec2 result = v / 0;
    EXPECT_TRUE(std::isinf(result.x) && std::isinf(result.y));

    result = zero / 0;
    EXPECT_TRUE(std::isnan(result.x) && std::isnan(result.y));
}

TYPED_TEST(Vector2tFloatTypesRobustnessTest, ExtremeValuesOverflow)
{
    using Vec2 = typename TestFixture::Vec2;
    auto maxVal = std::numeric_limits<decltype(Vec2(0).x)>::max();

    Vec2 v(maxVal, maxVal);
    auto sq = v.LengthSquared();
    auto len = v.Length();

    EXPECT_TRUE(std::isinf(sq));
    EXPECT_TRUE(std::isinf(len));
}

TYPED_TEST(Vector2tIntTypesRobustnessTest, ExtremeValuesOverflow)
{
    using Vec2 = typename TestFixture::Vec2;
    auto maxVal = std::numeric_limits<decltype(Vec2(0).x)>::max();

    Vec2 v(maxVal, maxVal);
    auto sq = v.LengthSquared();
    auto len = v.Length();

    EXPECT_EQ(sq, 2);
    EXPECT_NEAR(len, std::sqrt(2), 1e-5);
}

TYPED_TEST(Vector2tFloatTypesRobustnessTest, AngleBetweenZeroVectors)
{
    using Vec2 = typename TestFixture::Vec2;
    Vec2 zero(0, 0);
    Vec2 normal(1, 0);

    auto angle = Vec2::AngleBetween(zero, normal);
    EXPECT_EQ(angle, 0);
}

TYPED_TEST(Vector2tFloatTypesRobustnessTest, ProjectAndReflectWithZeroNormal)
{
    using Vec2 = typename TestFixture::Vec2;
    Vec2 v(1, 1);
    Vec2 zeroNormal(0, 0);

    Vec2 proj = Vec2::Project(v, zeroNormal);
    EXPECT_EQ(proj.x, 0);
    EXPECT_EQ(proj.y, 0);

    proj = Vec2::ProjectNormalized(v, zeroNormal);
    EXPECT_EQ(proj.x, 0);
    EXPECT_EQ(proj.y, 0);

    Vec2 refl = Vec2::Reflect(v, zeroNormal);
    EXPECT_EQ(refl.x, v.x);
    EXPECT_EQ(refl.y, v.y);
}

TYPED_TEST(Vector2tFloatTypesRobustnessTest, ProjectOntoNonNormalizedVector)
{
    using Vec2 = typename TestFixture::Vec2;
    Vec2 vec(3, 4);
    Vec2 onto(1);

    Vec2 res = Vec2::Project(vec, onto);
    EXPECT_NEAR(res.x, 3.5, 1e-5);
    EXPECT_NEAR(res.y, 3.5, 1e-5);
}

TYPED_TEST(Vector2tFloatTypesRobustnessTest, RotateByPrecision)
{
    using Vec2 = typename TestFixture::Vec2;
    Vec2 v(1, 0);

    Vec2 rot = v.RotatedBy(1e-9);
    EXPECT_NEAR(rot.x, 1.0, 1e-8);
    EXPECT_NEAR(rot.y, 0.0, 1e-8);

    if constexpr (std::is_same_v<decltype(v.x), float>)
        rot = v.RotatedBy(10000 * (2 * Math::PI));
    else if constexpr (std::is_same_v<decltype(v.x), double>)
        rot = v.RotatedBy(10000 * (2 * Math::PI_D));

    EXPECT_NEAR(rot.x, 1.0, 1.3e-2);
    EXPECT_NEAR(rot.y, 0.0, 1.3e-2);
}
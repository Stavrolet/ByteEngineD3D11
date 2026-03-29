#include <gtest/gtest.h>
#include "ByteEngine/Math/Math.h"

using namespace ByteEngine::Math;

TEST(MathTest, ConstantsAndConversions)
{
    EXPECT_NEAR(Math::DegToRad(180.0_df), Math::PI, 1e-5f);
    EXPECT_NEAR(Math::RadToDeg(RadianD(Math::PI_D)), 180.0, 1e-5);
}

TEST(MathTest, Trigonometry)
{
    EXPECT_NEAR(Math::Sin(0.0_rf), 0.0f, 1e-5f);
    EXPECT_NEAR(Math::Sin(RadianF(Math::PI / 2.0f)), 1.0f, 1e-5f);
    EXPECT_NEAR(Math::Cos(0.0_rf), 1.0f, 1e-5f);
    EXPECT_NEAR(Math::Cos(RadianF(Math::PI)), -1.0f, 1e-5f);

    float s, c;
    Math::SinCos(s, c, RadianF(Math::PI / 2.0f));
    EXPECT_NEAR(s, 1.0f, 1e-5f);
    EXPECT_NEAR(c, 0.0f, 1e-5f);

    EXPECT_NEAR(Math::Asin(1.0f), Math::PI / 2.0f, 1e-4f);
    EXPECT_NEAR(Math::Acos(0.0f), Math::PI / 2.0f, 1e-4f);
}

TEST(MathTest, BasicUtilities)
{
    EXPECT_EQ(Math::Sign(-10.0f), -1.0f);
    EXPECT_EQ(Math::Sign(10.0f), 1.0f);
    EXPECT_EQ(Math::Sign(0.0f), 0.0f);

    EXPECT_NEAR(Math::Fract(1.75f), 0.75f, 1e-5f);
}

TEST(MathTest, Algebra)
{
    EXPECT_NEAR(Math::LogN(25.0f, 5.0f), 2.0f, 1e-5);
}

TEST(MathTest, ClampAndRemap)
{
    EXPECT_EQ(Math::Clamp(0.5f, 0.0f, 1.0f), 0.5f);
    EXPECT_EQ(Math::Clamp(2.0f, 0.0f, 1.0f), 1.0f);
    EXPECT_EQ(Math::Clamp(-1.0f, 0.0f, 1.0f), 0.0f);

    EXPECT_NEAR(Math::Remap(0.5f, 0.0f, 1.0f, 0.0f, 10.0f), 5.0f, 1e-5f);
}

TEST(MathTest, AngleDifference)
{
    EXPECT_NEAR(Math::AngleDifference(18.8495559_rf, RadianF(Math::PI / 2)), Math::PI / 2, 1e-4f);
}

TEST(MathTest, Interpolation)
{
    EXPECT_NEAR(Math::Lerp(0.0f, 10.0f, 0.5f), 5.0f, 1e-5f);
    EXPECT_NEAR(Math::InverseLerp(0.0f, 10.0f, 5.0f), 0.5f, 1e-5f);

    EXPECT_NEAR(Math::LerpAngle(0.0f, Math::PI / 2 + Math::PI * 2, 0.5f), 0.7853981f, 1e-4f);

    EXPECT_NEAR(Math::MoveTowards(0.0f, 10.0f, 2.0f), 2.0f, 1e-5f);
    EXPECT_NEAR(Math::MoveTowards(0.0f, 1.0f, 2.0f), 1.0f, 1e-5f);

    EXPECT_NEAR(Math::SmoothStep(0, 10, 2.0f), 0.104f, 1e-4f);
    EXPECT_NEAR(Math::SmoothStep(10, 10.0f, 0.3), 0.0, 1e-4f);
}

TEST(MathTest, RangeFunctions)
{
    EXPECT_NEAR(Math::LoopValue(12.0, 0.0f, 10.0f), 2.0f, 1e-5f);
    EXPECT_NEAR(Math::LoopValue(-2.0f, 0.0f, 10.0f), 8.0f, 1e-5f);

    EXPECT_EQ(Math::LoopValue(12, 0, 10), 2);
    EXPECT_EQ(Math::LoopValue(-2, 0, 10ll), 8);

    EXPECT_NEAR(Math::PingPong(12.0f, 10.0f), 8.0f, 1e-5f);
}

TEST(MathTest, EqualApproximetly)
{
    EXPECT_TRUE(Math::IsEqualApproximetly(1.0000001f, 1.0f, 0.0001f));
}

TEST(MathTest, Average)
{
    std::vector<float> data = { 1.0f, 2.0f, 3.0f, 4.0f };
    EXPECT_NEAR(Math::Average(data), 2.5f, 1e-5f);

    EXPECT_NEAR(Math::Average<double>({ 10.0, 20.0 }), 15.0, 1e-5);
}

TEST(MathTest, MinMax)
{
    std::vector<int> data = { 1, 2, 3, 4, -100, 0 };

    EXPECT_EQ(Math::Min(10, 2), 2);
    EXPECT_EQ(Math::Min(10, 2, 100), 2);
    EXPECT_EQ(Math::Min(data), -100);

    EXPECT_EQ(Math::Max(10, 2), 10);
    EXPECT_EQ(Math::Max(10, 2, 100), 100);
    EXPECT_EQ(Math::Max(data), 4);
}
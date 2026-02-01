#pragma once

namespace ByteEngine
{
    struct Vector2
    {
        union
        {
            struct
            {
                float x;
                float y;
            };

            struct
            {
                float u;
                float v;
            };

            float data[2];
        };

        explicit constexpr Vector2(float xy = 0.0f)
            : x(xy), y(xy) { }

        constexpr Vector2(float x, float y)
            : x(x), y(y) { }

        constexpr Vector2 operator+(Vector2 a) const { return Vector2(a.x + x, a.y + y); }
        constexpr Vector2 operator+(float a) const { return Vector2(a + x, a + y); }
        constexpr Vector2 operator-(Vector2 a) const { return Vector2(x - a.x, y - a.y); }
        constexpr Vector2 operator-(float a) const { return Vector2(x - a, y - a); }

        constexpr Vector2& operator+=(Vector2 a)
        {
            x += a.x;
            y += a.y;
            return *this;
        }

        constexpr Vector2& operator+=(float a)
        {
            x += a;
            y += a;
            return *this;
        }

        constexpr Vector2& operator-=(Vector2 a)
        {
            x -= a.x;
            y -= a.y;
            return *this;
        }

        constexpr Vector2& operator-=(float a)
        {
            x -= a;
            y -= a;
            return *this;
        }

        constexpr Vector2 operator*(Vector2 a) const { return Vector2(x * a.x, y * a.y); }
        constexpr Vector2 operator*(float s) const { return Vector2(x * s, y * s); }
        friend constexpr Vector2 operator*(float s, Vector2 v) { return v * s; }

        constexpr Vector2& operator*=(Vector2 a)
        {
            x *= a.x;
            y *= a.y;
            return *this;
        }

        constexpr Vector2& operator*=(float s)
        {
            x *= s;
            y *= s;
            return *this;
        }

        constexpr Vector2 operator/(Vector2 a) const { return Vector2(x / a.x, y / a.y); }
        constexpr Vector2 operator/(float s) const { return Vector2(x / s, y / s); }        

        constexpr Vector2& operator/=(Vector2 a)
        {
            x /= a.x;
            y /= a.y;
            return *this;
        }

        constexpr Vector2& operator/=(float s)
        {
            x /= s;
            y /= s;
            return *this;
        }

        float& operator[](int32 index) { return data[index]; }
        float operator[](int32 index) const { return data[index]; }
    };

    static inline constexpr Vector2 Zero(0.0f);
    static inline constexpr Vector2 One(1.0f);
    static inline constexpr Vector2 Up(0.0f, 1.0f);
    static inline constexpr Vector2 Down(0.0f, -1.0f);
    static inline constexpr Vector2 Left(-1.0f, 0.0f);
    static inline constexpr Vector2 Right(1.0f, 0.0f);
}
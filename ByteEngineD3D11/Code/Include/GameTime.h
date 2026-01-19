#pragma once

#include <chrono>
#include "TypesAliases/PrimitiveTypesAliases.h"

using namespace std::chrono;
using namespace ByteEngine;

class Time
{
private:
    friend void UpdateTime();

    static inline nanoseconds startTime = high_resolution_clock::now().time_since_epoch();
    static inline nanoseconds lastFrameTime = startTime;

    static inline float32 totalTime = 0.0f;
    static inline float32 deltaTime = 0.0f;

public:
    static float32 GetTotalTime() { return totalTime; }
    static float32 GetDeltaTime() { return deltaTime; }

private:
    void static Update()
    {
        deltaTime = static_cast<float>(duration_cast<nanoseconds>(high_resolution_clock::now().time_since_epoch() - lastFrameTime) / 1.0s);

        if (deltaTime > 0.1f)
            deltaTime = 0.1f;

        totalTime += deltaTime;
        lastFrameTime = high_resolution_clock::now().time_since_epoch();
    }
};
#pragma once

namespace ByteEngine
{
    template <typename T>
    class Singleton
    {
        friend class Application;

    private:
        static inline T* instance;

    public:
        Singleton(const Singleton&) = delete;
        virtual ~Singleton() = default;

        Singleton& operator=(const Singleton&) = delete;

        static T& GetInstance()
        {
            return *instance;
        }

    protected:
        Singleton() = default;
        Singleton(Singleton&&) = default;

        Singleton& operator=(Singleton&&) = default;

    private:
        static void SetInstance(T* newInstance)
        {
            instance = newInstance;
        }
    };
}
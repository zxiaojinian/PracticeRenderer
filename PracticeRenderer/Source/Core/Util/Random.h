#pragma once

#include <random>

namespace PR
{
    class Random 
    {
    public:
        static int Range(int min, int max) 
        {
            static std::random_device rd;
            static std::mt19937 gen(rd());
            std::uniform_int_distribution<int> distribution(min, max);
            return distribution(gen);
        }

        static float Range(float min, float max) 
        {
            static std::random_device rd;
            static std::mt19937 gen(rd());
            std::uniform_real_distribution<float> distribution(min, max);
            return distribution(gen);
        }
    };
}
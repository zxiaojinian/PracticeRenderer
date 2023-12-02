#pragma once

#include <random>

namespace PR
{
    class Random 
    {
    public:
        static void SetSeed(unsigned int seed)
        {
            std::mt19937 gen(seed);
            generator() = gen;
        }

        static int Range(int min, int max)
        {
            std::uniform_int_distribution<int> distribution(min, max);
            return distribution(generator());
        }

        static float Range(float min, float max)
        {
            std::uniform_real_distribution<float> distribution(min, max);
            return distribution(generator());
        }

    private:
        static std::mt19937& generator()
        {
            static std::random_device rd;
            static std::mt19937 gen(rd());
            return gen;
        }
    };
}
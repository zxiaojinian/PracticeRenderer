#pragma once

namespace PR
{
    enum class ColorWriteMask : uint8_t
    {
        Alpha = 0x1,
        Blue = 0x2,
        Green = 0x4,
        Red = 0x8,
        All = 0xF
    };
}

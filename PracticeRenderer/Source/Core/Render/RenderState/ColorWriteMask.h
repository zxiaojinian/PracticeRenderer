#pragma once

namespace PR
{
    enum class ColorWriteMask : uint8_t
    {
        None = 0,
        Alpha = 0x1,
        Blue = 0x2,
        Green = 0x4,
        Red = 0x8,
        All = 0xF
    };
}

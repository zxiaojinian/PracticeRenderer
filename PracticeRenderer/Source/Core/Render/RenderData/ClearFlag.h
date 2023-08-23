#pragma once

enum class ClearFlag
{
    None = 0,
    Color = 1,
    Depth = 2,

    All = Depth | Color
};


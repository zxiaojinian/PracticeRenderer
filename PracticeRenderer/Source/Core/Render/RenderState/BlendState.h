#pragma once

#include "ColorWriteMask.h"

namespace PR
{
    enum class BlendMode
    {
        Zero,
        One,
        SrcColor,
        DstColor,
        SrcAlpha,
        DstAlpha,
        OneMinusSrcColor,
        OneMinusDstColor,
        OneMinusSrcAlpha,
        OneMinusDstAlpha
    };

    enum class BlendOp
    {
        Add,
        Subtract,
        ReverseSubtract,
        Min,
        Max,
    };

    struct BlendState
    {
    public:
        ColorWriteMask WriteMask{ ColorWriteMask::All };
        bool EnableBlend{ false };
        BlendMode SourceColorBlendMode{ BlendMode::One };
        BlendMode DestinationColorBlendMode{ BlendMode::Zero };
        BlendMode SourceAlphaBlendMode{ BlendMode::One };
        BlendMode DestinationAlphaBlendMode{ BlendMode::Zero };
        BlendOp ColorBlendOperation{ BlendOp::Add };
        BlendOp AlphaBlendOperation{ BlendOp::Add };
    }; 
}

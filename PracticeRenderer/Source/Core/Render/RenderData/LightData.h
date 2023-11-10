#pragma once

#include "Core/Render/RenderData/Color.h"

#include <glm/glm.hpp>

namespace PR
{
    struct LightData
    {
        /// <summary>
        /// w=0-->dir
        /// </summary>
        glm::vec4 Position = { 0.0f, 0.0f, 1.0f, 1.0f };
        Color Color = { 0.0f, 0.0f, 0.0f, 0.0f };
        glm::vec4 Attenuation = { 0.0f, 1.0f, 0.0f, 1.0f };
        glm::vec4 SpotDirection = { 0.0f, 0.0f, 1.0f, 0.0f };
    };
}

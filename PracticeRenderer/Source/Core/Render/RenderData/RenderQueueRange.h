#pragma once

#include "Core/Common.h"

struct RenderQueueRange
{
public:
    uint32_t GetLowerBound() const { return m_LowerBound; }
    uint32_t GetUpperBound() const { return m_UpperBound; }
    void SetLowerBound(uint32_t value) { PR_ASSERT((value < 0 || value > 5000), "The lower bound must be at(0,5000)"); m_LowerBound = value; }
    void SetUpperBound(uint32_t value) { PR_ASSERT((value < 0 || value > 5000), "The upper bound must be at(0,5000)"); m_UpperBound = value; }

    static RenderQueueRange All()
    {
        RenderQueueRange result{};
        result.m_LowerBound = 0;
        result.m_UpperBound = 5000;
        return result;
    }

    static RenderQueueRange Opaque()
    {
        RenderQueueRange result{};
        result.m_LowerBound = 0;
        result.m_UpperBound = 2500;
        return result;
    }

    static RenderQueueRange Transparent()
    {
        RenderQueueRange result{};
        result.m_LowerBound = 2501;
        result.m_UpperBound = 5000;
        return result;
    }

private:
    uint32_t m_LowerBound;
    uint32_t m_UpperBound;
};

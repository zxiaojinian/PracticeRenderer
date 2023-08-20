#pragma once

enum class RenderPassEvent
{

    //before rendering any other passes in the pipeline
    BeforeRendering = 0,

    //before rendering shadowmaps
    BeforeRenderingShadows = 50,

    //after rendering shadowmaps
    AfterRenderingShadows = 100,

    //before rendering prepasses, f.ex, depth prepass
    BeforeRenderingPrepasses = 150,

    //after rendering prepasses, f.ex, depth prepass
    AfterRenderingPrePasses = 200,

    //before rendering opaque objects
    BeforeRenderingOpaques = 250,

    //after rendering opaque objects
    AfterRenderingOpaques = 300,

    //before rendering the sky
    BeforeRenderingSkybox = 350,

    //after rendering the sky
    AfterRenderingSkybox = 400,

    //before rendering transparent objects
    BeforeRenderingTransparents = 450,

    //after rendering transparent objects
    AfterRenderingTransparents = 500,

    //before rendering post-processing effects
    BeforeRenderingPostProcessing = 550,

    //after rendering post-processing effects but before final blit, post-processing AA effects and color grading
    AfterRenderingPostProcessing = 600,

    //after rendering all effects
    AfterRendering = 1000,
};
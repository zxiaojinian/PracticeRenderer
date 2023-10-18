#ifndef TiledCullingCommon
#define TiledCullingCommon

#define TILE_SIZE 16
#define MAX_NUM_LIGHTS_PER_TILE 32

uniform mat4 InverseProjection;
uniform vec4 ScreenParams_CS;//w,h,1/w,1/h

vec4 NDCToView(vec4 ndc)
{
    vec4 view = InverseProjection * ndc;
    view = view / view.w;
    return view;
}
 
vec4 ScreenToView(vec4 screen)
{
    vec2 screenPos01 = screen.xy * ScreenParams_CS.zw;
    vec4 ndc = vec4(screenPos01 * 2.0f - 1.0f, screen.z * 2.0 - 1.0, screen.w);
    return NDCToView(ndc);
}

#endif

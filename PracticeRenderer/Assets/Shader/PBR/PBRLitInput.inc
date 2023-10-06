#ifndef PBR_Lit_Input
#define PBR_Lit_Input

#include "../Common/Packing.inc"

struct SurfaceData
{
    vec3 albedo;
    float metallic;
    float smoothness;
    vec3 normalTS;
    vec3 emission;
    float occlusion;
    float alpha;
};

uniform sampler2D BaseMap;
uniform sampler2D MaskMap;
uniform sampler2D NormalMap;
uniform sampler2D HeightMap;

void InitializeLitSurfaceData(vec2 uv, out SurfaceData outSurfaceData)
{
    vec4 baseMap = texture(BaseMap, uv);
    if (baseMap.a < 0.5)
    {
        discard;
    }
    outSurfaceData.albedo = baseMap.rgb;
    outSurfaceData.alpha = baseMap.a;

    vec4 maskMap = texture(MaskMap, uv);
    vec4 normalMap = texture(NormalMap, uv);
    vec4 heightMap = texture(HeightMap, uv);

    outSurfaceData.metallic = maskMap.r;
    outSurfaceData.smoothness = maskMap.a;
    outSurfaceData.normalTS = UnpackNormal(normalMap.rgb);
    outSurfaceData.emission = vec3(0.0);
    outSurfaceData.occlusion = maskMap.g;
}

#endif
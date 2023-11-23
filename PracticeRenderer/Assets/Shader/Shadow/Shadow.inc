#ifndef Shadow_inc
#define Shadow_inc

#define MAX_SHADOW_CASCADES 4

layout (std140) uniform MainLightShadowData
{
    mat4 MainLightWorldToShadow[MAX_SHADOW_CASCADES + 1];
    vec4 CascadeShadowSplitSpheres[MAX_SHADOW_CASCADES];
};

uniform sampler2DArrayShadow  MainLightShadowmap;

int ComputeCascadeIndex(vec3 positionWS)
{
    vec3 fromCenter0 = positionWS - CascadeShadowSplitSpheres[0].xyz;
    vec3 fromCenter1 = positionWS - CascadeShadowSplitSpheres[1].xyz;
    vec3 fromCenter2 = positionWS - CascadeShadowSplitSpheres[2].xyz;
    vec3 fromCenter3 = positionWS - CascadeShadowSplitSpheres[3].xyz;
    vec4 distances2 = vec4(dot(fromCenter0, fromCenter0), dot(fromCenter1, fromCenter1), dot(fromCenter2, fromCenter2), dot(fromCenter3, fromCenter3));

    vec4 cascadeShadowSplitSphereRadius = vec4(CascadeShadowSplitSpheres[0].w, CascadeShadowSplitSpheres[1].w, CascadeShadowSplitSpheres[2].w, CascadeShadowSplitSpheres[3].w);
    // ivec4 weights = ivec4(distances2.x < cascadeShadowSplitSphereRadius.x, distances2.x < cascadeShadowSplitSphereRadius.x, distances2.x < cascadeShadowSplitSphereRadius.x, distances2.x < cascadeShadowSplitSphereRadius.x);
    ivec4 weights = ivec4(0);
    weights.yzw = clamp(weights.yzw - weights.xyz, ivec3(0), ivec3(1));

    return 4 - dot(weights, ivec4(4, 3, 2, 1)); //可能为4，故MainLightWorldToShadow长度为5
}

vec4 TransformWorldToShadowCoord(vec3 positionWS)
{
    int cascadeIndex = ComputeCascadeIndex(positionWS);
    vec4 shadowCoord = MainLightWorldToShadow[cascadeIndex] * vec4(positionWS, 1.0);
    shadowCoord.xyz = shadowCoord.xyz / shadowCoord.w;
    shadowCoord.xyz = shadowCoord.xyz * 0.5 + 0.5;
    shadowCoord.w = cascadeIndex;
    return shadowCoord;
}

float MainLightRealtimeShadow(vec4 shadowCoord)
{
    float shadowAttenuation = texture(MainLightShadowmap, shadowCoord).r;
    return shadowCoord.z <= 0.0 ? 1.0 : shadowAttenuation;//MainLightWorldToShadow最后一个矩阵变换后，坐标z=0
}

#endif
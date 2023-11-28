#ifndef Shadow_inc
#define Shadow_inc

#define MAX_SHADOW_CASCADES 4

layout (std140) uniform MainLightShadowData
{
    mat4 MainLightWorldToShadow[MAX_SHADOW_CASCADES + 1];
    vec4 CascadeShadowSplitSpheres[MAX_SHADOW_CASCADES];
    vec4 MainLightShadowmapSize;//(size, 1/size, 0, 0)
    float ShowCascade;
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
    ivec4 weights = ivec4(distances2.x < cascadeShadowSplitSphereRadius.x, distances2.y < cascadeShadowSplitSphereRadius.y, distances2.z < cascadeShadowSplitSphereRadius.z, distances2.w < cascadeShadowSplitSphereRadius.w);
    weights.yzw = clamp(weights.yzw - weights.xyz, ivec3(0), ivec3(1));
    return 4 - int(dot(weights, ivec4(4, 3, 2, 1))); //可能为4，故MainLightWorldToShadow长度为5
    // for(int i = 0; i < MAX_SHADOW_CASCADES; i++)
    // {
    //     vec3 fromCenter = positionWS - CascadeShadowSplitSpheres[i].xyz;
    //     if(dot(fromCenter, fromCenter) < CascadeShadowSplitSpheres[i].w)
    //     {
    //         return i;
    //     }
    // }
    // return MAX_SHADOW_CASCADES;
}

vec4 TransformWorldToShadowCoord(vec3 positionWS)
{
    int cascadeIndex = ComputeCascadeIndex(positionWS);
    vec4 shadowCoord = MainLightWorldToShadow[cascadeIndex] * vec4(positionWS, 1.0);
    shadowCoord.xyz = shadowCoord.xyz / shadowCoord.w;
    shadowCoord.xyz = shadowCoord.xyz * 0.5 + 0.5;

    shadowCoord.w = shadowCoord.z;
    shadowCoord.z = cascadeIndex;

    return shadowCoord;
}

float MainLightRealtimeShadow(vec4 shadowCoord)
{
    //4 samples hardware comparison = 3X3 sample pcf
    vec4 offset = MainLightShadowmapSize.yyyy * vec4(0.5, -0.5, 0.5, -0.5);
    float shadowAttenuation = 0.25 * texture(MainLightShadowmap, shadowCoord + vec4(offset.xz, 0.0, 0.0));//rt
    shadowAttenuation += 0.25 * texture(MainLightShadowmap, shadowCoord + vec4(offset.xw, 0.0, 0.0));//rb
    shadowAttenuation += 0.25 * texture(MainLightShadowmap, shadowCoord + vec4(offset.yz, 0.0, 0.0));//lt
    shadowAttenuation += 0.25 * texture(MainLightShadowmap, shadowCoord + vec4(offset.yw, 0.0, 0.0));//lb
    //float shadowAttenuation = texture(MainLightShadowmap, shadowCoord);
    return shadowCoord.z >= MAX_SHADOW_CASCADES ? 1.0 : shadowAttenuation;
}

void DebugCascade(float cascadeIndex, inout vec3 color)
{
    const vec3 cascadeColor[] = {vec3(1.0,0.0,0.0), vec3(0.0,1.0,0.0), vec3(0.0,0.0,1.0), vec3(1.0,1.0,0.0), vec3(0.0,0.0,0.0)};
    int index = clamp(int(cascadeIndex), 0, 4);
    color =  mix(color, cascadeColor[index], ShowCascade);
}

#endif
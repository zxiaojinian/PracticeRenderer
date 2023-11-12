#ifndef Shadow_inc
#define Shadow_inc

uniform mat4 Matrix_MainLightShadow;
uniform sampler2D MainLightShadowmap;

vec4 TransformWorldToShadowCoord(vec3 positionWS)
{
    vec4 shadowCoord = Matrix_MainLightShadow * vec4(positionWS, 1.0);
    return vec4(shadowCoord.xyz, 0.0);
}

float MainLightRealtimeShadow(vec4 shadowCoord)
{
    vec3 projCoords = shadowCoord.xyz / shadowCoord.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(MainLightShadowmap, projCoords.xy).r; 
    float currentDepth = projCoords.z;
    float shadow = currentDepth < closestDepth  ? 1.0 : 0.0;    
    return shadow;
}

#endif
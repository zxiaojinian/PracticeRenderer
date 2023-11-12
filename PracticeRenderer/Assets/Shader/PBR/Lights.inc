#ifndef Lights
#define Lights

#include "../Shadow/Shadow.inc"

struct LightData
{
    vec4 position;
    vec4 color;
    vec4 attenuation;
    vec4 spotDirection;
};

layout(std430) buffer LightDataBuffer
{
    LightData lightDataBuffer[];
};

#define FLT_MIN  1.175494351e-38 

struct Light
{
    vec3 direction;
    vec3 color;
    float distanceAttenuation;
    float shadowAttenuation;
    vec3 attenuationColor;
};

float DistanceAttenuation(float distanceSqr, vec2 distanceAttenuation)
{
    //From URP
    //attenuation = smoothFactor * 1.0 / distanceToLightSqr
    //smoothFactor = (1.0 - (distanceToLightSqr * 1.0 / lightRangeSqr) ^ 2) ^ 2
    //distanceAttenuation.x = 1.0 / lightRangeSqr

    float lightAtten = 1.0 / distanceSqr;
    float factor = distanceSqr * distanceAttenuation.x;
    float smoothFactor = clamp(1.0 - factor * factor, 0.0, 1.0);
    smoothFactor = smoothFactor * smoothFactor;

    return lightAtten * smoothFactor;
}

float AngleAttenuation(vec3 spotDirection, vec3 lightDirection, vec2 spotAttenuation)
{
    //From URP
    // Spot Attenuation with a linear falloff can be defined as
    // (SdotL - cosOuterAngle) / (cosInnerAngle - cosOuterAngle)
    // This can be rewritten as
    // invAngleRange = 1.0 / (cosInnerAngle - cosOuterAngle)
    // SdotL * invAngleRange + (-cosOuterAngle * invAngleRange)
    // SdotL * spotAttenuation.x + spotAttenuation.y

    // If we precompute the terms in a MAD instruction
    float SdotL = dot(spotDirection, lightDirection);
    float atten = clamp(SdotL * spotAttenuation.x + spotAttenuation.y, 0.0, 1.0);
    return atten * atten;
}

Light GetMainLight(vec4 shadowCoord)
{
    Light light;
    light.direction = lightDataBuffer[0].position.xyz;
    light.distanceAttenuation = 1.0;
    light.shadowAttenuation = MainLightRealtimeShadow(shadowCoord);
    light.color = lightDataBuffer[0].color.rgb;
    light.attenuationColor = light.color * light.distanceAttenuation * light.shadowAttenuation;
    return light;
}

Light GetLight(uint lightIndex, vec3 positionWS)
{
    lightIndex = lightIndex + 1;
    vec4 lightPositionWS = lightDataBuffer[lightIndex].position;
    vec3 color = lightDataBuffer[lightIndex].color.rgb;
    vec4 distanceAndSpotAttenuation = lightDataBuffer[lightIndex].attenuation;
    vec4 spotDirection = lightDataBuffer[lightIndex].spotDirection;

    // Directional lights store direction in lightPosition.xyz and have .w set to 0.0.
    // This way the following code will work for both directional and punctual lights.
    vec3 lightVector = lightPositionWS.xyz - positionWS * lightPositionWS.w;
    float distanceSqr = max(dot(lightVector, lightVector), FLT_MIN);

    vec3 lightDirection = vec3(lightVector * inversesqrt(distanceSqr));
    float attenuation = DistanceAttenuation(distanceSqr, distanceAndSpotAttenuation.xy) * AngleAttenuation(spotDirection.xyz, lightDirection, distanceAndSpotAttenuation.zw);

    Light light;
    light.direction = lightDirection;
    light.distanceAttenuation = attenuation;
    light.shadowAttenuation = 1.0;
    light.color = color;
    light.attenuationColor = light.color * light.distanceAttenuation * light.shadowAttenuation;
    return light;
}

#endif
struct Light
{
    float3   direction;
    float3   color;
    float    distanceAttenuation;
    float    shadowAttenuation;
};


float DistanceAttenuation(float distanceSqr, float2 distanceAttenuation)
{
    //From URP
    //attenuation = smoothFactor * 1.0 / distanceToLightSqr
    //smoothFactor = (1.0 - (distanceToLightSqr * 1.0 / lightRangeSqr) ^ 2) ^ 2
    //distanceAttenuation.x = 1.0 / lightRangeSqr

    float lightAtten = rcp(distanceSqr);
    float factor = distanceSqr * distanceAttenuation.x;
    float smoothFactor = saturate(1.0 - factor * factor);
    smoothFactor = smoothFactor * smoothFactor;

    return lightAtten * smoothFactor;
}

float AngleAttenuation(float3 spotDirection, float3 lightDirection, float2 spotAttenuation)
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
    float atten = saturate(SdotL * spotAttenuation.x + spotAttenuation.y);
    return atten * atten;
}

Light GetLight(int lightIndex, float3 positionWS)
{
    float4 lightPositionWS = _AdditionalLightsBuffer[perObjectLightIndex].position;
    float3 color = _AdditionalLightsBuffer[perObjectLightIndex].color.rgb;
    float4 distanceAndSpotAttenuation = _AdditionalLightsBuffer[perObjectLightIndex].attenuation;
    float4 spotDirection = _AdditionalLightsBuffer[perObjectLightIndex].spotDirection;

    // Directional lights store direction in lightPosition.xyz and have .w set to 0.0.
    // This way the following code will work for both directional and punctual lights.
    float3 lightVector = lightPositionWS.xyz - positionWS * lightPositionWS.w;
    float distanceSqr = max(dot(lightVector, lightVector), HALF_MIN);

    float3 lightDirection = float3(lightVector * rsqrt(distanceSqr));
    float attenuation = DistanceAttenuation(distanceSqr, distanceAndSpotAttenuation.xy) * AngleAttenuation(spotDirection.xyz, lightDirection, distanceAndSpotAttenuation.zw);

    Light light;
    light.direction = lightDirection;
    light.distanceAttenuation = attenuation;
    light.color = color;
}

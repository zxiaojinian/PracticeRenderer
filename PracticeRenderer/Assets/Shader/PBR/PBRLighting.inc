#ifndef PBR_Lighting
#define PBR_Lighting

#include "Lights.inc"
#include "PBRCommon.inc"

struct InputData
{
    vec3 positionWS;
    vec3 normalWS;
    vec3 viewDirectionWS;
    vec4 shadowCoord;
};

vec3 LightingPhysicallyBased(vec3 diffuseColor, vec3 specularColor, float roughness, vec3 V, vec3 N, vec3 L, vec3 lightColor)
{
    vec3 H = normalize(L + V);
    float NoH = clamp(dot(N, H), 0.0, 1.0);
    float NoV = clamp(abs(dot(N, V)) + 1e-5, 0.0, 1.0);//开启双面时可以区分正反面
    float NoL = clamp(dot(N, L), 0.0, 1.0);
    float VoH = clamp(dot(V, H), 0.0, 1.0);

    //BRDF,from UE4 DefaultLitBxDF
    vec3 diffuseBRDF = Diffuse_Lambert(diffuseColor);
    vec3 specularBRDF = SpecularGGX(specularColor, roughness, NoH, NoV, NoL, VoH);
    vec3 BRDF =  diffuseBRDF + specularBRDF;
    vec3 radiance = lightColor * NoL;

    //reflectance equation
    vec3 lighting = BRDF * radiance;
    return lighting;
}

vec3 DirectLighting(vec3 diffuseColor, vec3 specularColor, float roughness, vec3 posWS, vec3 N, vec3 V)
{
    vec3 directLighting = vec3(0.0);

    for (int i = 0; i < lightDataBuffer.length(); ++i) 
    {
        Light light = GetLight(i, posWS);
        vec3 L = light.direction;
        vec3 lightColor = light.attenuationColor;
        directLighting += LightingPhysicallyBased(diffuseColor, specularColor, roughness, V, N, L, lightColor);
    }
    return directLighting;
}

vec4 PBRLit(InputData inputData, SurfaceData surfaceData)
{
    vec4 finalColor = vec4(0.0);

    vec3 diffuseColor = mix(surfaceData.albedo, vec3(0.0), surfaceData.metallic);
    vec3 specularColor = mix(vec3(0.04, 0.04, 0.04), surfaceData.albedo, surfaceData.metallic);

    vec3 directLighting = DirectLighting(diffuseColor, specularColor, 1.0 - surfaceData.smoothness, inputData.positionWS, inputData.normalWS, inputData.viewDirectionWS);
    finalColor.rgb = directLighting;
    finalColor.a = surfaceData.alpha;
    return finalColor;
}

#endif
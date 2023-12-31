#ifndef PBR_Lighting
#define PBR_Lighting

#include "Lights.inc"
#include "PBRCommon.inc"
#include "Assets/Shader/LightCulling/TiledCulling/TiledLightingCommon.inc"

uniform samplerCube irradianceMap;
uniform samplerCube specularMap;

struct InputData
{
    vec3 positionWS;
    vec3 normalWS;
    vec3 viewDirectionWS;
    vec4 shadowCoord;
    vec3 posVS;
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

vec3 DirectLighting(vec3 diffuseColor, vec3 specularColor, float roughness, vec3 posWS, vec3 N, vec3 V, vec3 posVS, vec4 shadowCoord)
{
    vec3 directLighting = vec3(0.0);

    // for (uint i = 0; i < lightDataBuffer.length(); ++i) 
    // {
    //     Light light = GetLight(i, posWS);
    //     vec3 L = light.direction;
    //     vec3 lightColor = light.attenuationColor;
    //     directLighting += LightingPhysicallyBased(diffuseColor, specularColor, roughness, V, N, L, lightColor);
    // }

    Light light = GetMainLight(shadowCoord);
    vec3 L = light.direction;
    vec3 lightColor = light.attenuationColor;
    directLighting += LightingPhysicallyBased(diffuseColor, specularColor, roughness, V, N, L, lightColor);  

    //tiled light
    TileLightData data = GetTileLightData(posVS.z);
    for (int i = 0; i < data.lightCount; ++i)
    {
        uint lightIndex = GetTileLightIndex(data, i);
        Light light = GetLight(lightIndex, posWS);
        vec3 L = light.direction;
        vec3 lightColor = light.attenuationColor;
        directLighting += LightingPhysicallyBased(diffuseColor, specularColor, roughness, V, N, L, lightColor);        
    }

    return directLighting;
}


vec3 GlossyEnvironmentReflection(vec3 reflectVector, vec3 posWS, float roughness)
{
    const float MAX_REFLECTION_LOD = 4.0;
    return textureLod(specularMap, reflectVector, roughness * MAX_REFLECTION_LOD).rgb;
}

vec3 GlobalIllumination(vec3 diffuseColor, vec3 specularColor, float roughness, vec3 posWS, vec3 N, vec3 V, float occlusion)
{
    vec3 indirectLighting = vec3(0.0);

    //diffuse
    vec3 indirectDiffuse = diffuseColor * texture(irradianceMap, N).rgb;

    //specular
    vec3 R = reflect(-V, N); 
    vec3 speucularIrradiance = GlossyEnvironmentReflection(R, posWS, roughness);
    float NoV = clamp(abs(dot(N, V)) + 1e-5, 0.0, 1.0);    
    vec3 specularEnvBRDF = EnvBRDFApprox(specularColor, roughness, NoV);
    vec3 indirectSpecular = specularEnvBRDF * speucularIrradiance;

    indirectLighting = (indirectDiffuse + indirectSpecular) * occlusion;
    return indirectLighting;
}

vec4 PBRLit(InputData inputData, SurfaceData surfaceData)
{
    vec4 finalColor = vec4(0.0);

    vec3 diffuseColor = mix(surfaceData.albedo, vec3(0.0), surfaceData.metallic);
    vec3 specularColor = mix(vec3(0.04, 0.04, 0.04), surfaceData.albedo, surfaceData.metallic);
    float roughness = 1.0 - surfaceData.smoothness;

    vec3 directLighting = DirectLighting(diffuseColor, specularColor, roughness, inputData.positionWS, inputData.normalWS, inputData.viewDirectionWS, inputData.posVS, inputData.shadowCoord);
    vec3 gi = GlobalIllumination(diffuseColor, specularColor, roughness, inputData.positionWS, inputData.normalWS, inputData.viewDirectionWS, 1.0);

    finalColor.rgb = directLighting + gi;
    finalColor.a = surfaceData.alpha;

    DebugCascade(inputData.shadowCoord.z, finalColor.rgb);
    return finalColor;
}

#endif
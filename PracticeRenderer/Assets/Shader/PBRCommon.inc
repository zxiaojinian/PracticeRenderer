
#define PI 3.14159265358979323846

vec3 Diffuse_Lambert(vec3 DiffuseColor)
{
    return DiffuseColor * (1 / PI);
}

float Pow4(float x)
{
    float xx = x*x;
    return xx * xx;
}

float Pow5(float x)
{
    return x*x * x*x * x;
}

float D_GGX_UE4(float a2, float NoH)
{
    float d = ( NoH * a2 - NoH ) * NoH + 1;	// 2 mad
    return a2 / ( PI*d*d );					// 4 mul, 1 rcp
}

// Appoximation of joint Smith term for GGX
// [Heitz 2014, "Understanding the Masking-Shadowing Function in Microfacet-Based BRDFs"]
float Vis_SmithJointApprox(float a2, float NoV, float NoL)
{
    float a = sqrt(a2);
    float Vis_SmithV = NoL * ( NoV * ( 1 - a ) + a );
    float Vis_SmithL = NoV * ( NoL * ( 1 - a ) + a );
    // return 0.5 * rcp( Vis_SmithV + Vis_SmithL );
    return 0.5 / ( Vis_SmithV + Vis_SmithL );
}

// [Schlick 1994, "An Inexpensive BRDF Model for Physically-Based Rendering"]
vec3 F_Schlick_UE4(vec3 SpecularColor, float VoH)
{
    float Fc = Pow5( 1 - VoH );					// 1 sub, 3 mul
    //return Fc + (1 - Fc) * SpecularColor;		// 1 add, 3 mad

    // Anything less than 2% is physically impossible and is instead considered to be shadowing
    return clamp(50.0 * SpecularColor.g, 0.0, 1.0) * Fc + (1 - Fc) * SpecularColor;
}

// Generalized microfacet specular,from UE4 SpecularGGX
vec3 SpecularGGX(vec3 specularColor, float roughness, float NoH, float NoV, float NoL, float VoH)
{
    float a2 = Pow4(roughness);
    float D = D_GGX_UE4(a2, NoH);
    float Vis = Vis_SmithJointApprox( a2, NoV, NoL);
    vec3 F = F_Schlick_UE4( specularColor, VoH );
    return D * Vis * F;
}

vec3 StandardDirectLighting(vec3 DiffuseColor, vec3 SpecularColor, float Roughness, vec3 V, vec3 N, vec3 L, vec3 LightColor, float Shadow)
{
    vec3 H = normalize(L + V);
    float NoH = clamp(dot(N, H), 0.0, 1.0);
    float NoV = clamp(abs(dot(N, V)) + 1e-5, 0.0, 1.0);//开启双面时可以区分正反面
    float NoL = clamp(dot(N, L), 0.0, 1.0);
    float VoH = clamp(dot(V, H), 0.0, 1.0);

    //BRDF,from UE4 DefaultLitBxDF
    vec3 diffuseBRDF = Diffuse_Lambert(DiffuseColor);
    vec3 specularBRDF = SpecularGGX(SpecularColor, Roughness, NoH, NoV, NoL, VoH);
    vec3 BRDF =  diffuseBRDF + specularBRDF;
    vec3 Radiance = LightColor * Shadow;

    //reflectance equation
    vec3 lighting = BRDF * Radiance * NoL;
    return lighting;
}

void Standard_DirectLighting(vec3 diffuseColor, vec3 specularColor, float roughness, vec3 posWS, vec3 N, vec3 V, out vec3 directLighting)
{
    directLighting = vec3(0.0);

    for (int i = 0; i < lightDataBuffer.length(); ++i) 
    {
        Light light = GetLight(i, posWS);
        vec3 L = light.direction;
        vec3 lightColor = light.color;
        float shadow = light.shadowAttenuation;
        directLighting += StandardDirectLighting(diffuseColor, specularColor, roughness, V, N, L, lightColor, shadow);
    }
}
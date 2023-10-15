
#ifndef PBR_Common
#define PBR_Common

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
    return x * x * x * x * x;
}

// GGX / Trowbridge-Reitz
// [Walter et al. 2007, "Microfacet models for refraction through rough surfaces"]
float D_GGX(float a2, float NoH)
{
    float d = ( NoH * a2 - NoH ) * NoH + 1; // 2 mad
    return a2 / ( PI*d*d );         // 4 mul, 1 rcp
}

// Appoximation of joint Smith term for GGX
// [Heitz 2014, "Understanding the Masking-Shadowing Function in Microfacet-Based BRDFs"]
float Vis_SmithJointApprox(float a, float NoV, float NoL)
{
    //float a = sqrt(a2);
    float Vis_SmithV = NoL * ( NoV * ( 1 - a ) + a );
    float Vis_SmithL = NoV * ( NoL * ( 1 - a ) + a );
    // return 0.5 * rcp( Vis_SmithV + Vis_SmithL );
    return 0.5 / ( Vis_SmithV + Vis_SmithL );
}

// [Schlick 1994, "An Inexpensive BRDF Model for Physically-Based Rendering"]
vec3 F_Schlick(vec3 SpecularColor, float VoH)
{
    float Fc = Pow5( 1 - VoH );                 // 1 sub, 3 mul
    //return Fc + (1 - Fc) * SpecularColor;     // 1 add, 3 mad

    // Anything less than 2% is physically impossible and is instead considered to be shadowing
    return clamp(50.0 * SpecularColor.g, 0.0, 1.0) * Fc + (1 - Fc) * SpecularColor;
}

// Generalized microfacet specular,from UE4 SpecularGGX
vec3 SpecularGGX(vec3 specularColor, float roughness, float NoH, float NoV, float NoL, float VoH)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float D = D_GGX(a2, NoH);
    float Vis = Vis_SmithJointApprox( a, NoV, NoL);
    vec3 F = F_Schlick( specularColor, VoH );
    return D * Vis * F;
}

//from ue4
vec3 EnvBRDFApprox( vec3 SpecularColor, float Roughness, float NoV )
{
    // [ Lazarov 2013, "Getting More Physical in Call of Duty: Black Ops II" ]
    // Adaptation to fit our G term.
    const vec4 c0 = vec4(-1.0, -0.0275, -0.572, 0.022);
    const vec4 c1 = vec4(1.0, 0.0425, 1.04, -0.04);
    vec4 r = Roughness * c0 + c1;
    float a004 = min( r.x * r.x, exp2( -9.28 * NoV ) ) * r.x + r.y;
    vec2 AB = vec2( -1.04, 1.04 ) * a004 + r.zw;

    // Anything less than 2% is physically impossible and is instead considered to be shadowing
    // Note: this is needed for the 'specular' show flag to work, since it uses a SpecularColor of 0
    AB.y *= clamp( 50.0 * SpecularColor.g, 0.0, 1.0 );

    return SpecularColor * AB.x + AB.y;
}

#endif

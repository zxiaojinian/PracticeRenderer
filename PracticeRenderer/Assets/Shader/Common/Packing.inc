#ifndef Packing
#define Packing

vec3 UnpackNormal(vec3 packedNormal, float scale = 1.0)
{
    vec3 normal;
    normal.xyz = packedNormal.rgb * 2.0 - 1.0;
    normal.xy *= scale;
    return normal;
}
#endif
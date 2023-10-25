#ifndef TiledCullingCommon
#define TiledCullingCommon

#define TILE_SIZE 16
#define MAX_NUM_LIGHTS_PER_TILE 64

uniform mat4 InverseProjection;
uniform vec4 ScreenParams_CS;//w,h,1/w,1/h

vec4 NDCToView(vec4 ndc)
{
    vec4 view = InverseProjection * ndc;
    view = view / view.w;
    return view;
}
 
vec4 ScreenToView(vec4 screen)
{
    vec2 screenPos01 = screen.xy * ScreenParams_CS.zw;
    vec4 ndc = vec4(screenPos01 * 2.0 - 1.0, screen.z * 2.0 - 1.0, screen.w);
    return NDCToView(ndc);
}


struct Sphere
{
    vec3 c;
    float  r;
};

struct AABB
{
    vec3 c; // center
    vec3 e; // half extents
};


void ComputeFrustumDualAABB(uvec2 groupID, float minZ, float maxZ2, float minZ2, float maxZ, out AABB aabb1,out AABB aabb2)
{
    uint px = TILE_SIZE * groupID.x;
    uint py = TILE_SIZE * groupID.y;
    uint pxp = TILE_SIZE * (groupID.x + 1);
    uint pyp = TILE_SIZE * (groupID.y + 1);

    //远裁剪面
    vec3 frustumTL = ScreenToView(vec4(px, pyp, 1.0, 1.0)).xyz;
    vec3 frustumBR = ScreenToView(vec4(pxp, py, 1.0, 1.0)).xyz;

    {
        //naer aabb
        float factorMax = maxZ2 / frustumTL.z;
        vec2 frustumTopLeftAtBack = frustumTL.xy * factorMax;//相似三角形
        vec2 frustumBottomRightAtBack = frustumBR.xy * factorMax;
        float factorMin = minZ / frustumTL.z;
        vec2 frustumTopLeftAtFront = frustumTL.xy * factorMin;
        vec2 frustumBottomRightAtFront = frustumBR.xy * factorMin;

        vec2 frustumMinXY = min( frustumTopLeftAtBack, min( frustumBottomRightAtBack, min(frustumTopLeftAtFront, frustumBottomRightAtFront) ) );
        vec2 frustumMaxXY = max( frustumTopLeftAtBack, max( frustumBottomRightAtBack, max(frustumTopLeftAtFront, frustumBottomRightAtFront) ) );

        vec3 frustumAABBMin = vec3(frustumMinXY.x, frustumMinXY.y, minZ);
        vec3 frustumAABBMax = vec3(frustumMaxXY.x, frustumMaxXY.y, maxZ2);

        aabb1.c = (frustumAABBMin + frustumAABBMax) * 0.5;
        aabb1.e = abs(frustumAABBMax - aabb1.c);
    }

    {
        //far aabb
        float factorMax = maxZ / frustumTL.z;
        vec2 frustumTopLeftAtBack = frustumTL.xy * factorMax;//相似三角形
        vec2 frustumBottomRightAtBack = frustumBR.xy * factorMax;
        float factorMin = minZ2 / frustumTL.z;;
        vec2 frustumTopLeftAtFront = frustumTL.xy * factorMin;
        vec2 frustumBottomRightAtFront = frustumBR.xy * factorMin;

        vec2 frustumMinXY = min( frustumTopLeftAtBack, min( frustumBottomRightAtBack, min(frustumTopLeftAtFront, frustumBottomRightAtFront) ) );
        vec2 frustumMaxXY = max( frustumTopLeftAtBack, max( frustumBottomRightAtBack, max(frustumTopLeftAtFront, frustumBottomRightAtFront) ) );

        vec3 frustumAABBMin = vec3(frustumMinXY.x, frustumMinXY.y, minZ2);
        vec3 frustumAABBMax = vec3(frustumMaxXY.x, frustumMaxXY.y, maxZ);

        aabb2.c = (frustumAABBMin + frustumAABBMax) * 0.5f;
        aabb2.e = abs(frustumAABBMax - aabb2.c);
    }
}

bool SphereIntersectsAABB(Sphere sphere, AABB aabb)
{
    vec3 vDelta = max(vec3(0.0), abs(aabb.c - sphere.c) - aabb.e);
    float fDistSq = dot(vDelta, vDelta);
    return fDistSq <= sphere.r * sphere.r;
}

#endif

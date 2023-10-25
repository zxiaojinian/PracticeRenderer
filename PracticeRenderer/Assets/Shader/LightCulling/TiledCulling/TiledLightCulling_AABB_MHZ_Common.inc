#ifndef TiledLightCulling_AABB_MHZ_Common
#define TiledLightCulling_AABB_MHZ_Common

#include "TiledCullingCommon.inc"

#define NUM_THREADS_CULLING TILE_SIZE * TILE_SIZE

layout (local_size_x = TILE_SIZE, local_size_y = TILE_SIZE, local_size_z = 1) in;

uniform sampler2D depthBounds;

#if defined(LightCountDebug)
    layout (rgba32f) writeonly uniform image2D TiledCullingDebugTexture;
#endif

layout(std430) buffer LightsCullingDataBuffer
{
    Sphere lightsCullingData[];
};

layout(std430) buffer LightIndexListDoubleBuffer
{
    uint lightIndexListDouble[];
};

shared uint ldsLightCountA;
shared uint ldsLightCountB;
shared uint ldsLightList[MAX_NUM_LIGHTS_PER_TILE * 2];

shared float ldsTileHalfZ;
shared AABB ldsTileFrustumAABB1;
shared AABB ldsTileFrustumAABB2;


// shared uint uMinDepth;
// shared uint uMaxDepth;
// uniform sampler2D CmeraDepthTexture;

//gpu pro
void LightCulling() 
{
    // float fDepth = texelFetch(CmeraDepthTexture, ivec2(gl_GlobalInvocationID.xy), 0).r;    
    // uint uDepth = floatBitsToUint( fDepth );

    // if(gl_LocalInvocationIndex == 0)
    // {    
    //     uMinDepth = 0xffffffff;
    //     uMaxDepth = 0;
    // }
    // barrier();
    // memoryBarrierShared();  

    // atomicMin( uMinDepth, uDepth );
    // atomicMax( uMaxDepth, uDepth );
 
    // barrier();
    // memoryBarrierShared();  

    // if(gl_LocalInvocationIndex == 0)
    // {
    //     float fMinDepth = uintBitsToFloat( uMinDepth );
    //     float fMaxDepth = uintBitsToFloat( uMaxDepth );  
    //     fMinDepth = NDCToView(vec4(0.0, 0.0, 2.0 * fMinDepth - 1.0, 1.0)).z;       
    //     fMaxDepth = NDCToView(vec4(0.0, 0.0, 2.0 * fMaxDepth - 1.0, 1.0)).z; 
    //     float fHalfZ = (fMinDepth + fMaxDepth) * 0.5;
    //     ldsTileHalfZ = fHalfZ;

    //     ComputeFrustumDualAABB(gl_WorkGroupID.xy, fMinDepth, fHalfZ, fHalfZ, fMaxDepth, ldsTileFrustumAABB1, ldsTileFrustumAABB2);

    //     ldsLightCountA = 0;
    //     ldsLightCountB = 0;
    // }

    if(gl_LocalInvocationIndex == 0)
    {
        vec4 depthBounds = texelFetch(depthBounds, ivec2(gl_WorkGroupID.xy), 0);
        float tileMinDepth = depthBounds.x;
        float tileMaxDepth = depthBounds.w;
        float tileMinDepth2 = depthBounds.z;
        float tileMaxDepth2 = depthBounds.y;
        ldsTileHalfZ = (tileMinDepth + tileMaxDepth) * 0.5;

        ComputeFrustumDualAABB(gl_WorkGroupID.xy, tileMinDepth, tileMaxDepth2, tileMinDepth2, tileMaxDepth, ldsTileFrustumAABB1, ldsTileFrustumAABB2);

        ldsLightCountA = 0;
        ldsLightCountB = 0;
    }
    barrier();
    memoryBarrierShared();

    for (uint i = gl_LocalInvocationIndex; i < lightsCullingData.length(); i += NUM_THREADS_CULLING)
    {
        bool testResult1 = false;//near half
        bool testResult2 = false;//far half

        Sphere lightSphere = lightsCullingData[i];
        testResult1 = SphereIntersectsAABB(lightSphere, ldsTileFrustumAABB1);
        testResult2 = SphereIntersectsAABB(lightSphere, ldsTileFrustumAABB2);

        if(testResult1)
        {
            uint index = atomicAdd(ldsLightCountA, 1);
            
            if (index < MAX_NUM_LIGHTS_PER_TILE)
            {
                ldsLightList[index] = i;
            }
        }

        if(testResult2)
        {
            uint index = atomicAdd(ldsLightCountB, 1);
            
            if (index < MAX_NUM_LIGHTS_PER_TILE)
            {
                ldsLightList[MAX_NUM_LIGHTS_PER_TILE + index] = i;
            }            
        }
    }
    barrier();
    memoryBarrierShared();  

    uint tileFlatIndex = gl_WorkGroupID.x + gl_WorkGroupID.y * gl_NumWorkGroups.x;
    uint startOffset = tileFlatIndex * (MAX_NUM_LIGHTS_PER_TILE  * 2 + 4);  
    uint lightStartOffset = startOffset + 4;
    uint lightCountA =  min(ldsLightCountA, MAX_NUM_LIGHTS_PER_TILE);
    for ( uint i = gl_LocalInvocationIndex; i < lightCountA; i += NUM_THREADS_CULLING)
    {
        lightIndexListDouble[lightStartOffset + i] = ldsLightList[i];
    }

    lightStartOffset += MAX_NUM_LIGHTS_PER_TILE;
    uint lightCountB =  min(ldsLightCountB, MAX_NUM_LIGHTS_PER_TILE);
    for ( uint i = gl_LocalInvocationIndex; i < lightCountB; i += NUM_THREADS_CULLING)
    {
        lightIndexListDouble[lightStartOffset + i] = ldsLightList[i + MAX_NUM_LIGHTS_PER_TILE];
    }

    if(gl_LocalInvocationIndex == 0)
    {
        float fHalfZ = ldsTileHalfZ;

        // store fHalfZ for this tile as two 16-bit unsigned values
        uint uHalfZBits = floatBitsToUint(fHalfZ);
        uint uHalfZBitsHigh = uHalfZBits >> 16;
        uint uHalfZBitsLow = uHalfZBits & 0x0000FFFF;
        lightIndexListDouble[startOffset+0] = uHalfZBitsHigh;
        lightIndexListDouble[startOffset+1] = uHalfZBitsLow;

        // store the light count for list A
        lightIndexListDouble[startOffset+2] = lightCountA;

        // store the light count for list B
        lightIndexListDouble[startOffset+3] = lightCountB;
    }

    #if defined(LightCountDebug)
        if (gl_LocalInvocationID.x == 0 || gl_LocalInvocationID.y == 0)
        {
            imageStore(TiledCullingDebugTexture, ivec2(gl_GlobalInvocationID.xy), vec4(0.0, 0.0, 0.0, 1.0));
        }
        else if (gl_LocalInvocationID.x == 1 || gl_LocalInvocationID.y == 1)
        {
            imageStore(TiledCullingDebugTexture, ivec2(gl_GlobalInvocationID.xy), vec4(1.0, 1.0, 1.0, 1.0));
        }
        else
        {
            uint lightCount = max(ldsLightCountA, ldsLightCountB);
            if(lightCount <= MAX_NUM_LIGHTS_PER_TILE)
            {
                const vec3 heatMap[] = {    vec3(0.0,0.0,0.0),
                                            vec3(0.0,0.0,1.0),
                                            vec3(0.0,1.0,1.0),
                                            vec3(0.0,1.0,0.0),
                                            vec3(1.0,1.0,0.0),
                                            vec3(1.0,0.0,0.0) };
                const float mapTexLen = 5.0;
                float l = clamp(float(lightCount) / MAX_NUM_LIGHTS_PER_TILE, 0.0, 1.0) * mapTexLen;
                vec3 a = heatMap[int(floor(l))];
                vec3 b = heatMap[int(ceil(l))];
                vec4 heatMapColor = vec4(mix(a, b, l - floor(l)), 1.0);
                imageStore(TiledCullingDebugTexture, ivec2(gl_GlobalInvocationID.xy), heatMapColor);
            }
            else
            {
                imageStore(TiledCullingDebugTexture, ivec2(gl_GlobalInvocationID.xy), vec4(0.5, 0.0, 1.0, 1.0));
            }
        }
    #endif
}

#endif


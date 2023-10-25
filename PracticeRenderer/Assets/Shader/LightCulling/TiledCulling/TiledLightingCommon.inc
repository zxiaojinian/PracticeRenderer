#ifndef TiledLightingCommon
#define TiledLightingCommon

#include "TiledCullingCommon.inc"
uniform int NumTilesX;

layout(std430) buffer LightIndexListDoubleBuffer
{
    uint lightIndexListDouble[];
};


struct TileLightData
{
    uint startOffset;
    uint lightCount;
};

TileLightData GetTileLightData(float poszVS)
{
    TileLightData data;

    uvec2 tileIndex = uvec2(floor(gl_FragCoord.xy / TILE_SIZE));
    uint tileFlatIndex = tileIndex.x + tileIndex.y * NumTilesX;
    uint startIndex = (MAX_NUM_LIGHTS_PER_TILE  * 2 + 4) * tileFlatIndex;

    // reconstruct fHalfZ
    uint uHalfZBitsHigh = lightIndexListDouble[startIndex];
    uint uHalfZBitsLow = lightIndexListDouble[startIndex + 1];
    uint uHalfZBits = (uHalfZBitsHigh << 16) | uHalfZBitsLow;
    float fHalfZ = uintBitsToFloat(uHalfZBits);//负数
    float fViewPosZ = poszVS;

    data.startOffset = (fViewPosZ > fHalfZ) ? (startIndex + 4) : (startIndex + 4 + MAX_NUM_LIGHTS_PER_TILE);
    data.lightCount = (fViewPosZ > fHalfZ) ? lightIndexListDouble[startIndex + 2] : lightIndexListDouble[startIndex + 3];
    data.lightCount = min(data.lightCount, MAX_NUM_LIGHTS_PER_TILE);

    return data;
}

uint GetTileLightIndex(TileLightData data, int index)
{
    return lightIndexListDouble[data.startOffset + index];
}

#endif

#type vertex

#version 460 core

layout(location = 0) in vec3 positionOS;
layout(location = 1) in vec3 normalOS;
layout(location=2) in vec2 texcoord;
layout(location=3) in vec3 tangentOS;
layout(location=4) in vec3 bitangentOS;

out VS_OUT 
{
    vec2 uv;
    vec4 normalWS;
    vec4 tangentWS;
    vec4 bitangentWS;
    vec3 posVS;
} vs_out;

uniform mat4 Matrix_M;
uniform mat4 Matrix_I_M;

layout (std140) uniform CameraMatrix
{
    mat4 Matrix_V;
    mat4 Matrix_P;
    mat4 Matrix_VP;
};

void main()
{
    vec4 posWS = Matrix_M * vec4(positionOS, 1.0);
    gl_Position = Matrix_VP * posWS;

    vec3 normalWS = normalize(normalOS * mat3(Matrix_I_M));
    vec3 tangentWS = normalize(mat3(Matrix_M) * tangentOS);
    vec3 bitangentWS = normalize(mat3(Matrix_M) * bitangentOS);

    vs_out.uv = texcoord;
    vs_out.normalWS = vec4(normalWS, posWS.x);
    vs_out.tangentWS = vec4(tangentWS, posWS.y);
    vs_out.bitangentWS = vec4(bitangentWS, posWS.z);
    vs_out.posVS = vec3(Matrix_V * posWS);
}


#type fragment

#version 460 core

in VS_OUT 
{
    vec2 uv;
    vec4 normalWS;
    vec4 tangentWS;
    vec4 bitangentWS;
    vec3 posVS;
} fs_in;

layout (std140) uniform CameraData
{
    vec4 worldSpaceCameraPos;
};

layout(location = 0) out vec4 fragColor;

#include "PBRLitInput.inc"
#include "PBRLighting.inc"

void InitializeInputData(vec3 normalTS, out InputData inputData)
{
    inputData.positionWS = vec3(fs_in.normalWS.w, fs_in.tangentWS.w, fs_in.bitangentWS.w);

    vec3 normalWS = normalize(fs_in.normalWS.xyz);
    vec3 tangentWS = normalize(fs_in.tangentWS.xyz);
    vec3 bitangentWS = normalize(fs_in.bitangentWS.xyz);
    mat3 TBN = mat3(tangentWS, bitangentWS, normalWS);
    inputData.normalWS = normalize(TBN * normalTS);

    inputData.viewDirectionWS = normalize(worldSpaceCameraPos.xyz - inputData.positionWS);
    inputData.shadowCoord = TransformWorldToShadowCoord(inputData.positionWS);
    inputData.posVS = fs_in.posVS;
}

void main()
{
    SurfaceData surfaceData;
    InitializeLitSurfaceData(fs_in.uv, surfaceData);

    InputData inputData;
    InitializeInputData(surfaceData.normalTS, inputData);

    //inputData.normalWS = vec3(fs_in.normalWS.xyz);
    fragColor = PBRLit(inputData, surfaceData);
    //fragColor.rgb = surfaceData.albedo;
    //fragColor.rgb = surfaceData.normalTS;
    // fragColor.rgb = vec3(fs_in.uv.x, 0.0, 0.0);
    //fragColor.rgb = fs_in.tangentWS.rgb;
}

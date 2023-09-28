//vertex shader
#type vertex

#version 460 core

layout(location = 0) in vec3 positionOS;
layout(location = 1) in vec3 normalOS;
layout(location=2) in vec3 tangentOS;
layout(location=3) in vec3 bitangentOS;
layout(location=4) in vec2 texcoord;

out VS_OUT 
{
    vec2 uv;
    vec4 normalWS;
    vec4 tangentWS;
    vec4 bitangentWS;
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
}


//fragment shader
#type fragment

#version 460 core

in VS_OUT 
{
    vec2 uv;
    vec4 normalWS;
    vec4 tangentWS;
    vec4 bitangentWS;
} fs_in;

layout (std140) uniform CameraData
{
    vec4 worldSpaceCameraPos;
};

layout(location = 0) out vec4 fragColor;

#include "Lighting.inc"
#include "PBRCommon.inc"

void main()
{
    vec2 uv = fs_in.uv;
    vec3 posWS = vec3(fs_in.normalWS.w, fs_in.tangentWS.w, fs_in.bitangentWS.w);
    vec3 viewDirWS = normalize(worldSpaceCameraPos.xyz - posWS);
    vec3 normalWS = normalize(fs_in.normalWS.xyz);
    vec3 tangentWS = normalize(fs_in.tangentWS.xyz);
    vec3 bitangentWS = normalize(fs_in.bitangentWS.xyz);
    mat3 TBN = mat3(tangentWS, bitangentWS, normalWS);

    vec3 directLighting = vec3(0.0);
    vec3 diffuseColor = vec3(0.5);
    vec3 specularColor = vec3(0.1);
    float roughness = 0.45;

    Standard_DirectLighting(diffuseColor, specularColor, roughness, posWS, normalWS, viewDirWS, directLighting);
    vec3 indirectLighting = vec3(0.0);
    fragColor = vec4(directLighting + indirectLighting, 1.0);
}

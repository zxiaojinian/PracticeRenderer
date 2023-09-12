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
    vec3 normalWS;
    vec3 tangentWS;
    vec3 bitangentWS;
} vs_out;

uniform mat4 Matrix_M;
uniform mat4 Matrix_I_M;
uniform mat4 Matrix_VP;

void main()
{
    gl_Position = Matrix_VP * Matrix_M * vec4(positionOS, 1.0);
    vs_out.uv = texcoord;
    vs_out.normalWS = normalize(normalOS * mat3(Matrix_I_M));
    vs_out.tangentWS = normalize(mat3(Matrix_M) * tangentOS);
    vs_out.bitangentWS = normalize(mat3(Matrix_M) * bitangentOS);
}


#type fragment

#version 460 core

in VS_OUT 
{
    vec2 uv;
    vec3 normalWS;
    vec3 tangentWS;
    vec3 bitangentWS;
} fs_in;

layout(location = 0) out vec4 fragColor;

uniform vec4 u_LightPosition;

void main()
{
    vec3 normalWS = normalize(fs_in.normalWS);
    float diff = max(dot(normalWS, u_LightPosition.xyz), 0.0);
    fragColor = vec4(diff, diff, diff, 1.0);
}

#type vertex

#version 460 core

layout(location = 0) in vec3 positionOS;

out VS_OUT 
{
    vec3 positionOS;
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
    gl_Position = Matrix_VP * Matrix_M * vec4(positionOS, 1.0);
    vec3 posOS = vec3(positionOS.x, positionOS.y, positionOS.z);
    vs_out.positionOS = vec3(posOS.x, posOS.y, -posOS.z);
}


#type fragment

#version 460 core

in VS_OUT 
{
    vec3 positionOS;
} fs_in;

layout(location = 0) out vec4 fragColor;

uniform samplerCube SkyCubeMap;
//uniform samplerCube specularMap;

void main()
{
    fragColor = texture(SkyCubeMap, fs_in.positionOS);
    //fragColor = textureLod(specularMap, fs_in.positionOS, 1);
}

#type vertex

#version 460 core

layout(location = 0) in vec3 positionOS;

uniform mat4 Matrix_M;

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
}


#type fragment

#version 460 core

layout(location = 0) out vec4 fragColor;

void main()
{
    fragColor = vec4(0.0);
}

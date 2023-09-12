#type vertex

#version 460 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;

out vec3 normal;

uniform mat4 u_Matrix_M;
uniform mat4 u_Matrix_VP;

void main()
{
	gl_Position = u_Matrix_VP * u_Matrix_M * vec4(a_Position, 1.0);
	normal = a_Normal;
}

#type fragment

#version 460 core

layout(location = 0) out vec4 color;

in vec3 normal;

uniform vec4 u_LightPosition;

void main()
{
    float diff = max(dot(normalize(normal), u_LightPosition.xyz), 0.0);
	color = vec4(diff, diff, diff, 1.0);
}
// Flat Color Shader

#type vertex

#version 460 core

layout(location = 0) in vec3 a_Position;

//uniform mat4 u_Matrix_M;
//uniform mat4 u_Matrix_VP;

void main()
{
	//gl_Position = Matrix_VP * Matrix_M * vec4(a_Position, 1.0);
	gl_Position = vec4(a_Position, 1.0);
}

#type fragment

#version 460 core

layout(location = 0) out vec4 color;

uniform vec4 u_Color;

void main()
{
	color = u_Color;
}
#type vertex

#version 460 core

layout(location = 0) in vec3 a_Position;
layout(location = 2) in vec2 a_TexCoord;

out vec2 texCoord;

//uniform mat4 u_Matrix_M;
//uniform mat4 u_Matrix_VP;

void main()
{
    //gl_Position = Matrix_VP * Matrix_M * vec4(a_Position, 1.0);
    gl_Position = vec4(a_Position, 1.0);
    texCoord = a_TexCoord;
}

#type fragment

#version 460 core

in vec2 texCoord;

layout(location = 0) out vec4 fragColor;

uniform sampler2D u_SourceTex;

void main()
{
    fragColor = texture(u_SourceTex, texCoord);
    fragColor.rgb = fragColor.rgb / (fragColor.rgb + vec3(1.0));
    fragColor.rgb = pow(fragColor.rgb, vec3(0.45));
}

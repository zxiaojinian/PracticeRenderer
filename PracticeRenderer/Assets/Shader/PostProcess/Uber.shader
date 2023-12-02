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

uniform float bloomStrength;
uniform float exposure = 1.0;

uniform sampler2D srcTexture;
uniform sampler2D bloomTexture;

void main()
{
    vec3 finalColor = texture(srcTexture, texCoord).rgb;
    vec3 bloomColor = texture(bloomTexture, texCoord).rgb;
    finalColor = mix(finalColor, bloomColor, bloomStrength);

    // tone mapping
    finalColor = vec3(1.0) - exp(-finalColor * exposure);
    // gamma correct
    const float gamma = 2.2;
    finalColor = pow(finalColor, vec3(1.0 / gamma));
    fragColor = vec4(finalColor, 1.0);
}

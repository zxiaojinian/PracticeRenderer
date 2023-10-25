#type vertex

#version 460 core

layout(location = 0) in vec3 a_Position;
layout(location = 2) in vec2 a_TexCoord;

out vec2 texCoord;

void main()
{
    gl_Position = vec4(a_Position, 1.0);
    texCoord = a_TexCoord;
}

#type fragment

#version 460 core

in vec2 texCoord;

layout(location = 0) out vec4 fragColor;

uniform sampler2D TiledCullingDebugTexture;
uniform float debugAlpha;

void main()
{
    fragColor = texture(TiledCullingDebugTexture, texCoord);
    fragColor.a = debugAlpha;
}

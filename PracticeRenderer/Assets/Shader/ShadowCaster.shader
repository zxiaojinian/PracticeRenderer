#type vertex

#version 460 core

layout(location = 0) in vec3 positionOS;
layout(location = 1) in vec3 normalOS;

uniform mat4 Matrix_M;
uniform mat4 Matrix_I_M;

layout (std140) uniform CameraMatrix
{
    mat4 Matrix_V;
    mat4 Matrix_P;
    mat4 Matrix_VP;
};

uniform vec4 ShadowBias; //(lightDirectionBias, normalBias, 0.0f, 0.0f)
uniform vec4 LightDirection;

vec3 ApplyShadowBias(vec3 positionWS, vec3 normalWS)
{
    float invNdotL = 1.0 - clamp(dot(LightDirection.xyz, normalWS), 0.0, 1.0);
    float scale = invNdotL * ShadowBias.y;

    positionWS = LightDirection.xyz * ShadowBias.xxx + positionWS;
    positionWS = normalWS * scale.xxx + positionWS;
    return positionWS;
}

void main()
{
    vec4 posWS = Matrix_M * vec4(positionOS, 1.0);
    vec3 normalWS = normalize(normalOS * mat3(Matrix_I_M));
    posWS.xyz = ApplyShadowBias(posWS.xyz, normalWS);
    gl_Position = Matrix_VP * posWS;
}


#type fragment

#version 460 core

layout(location = 0) out vec4 fragColor;

void main()
{
    fragColor = vec4(0.0);
}

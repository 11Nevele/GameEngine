#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 atextureCord;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;
uniform mat4 projection;
uniform mat4 rotation;

uniform vec3 uLightPos;

out vec2 textureCord;
out vec3 relativeLightPos;
out vec3 normal;
void main()
{
    normal = normalize((rotation * vec4(aNormal,1.0)).xyz);
    relativeLightPos = normalize(uLightPos - (u_Transform * vec4(aPos, 1.0)).xyz);
    textureCord = atextureCord;
    gl_Position = projection * u_ViewProjection * u_Transform * vec4(aPos, 1.0);
}
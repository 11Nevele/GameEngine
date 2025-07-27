#version 330 core
layout (location = 0) in vec3 vertex;
layout (location = 1) in vec2 textureCord;
out vec2 TexCoords;

uniform mat4 projection;
uniform mat4 u_ViewProjection;

void main()
{
    gl_Position = projection * u_ViewProjection * vec4(vertex, 1.0);
    TexCoords = textureCord;
}  
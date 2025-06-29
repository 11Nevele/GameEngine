#version 330 core

layout(location = 0) in vec3 aPos;      // Vertex positions


// Transformation matrices
uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;
uniform mat4 projection;


void main()
{
    // Transform the vertex position
    gl_Position = projection * u_ViewProjection * u_Transform * vec4(aPos, 1.0);
}
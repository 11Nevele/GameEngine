#version 330 core

layout(location = 0) out vec4 color;
in vec2 textureCord;
in vec3 relativeLightPos;
in vec3 normal;

uniform vec4 uColor;
uniform sampler2D u_Texture;
uniform vec3 uAmbientLightColor;
uniform vec3 uLightColor;


  
void main()
{
    vec4 pointLightColor = vec4(uLightColor * max(dot(normal, relativeLightPos), 0.0), 1.0);
    color = texture(u_Texture, textureCord) * (vec4(uAmbientLightColor,1.0) + pointLightColor);
    //color = vec4(textureCord, 0 ,1);

} 
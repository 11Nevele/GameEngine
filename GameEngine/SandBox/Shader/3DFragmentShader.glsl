#version 330 core

layout(location = 0) out vec4 color;
in vec2 textureCord;
uniform vec4 uColor;
uniform sampler2D u_Texture;
uniform vec3 uLightColor;



  
void main()
{
    color = (texture(u_Texture, textureCord) * vec4(uLightColor,1.0));
    //color = vec4(textureCord, 0 ,1);

} 
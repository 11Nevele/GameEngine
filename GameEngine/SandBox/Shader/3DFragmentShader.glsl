#version 330 core

layout(location = 0) out vec4 color;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float     shininess;
};  

in vec2 textureCord;
in vec3 relativeLightPos;
in vec3 normal;
in vec3 viewDir;
in vec3 reflectDir;

uniform vec4 uColor;
uniform Material uMaterial;
uniform vec3 uAmbientLightColor;
uniform vec3 uLightColor;


  
void main()
{
    float diff = max(dot(normal, relativeLightPos), 0.0);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), uMaterial.shininess);
    if(uMaterial.shininess == 0.0)
        spec = 0.0;

    vec3 lightColor = uLightColor * max(dot(normal, relativeLightPos), 0.0);
    vec3 ambient  = uAmbientLightColor  * vec3(texture(uMaterial.diffuse, textureCord));
    vec3 diffuse  = lightColor  * diff * vec3(texture(uMaterial.diffuse, textureCord));  
    vec3 specular = lightColor * spec * vec3(texture(uMaterial.specular, textureCord));

    color = vec4(ambient + diffuse + specular, 1.0);
    //color = vec4(textureCord, 0 ,1);

} 
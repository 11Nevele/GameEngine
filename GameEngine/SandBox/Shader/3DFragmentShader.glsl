#version 330 core

layout(location = 0) out vec4 color;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float     shininess;
};  

in vec2 textureCord;
in vec3 fragPos;
in vec3 normal;
in vec3 viewDir;

uniform vec4 uColor;
uniform Material uMaterial;
uniform vec3 uAmbientLightColor;
uniform int uLightCount;
uniform vec3 uLightPos[4];
uniform vec3 uLightColor[4];


  
void main()
{
    vec3 lightContribution = vec3(0.0);
    vec3 ambient  = uAmbientLightColor  * vec3(texture(uMaterial.diffuse, textureCord));

    for (int i = 0; i < uLightCount; i++)
    {
        vec3 lightDir = normalize(uLightPos[i] - fragPos);
        float diff = max(dot(normal, lightDir), 0.0);
        vec3 reflectDir = reflect(-lightDir, normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), uMaterial.shininess);
        if(uMaterial.shininess == 0.0)
            spec = 0.0;

        vec3 diffuse  = uLightColor[i] * diff * vec3(texture(uMaterial.diffuse, textureCord));
        vec3 specular = uLightColor[i] * spec * vec3(texture(uMaterial.specular, textureCord));
        lightContribution += diffuse + specular;
    }

    color = vec4(ambient + lightContribution, 1.0);
    //color = vec4(textureCord, 0 ,1);

} 
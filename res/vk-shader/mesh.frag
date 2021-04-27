#version 440

struct SolidMaterial {
    vec3 color;
    vec3 specular;
    float shininess;
};

struct SimpleLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

layout(location = 0) in vec3 vertNormal;
layout(location = 1) in vec3 vertPos;
layout(location = 2) in vec3 vertColor;

layout(binding = 1)
uniform FragUniformBufferObject {
    vec3 cameraPos;
    SimpleLight light;
    SolidMaterial material;
} uniBuf;

layout(location = 0) out vec4 fragColor;

void main()
{
    vec3 ambient = uniBuf.light.ambient * uniBuf.material.color;

    vec3 lightDir = normalize(uniBuf.light.position - vertPos);
    float diff = max(dot(vertNormal, lightDir), 0.0);
    vec3 diffuse = uniBuf.light.diffuse * diff * uniBuf.material.color;

    //specular
    vec3 viewDir = normalize(uniBuf.cameraPos - vertPos);
    vec3 reflectDir = reflect(-lightDir, vertNormal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), uniBuf.material.shininess);
    vec3 specular = uniBuf.light.specular * (spec * uniBuf.material.specular);

    // vec3 result = ambient + diffuse + specular;
    vec3 result = ambient ;

    fragColor = vec4(result, 1.0f);
    // fragColor = vec4(uniBuf.material.color, 1.0f);
    // fragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
